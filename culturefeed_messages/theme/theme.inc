<?php
/**
 * @file
 * Theming / preprocess functions for culturefeed messages.
 */

/**
 * Preprocess a messages list.
 * @see culturefeed-messages-message-list.tpl.php
 */
function culturefeed_messages_preprocess_culturefeed_messages_message_list(&$variables) {

  $items = array();
  if ($variables['messages']->total == 0) {
    $items[] = array('data' => t('You have no messages'), 'class' => 'message');
  }
  else {

    if (arg(0) == 'pages') {
      $active_message = arg(3);
    }
    else {
      $active_message = arg(2);
    }


    $selected_id = 0;
    foreach ($variables['messages']->objects as $i => $message) {
      if ($message->id == $active_message) {
        $selected_id = $i;
      }
    }

    foreach ($variables['messages']->objects as $i => $message) {

      $class = 'message';
      $class_name = '';
      if ($i == $selected_id) {
        $class .= ' active';
        $class_name = 'active';
      }

      $items[] =  array(
        'data' => theme('culturefeed_messages_message_list_item', array('message' => $message, 'class_name' => $class_name)),
        'class' => $class,
      );

    }

  }

  $variables['items'] = $items;

  if ($variables['add_new_allowed']) {
    if (arg(0) == 'pages') {
      $variables['add_new_url'] = url('pages/' . arg(1) . '/messages/new');
    }
    else {
      $variables['add_new_url'] = url('culturefeed/messages/new');
    }
  }

}

/**
 * Preprocess a message list item.
 * @see culturefeed-messages-message-list-item.tpl.php
 */
function culturefeed_messages_preprocess_culturefeed_messages_message_list_item(&$variables) {

  _culturefeed_messages_preprocess_message($variables);
  _culturefeed_messages_preprocess_message_recipients($variables);

  $classes = array('message', 'message-' . $variables['status']);
  if (!empty($variables['class_name'])) {
    $classes[] = $variables['class_name'];
  }
  $variables['class_names'] = implode(" ", $classes);

  if (arg(0) == 'pages') {
    $variables['url'] = url('pages/' . arg(1) . '/messages/' . $variables['message']->id);
  }
  else {
    $variables['url'] = url('culturefeed/messages/' . $variables['message']->id);
  }

}

/**
 * Preprocess a message thread.
 * @see culturefeed-messages-thread.tpl.php
 */
function culturefeed_messages_preprocess_culturefeed_messages_thread(&$variables) {

  $thread = $variables['thread'];

  $variables['message_count'] = 1;

  $recipient_links = array();
  $recipients = $thread->recipients;
  $sender_id = $thread->sender->id;

  // Build link to profile of every recipient.
  $variables['recipient_links'] = '';
  $variables['recipient'] = '';
  if (!empty($recipients)) {

    $accounts = culturefeed_get_uids_for_cf_uids($recipients);
    foreach ($recipients as $uid => $recipient) {

      // Don't show the sender in the list.
      if ($uid == $sender_id || !isset($accounts[$uid])) {
        $sender_link = l($recipient->nick, 'user/' . $accounts[$uid]);
        continue;
      }

      $recipient_links[] = l($recipient->nick, 'user/' . $accounts[$uid]);

    }
    $variables['recipient_links'] = implode(', ', $recipient_links);
    $variables['recipient'] = $variables['recipient_links'];
  }

  if (!empty($thread->recipientPage)) {
    $variables['recipient_page'] = check_plain($thread->recipientPage->getName());
    $variables['recipient_page_link'] = culturefeed_search_detail_l('page', $thread->recipientPage->getId(), $thread->recipientPage->getName());
    $variables['recipient'] = t('!name of !page', array('!name' => $variables['recipient_links'], '!page' => $variables['recipient_page_link']));
  }

  $variables['sender_link'] = $sender_link;
  $variables['sender'] = check_plain($thread->sender->nick);
  if (!empty($thread->senderPage)) {
    $variables['sender_page'] = check_plain($thread->senderPage->getName());
    $variables['sender_page_link'] = culturefeed_search_detail_l('page', $thread->senderPage->getId(), $thread->senderPage->getName());
    $variables['sender'] = t('!name of !page', array('!name' => $variables['sender_link'], '!page' => $variables['sender_page_link']));
  }

  $variables['messages'] = array();
  $thread_type = empty($thread->type) ? 'default' : $thread->type;
  $variables['messages'][] = theme('culturefeed_messages_message_' . $thread_type, array('message' => $thread));

  $thread_children = $thread->children;
  if (!empty($thread_children)) {
    $variables['message_count'] += count($thread_children);
    foreach ($thread_children as $message) {
      $message_type = empty($message->type) ? 'default' : $message->type;
      $variables['messages'][] = theme('culturefeed_messages_message_' . $message_type, array('message' => $message));
    }
  }

  $variables['delete_link'] = l(t('Delete conversation'), 'culturefeed/messages/' . $thread->id . '/delete/nojs');

  $variables['subject'] = $thread->subject;

}

/**
 * Preprocess a message item with empty type.
 * @see culturefeed-messages-message-default.tpl.php
 */
