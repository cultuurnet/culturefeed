<?php
/**
 * @file
 * Block callbacks for culturefeed messages.
 */

/**
 * Show a users inbox inside a block.
 */
function culturefeed_messages_block_inbox() {

  try {
    $messages = DrupalCultureFeed::getLoggedInUserInstance()->messages()->getMessages();

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_messages', $e);
    return;
  }

  $block['subject'] = 'Inbox';
  $block['content'] = array(
    '#theme' => 'culturefeed_messages_message_list',
    '#messages' => $messages,
  );

  return $block;

}