<?php

class CultureFeed_Exception extends Exception {
  public $error_code;

  function __construct($message, $error_code) {
    parent::__construct($message, 0);
    $this->error_code = $error_code;
  }
}