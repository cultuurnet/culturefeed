<?php

class CultureFeed_Exception extends Exception {
  public $error_code;

  /**
   * @var string
   */
  protected $userFriendlyMessage;

  function __construct($message, $error_code) {
    parent::__construct($message, 0);
    $this->error_code = $error_code;
  }

  /**
   * @return string|null
   */
  public function getUserFriendlyMessage() {
    return $this->userFriendlyMessage;
  }

  /**
   * @param string $message
   */
  public function setUserFriendlyMessage($message) {
    if (!is_string($message)) {
      $exception_message = sprintf('%s requires argument 1 to be a string, %s given.', __METHOD__, gettype($message));
      throw new InvalidArgumentException($exception_message, 1);
    }
    $this->userFriendlyMessage = $message;
  }
}
