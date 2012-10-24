<?php

/**
 * @class
 * Exception when a service method returned an invalid code.
 */
class CultureFeed_InvalidCodeException extends Exception {
  public $message;

  function __construct($code, $message, $format = 'xml') {
    parent::__construct('Invalid code returned from service: ' . $message, $code);
    $this->message = $message;
  }
}