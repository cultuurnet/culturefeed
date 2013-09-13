<?php

/**
 * Logger class for culturefeed requests.
 */
class Culturefeed_Log_RequestLog {

  protected static $instance;

  /**
   * @var array of requests done to api.
   */
  protected $requests = array();

  /**
   * Factory method to get the log.
   */
  public static function getInstance() {

    if (!isset(self::$instance)) {
      self::$instance = new Culturefeed_Log_RequestLog();
    }

    return self::$instance;
  }

  public function addRequest($request) {
    $this->requests[] = $request;
  }

  public function getRequests() {
    return $this->requests;
  }

}
