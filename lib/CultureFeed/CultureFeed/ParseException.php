<?php

class CultureFeed_ParseException extends Exception {
  public $text;

  function __construct($text, $format = 'xml') {
    parent::__construct('Could not parse text. Expected ' . $format, 0);
    $this->text = $text;
  }
}