<?php
/**
 * @file
 * Page callbacks for culturefeed messages.
 */

/**
 * Page callback to via a message. If we are on the landing page, show the first one.
 * @param mixed $message NULL or CultureFeed_Messages_Message
 */
function culturefeed_messages_page_view_message($message = NULL, $page = NULL) {

  // If we are on landing page, show the first message.
  try {

    if (!$message) {
      $recipient_page = empty($page) ? NULL : $page->getId();
      $messages = DrupalCultureFeed::getLoggedInUserInstance()->messages()->getMessages($recipient_page);
      if ($messages->total == 0) {
        return array('#markup' => t('You have no messages'));
      }

      $message = DrupalCultureFeed::getLoggedInUserInstance()->messages()->getMessage($messages->objects[0]->id);
    }

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_messages', $e);
    return array('#markup' => t('Something went wrong while getting your message.'));
  }

  $build = array();
  $build['message_thread'] = array(
    '#theme' => 'culturefeed_messages_thread',
    '#thread' => $message,
  );

  $build['reply-form'] = drupal_get_form('culturefeed_messages_new_message_form', $message);

  return $build;

}

/**
 * Show the form to add a new message.
 * @param $object
 *   Object to send this message to. Can be a page or a message.
 */
function culturefeed_messages_new_message_form($form, &$form_state, $object = NULL) {

  $form = array();
  $form['title'] = array('#markup' => t('New message'));

  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
  );

  $form['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
  );

  // If empty object. Load active page. This form needs a page.
  if (empty($object)) {
    $object = culturefeed_pages_get_active_page();
    if (empty($object)) {
      return;
    }
  }

  $send_as_option = TRUE;
  // If we are on a page. Options are to members or to admins.
  if ($object instanceof CultureFeed_Cdb_Item_Page) {

    try {

      $cf_pages = DrupalCultureFeed::getConsumerInstance()->pages();
      $user_list = $cf_pages->getUserList($object->getId(), array(CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN, CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_MEMBER), FALSE);

      if (!empty($user_list->memberships)) {

        $member_count = 0;
        $admin_count = 0;

        foreach ($user_list->memberships as $user_list_membership) {
          if ($user_list_membership->role == CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN) {
            $admin_count++;
          }
          else {
            $member_count++;
          }
        }

      }

      $form['recipient'] = array(
        '#type' => 'radios',
        '#title' => t('Send new message to:'),
        '#options' => array(
          CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN => t('all administrators of my page') . ' (' . $admin_count . ')',
          CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_MEMBER => t('all members of my page') . ' (' . $member_count . ')',
        ),
      );

      $form_state['page'] = $object;

    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_messages', $e);
      return $form;
    }

  }
  // If object is a message. We are sending a reply.
  elseif ($object instanceof CultureFeed_Messages_Message) {
    $form['title']['#markup'] = t('Reply');
    $form['subject']['#value'] = 'RE:' . ' ' . $object->subject;
    $form['subject']['#access'] = FALSE;
    $form_state['reply_to'] = $object->id;
    $form_state['message_type'] = $object->type;
  }

  // Add select to choose the 'send as'.
  if ($send_as_option) {
    try {

      $cf_user = DrupalCultureFeed::getLoggedInUser();

      if (!empty($cf_user->pageMemberships)) {
        $send_as_options = array();
        foreach ($cf_user->pageMemberships as $membership) {
          $send_as_options[$membership->page->getId()] = $membership->page->getName();
        }

        // Always use recipientPage as senderPage in replies
        if ($object instanceof CultureFeed_Messages_Message && $object->recipientPage instanceof CultureFeed_Cdb_Item_Page) {
              $form['send_as'] = array(
                '#type' => 'hidden',
                '#default_value' => $object->recipientPage->getId(),
              );
        }

        else {
          $form['send_as'] = array(
            '#title' => t('Send message as'),
            '#type' => 'select',
            '#options' => $send_as_options,
            '#default_value' => culturefeed_pages_get_active_page_id(),
          );
        }
      }
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_messages', $e);
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send'),
  );

  return $form;

}

