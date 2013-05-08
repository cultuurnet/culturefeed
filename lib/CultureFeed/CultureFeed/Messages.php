<?php

/**
 * @class
 * Interface for messages management
 */
interface CultureFeed_Messages {

  /**
   * Get the message count for current user.
   */
  public function getMessageCount();

  /**
   * Get the messages for current user.
   * @param string $recipientPage
   *   Filter the list on messages that are send to this page.
   * @param string $type
   *   Filter the list on type.
   */
  public function getMessages($recipientPage = NULL, $type = NULL);

  /**
   * Get the given message and it's children.
   * @param unknown $id
   */
  public function getMessage($id);

  /**
   * Send a message.
   * @param array $params
   *   Array of params to send
   *
   * 1 of following 3 param arguments is required: recepient, recipientPage or replyTo
   * If recipientPage is given, a role is also required.
   */
  public function sendMessage($params);

}
