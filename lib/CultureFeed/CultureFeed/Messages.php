<?php

/**
 * @class
 * Interface for messages management
 */
interface CultureFeed_Messages {

  /**
   * Send a message.
   * @param string $recipient
   *   Uid of the recipient
   * @param string $recipientPage
   *   Id of the page that should receive the message
   * @param string $replyTo
   *   Id of the message to reply to
   * @param string $role
   *   Send this message to people that have the given role for the given recipientPage.
   * @param array $params
   *   Optional params
   *
   * 1 of following 3 arguments is required: recepient, recipientPage or replyTo
   * If recipientPage is given, a role is also required.
   */
  public function sendMessage($recipient = '', $recipientPage = '', $role = '', $replyTo = '', $params = array());

}