/**
 * Validate the send message form. Try to send the message to the service.
 * Note: only sending a page, or a reply is available in the UI.
 */
function culturefeed_messages_new_message_form_validate($form, &$form_state) {

  $params = array(
    'subject' => $form_state['values']['subject'],
    'body' => str_replace("\n", "<br />", $form_state['values']['message']),
  );

  if (!empty($form_state['values']['send_as'])) {
    $params['senderPage'] = $form_state['values']['send_as'];
  }
  // New messages always use current page as default.
  else {
    if (module_exists('culturefeed_pages')) {
      $curr_page = culturefeed_pages_get_active_page_id();
      if ($curr_page) {
        $params['senderPage'] = $curr_page;
      }
    }
  }

  // Check if we are sending to a page.
  if (!empty($form_state['page'])) {
    $params['recipientPage'] = $form_state['page']->getId();
    $params['role'] = $form_state['values']['recipient'];
    $params['type'] = CultureFeed_Messages_Message::TYPE_MEMBERS;
  }
  // Check if we are sending a reply.
  elseif (!empty($form_state['reply_to'])) {
    $params['replyTo'] = $form_state['reply_to'];
    $params['type'] = $form_state['message_type'];
  }
  else {
    $params['recipient'] = $form_state['values']['recipient'];
  }

  try {
    $message_id = DrupalCultureFeed::getLoggedInUserInstance()->messages()->sendMessage($params);
  }
  catch (Exception $e) {
    form_set_error('', t('Something went wrong while sending your message.'));
    watchdog_exception('culturefeed_messages', $e);
  }

}

/**
 * Submit the new message form.
 */
function culturefeed_messages_new_message_form_submit($form, &$form_state) {

  drupal_set_message(t('Message send'));

  $form_state['redirect'] = 'culturefeed/messages';
  if (!empty($form_state['page'])) {
    $form_state['redirect'] = 'pages/' . $form_state['page']->getId() . '/messages';
  }

  if (!empty($form_state['reply_to'])) {
    $form_state['redirect'] .= '/' . $form_state['reply_to'];
  }

}

/**
 * Page callback to add a confirimation to the
 * @param CultureFeed_Cdb_Item_Page $page
 */
function culturefeed_messages_page_delete_message_confirm($message, $request_type = 'ajax') {

  $form = drupal_get_form('culturefeed_messages_delete_message_confirm_form', $message, $request_type);

  if ($request_type == 'ajax') {
    $output = drupal_render($form);
    print $output;
  }
  else {
    return $form;
  }

}

/**
 * Form confirmation callback to show a form to confirm the removal of a message.
 */
function culturefeed_messages_delete_message_confirm_form($form, &$form_state, CultureFeed_Messages_Message $message, $request_type) {

  if ($request_type == 'ajax') {
    $form['#prefix'] = '<h3>' . t('Delete message') . '</h3>';
  }

  $form_state['message'] = $message;

  $form['page_remove_confirm'] = array(
    '#type' => 'markup',
    '#markup' => '
      <p>' . t('This action cannot be undone') . '</p>
      <p><strong>' . t('Are you sure you want to delete this message?') . '</strong></p>
      <hr />',
  );

  $form['remove'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#attributes' => array('class' => array('button-remove')),
  );

  $form['decline'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => isset($_GET['destination']) ? $_GET['destination'] : 'culturefeed/messages/' . $message->id,
    '#attributes' => array('class' => array('button-decline')),
  );

  return $form;

}

/**
 * Function to actually delete the page.
 */
function culturefeed_messages_delete_message_confirm_form_submit($form, &$form_state) {

  $message = $form_state['message'];

  // Send request to remove the message.
  try {
    DrupalCultureFeed::getLoggedInUserInstance()->messages()->deleteMessage($message->id);
    drupal_set_message(format_string(t('Message  "@message" is deleted.'), array('@message' => $message->subject)));
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_messages', $e);
    drupal_set_message(t('Something went wrong while deleting your message.'), 'error');
  }

  $form_state['redirect'] = 'culturefeed/messages';

}
