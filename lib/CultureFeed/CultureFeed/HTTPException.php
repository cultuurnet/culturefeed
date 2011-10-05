<?php

class CultureFeed_HTTPException extends Exception {
  public $body;
  
  function __construct($body, $code) {
    parent::__construct('The reponse for the HTTP request was not 200.', $code);
    $this->body = $body;
  }
}