<?php

/**
 * @class
 * Exception when a service method returned an invalid code.
 */
class CultureFeed_InvalidCodeException extends Exception {
  public $message;

  function __construct($message, $code, $format = 'xml') {
    $this->message = 'Invalid code returned from service: ' . $code . ' - ' . $message;
    $this->code = $code;
  }
}