function culturefeed_messages_preprocess_culturefeed_messages_message_default(&$variables) {

  _culturefeed_messages_preprocess_message($variables);
  _culturefeed_messages_preprocess_message_recipients($variables);

}

/**
 * Preprocess a message item of type contactmembers.
 * @see culturefeed-messages-message-contactmembers.tpl.php
 */
function culturefeed_messages_preprocess_culturefeed_messages_message_contactmembers(&$variables) {

  _culturefeed_messages_preprocess_message($variables);
  _culturefeed_messages_preprocess_message_recipients($variables);

}

/**
 * Preprocess a message item of type contactpage.
 * @see culturefeed-messages-message-contactpage.tpl.php
 */
function culturefeed_messages_preprocess_culturefeed_messages_message_contactpage(&$variables) {

  _culturefeed_messages_preprocess_message($variables);
  _culturefeed_messages_preprocess_message_recipients($variables);

}

/**
 * Preprocess a message item of type booking.
 * @see culturefeed-messages-message-booking.tpl.php
 */
function culturefeed_messages_preprocess_culturefeed_messages_message_booking(&$variables) {

  _culturefeed_messages_preprocess_message($variables);
  _culturefeed_messages_preprocess_message_recipients($variables);

}

/**
 * Preprocess a message item of type report.
 * @see culturefeed-messages-message-report.tpl.php
 */
function culturefeed_messages_preprocess_culturefeed_messages_message_report(&$variables) {

  _culturefeed_messages_preprocess_message($variables);
  _culturefeed_messages_preprocess_message_recipients($variables);

}

/**
 * Preprocess a message item of type requestadmin.
 * @see culturefeed-messages-message-requestadmin.tpl.php
 */
function culturefeed_messages_preprocess_culturefeed_messages_message_requestadmin(&$variables) {

  _culturefeed_messages_preprocess_message($variables);
  _culturefeed_messages_preprocess_message_recipients($variables);

}

/**
 * Preprocess recipients for a message.
 */
function _culturefeed_messages_preprocess_message_recipients(&$variables) {

  $message = $variables['message'];
  $recipient_links = array();
  $recipients = $message->recipients;
  $sender_id = $message->sender->id;

  // Build link to profile of every recipient.
  $variables['recipient_links'] = '';
  if (!empty($recipients)) {

    $accounts = culturefeed_get_uids_for_cf_uids($recipients);
    foreach ($recipients as $uid => $recipient) {

      // Don't show the sender in the list.
      if ($uid == $sender_id || !isset($accounts[$uid])) {
        continue;
      }

      $recipient_links[] = l($recipient->nick, 'user/' . $accounts[$uid]);

    }
    $variables['recipient_links'] = implode(', ', $recipient_links);

  }

  if (!empty($message->recipientPage)) {
      $variables['recipient_page'] =  check_plain($message->recipientPage->getName());
  }

}

/**
 * Helper function to preprocess the general variables for a message.
 */
function _culturefeed_messages_preprocess_message(&$variables) {

  $message = $variables['message'];

  $variables['id'] = $message->id;
  $variables['status'] = $message->status;
  $variables['subject'] = $message->subject;

  // Body can be a json. If it is, add the array to the variables.
  $body = drupal_json_decode($message->body);
  if ($body && is_array($body)) {
    $variables += $body;
  }
  else {
    $variables['body'] = $message->body;
  }

  if (isset($variables['body'])) {
    $variables['body'] = check_markup($variables['body'], 'filtered_html');
  }

  $variables['sender'] = check_plain($message->sender->nick);
  if (!empty($message->senderPage)) {
    $variables['sender_page'] = check_plain($message->senderPage->getName());
  }

  $variables['picture'] = !empty($message->sender->depiction) ? theme('image', array('path' => $message->sender->depiction . '?maxwidth=100')) : '';

  $date = empty($message->lastReply) ? $message->creationDate : $message->lastReply;
  $time_zone  = new DateTimeZone('Europe/Brussels');
  $last_reply = new DateTime();
  $last_reply->setTimestamp($date);
  $last_reply->setTimezone($time_zone);

  // Check if we should show x minutes ago, or not.
  $time_ago = $_SERVER['REQUEST_TIME'] - $date;
  if ($time_ago < (60 * 60)) {
    $minutes_ago = $last_reply->diff(new DateTime('now', $time_zone))->i;
    $variables['date'] = format_plural($minutes_ago, '@count minute ago', '@count minutes ago');
  }
  else {

    if ($date >= strtotime("today")) {
      $variables['date'] = t('Today at') . ' ' . $last_reply->format('H:i') . ' '  . t('hour');
    }
    else if ($date >= strtotime("yesterday")) {
      $variables['date'] = t('Yesterday at') . ' ' . $last_reply->format('H:i') . ' '  . t('hour');
    }
    else {
      $variables['date'] = format_date($date, 'custom', 'd M \o\m H:i') . ' '  . t('hour');
    }

  }
}

/**
 * Theme the total messages profile box item.
 */
function theme_culturefeed_messages_total_messages_profile_box_item($variables) {
  return l(format_plural($variables['total'], '@count new message', '@count new messages'), 'culturefeed/messages');
}
