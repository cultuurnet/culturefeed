<?php
/**
 * @file
 * Block callbacks for culturefeed messages.
 */

/**
 * Show a users inbox inside a block.
 * @param mixed $page
 *   CultureFeed_Cdb_Item_Page if we want only messages to this page.
 */
function culturefeed_messages_block_inbox($page = NULL) {

  if (!culturefeed_is_culturefeed_user()) {
    return;
  }

  try {
    $recipient_page = empty($page) ? NULL : $page->getId();
    $messages = DrupalCultureFeed::getLoggedInUserInstance()->messages()->getMessages($recipient_page);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_messages', $e);
    return;
  }

  $block['subject'] = 'Inbox';
  $block['content'] = array(
    '#theme' => 'culturefeed_messages_message_list',
    '#messages' => $messages,
    '#add_new_allowed' => (!module_exists('culturefeed_pages') || culturefeed_pages_get_active_page_id()), // Only admins can send message with normal form.
  );

  return $block;

}