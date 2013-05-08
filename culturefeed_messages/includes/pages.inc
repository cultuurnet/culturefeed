<?php
/**
 * @file
 * Page callbacks for culturefeed messages.
 */

/**
 * Show the form to add a new message.
 */
function culturefeed_messages_new_message_form($form, $form_state, $page = NULL) {

  $form = array();

  $recipient_options = array();

  // If we are on a page. Options are to members or to admins.
  if (!empty($page) && $page instanceof CultureFeed_Cdb_Item_Page) {

    try {

      $cf_pages = DrupalCultureFeed::getConsumerInstance()->pages();
      $user_list = $cf_pages->getUserList($page->getId(), array(CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN, CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_MEMBER), FALSE);

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

      $recipient_options[CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN] = 'alle beheerders van mijn pagina (' . $admin_count . ')';
      $recipient_options[CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_MEMBER] = 'alle leden van mijn pagina (' . $member_count . ')';

      $form_state['page'] = $page;

    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_messages', $e);
      return $form;
    }

  }

  $form['recipient'] = array(
    '#type' => 'radios',
    '#title' => 'Stuur bericht naar:',
    '#options' => $recipient_options,
  );

  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => 'Onderwerp',
  );

  $form['message'] = array(
    '#type' => 'textarea',
    '#title' => 'Berichttekst',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Verzenden',
  );

  return $form;

}

/**
 * Validate the send message form. Try to send the message to the service.
 * @param unknown $form
 * @param unknown $form_state
 */
function culturefeed_messages_new_message_form_validate($form, &$form_state) {

  $recipient = '';
  $recipient_page = '';
  $role = '';
  $reply_to = '';
  // Check if we are sending to a page.
  if (!empty($form['page'])) {
    $recipient_page = $form['page']->getId();
    $role = $form_state['values']['recipient'];
  }
  // Check if we are sending a reply.
  elseif (!empty($form['reply_to'])) {
    $reply_to = $form['reply_to'];
  }
  else {
    $recipient = $form_state['values']['recipient'];
  }

  $params = array(

  );

  $cf_messages= DrupalCultureFeed::getLoggedInUserInstance()->messages();
  $cf_messages->sendMessage($recipient, $recipient_page, $role, $reply_to, $params);

}