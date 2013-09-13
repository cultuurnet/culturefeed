<?php

/**
 * Logger class for culturefeed requests.
 */
class Culturefeed_Log_Request {

  /**
   * @var array of requests done to api.
   */
  protected $requests = array();

  /**
   * Start timestamp of the request in microseconds.
   * @var int
   */
  private $startTime;

  /**
   * Url beïng requested.
   * @var string
   */
  protected $url;

  /**
   * Response object.
   * @var CultureFeed_HttpResponse
   */
  protected $response;

  /**
   * Total time for the request in microseconds.
   * @var int
   */
  protected $time;

  public function __construct($url) {
    $this->url = $url;
    $this->startTime = microtime(TRUE);
  }

  public function getUrl() {
    return $this->url;
  }

  public function setUrl($url) {
    $this->url = $url;
  }

  public function getTime() {
    return $this->time;
  }

  public function setTime($time) {
    $this->time = $time;
  }

  public function getResponse() {
    return $this->response;
  }

  public function setResponse(Guzzle\Http\Message\Response $response) {
    $this->response = $response;
  }

  /**
   * Query has received a result, stop the query.
   */
  public function onRequestSent($response) {
    $stopTime = microtime(TRUE);
    $this->time = round(($stopTime - $this->startTime) * 1000, 2);
    $this->response = $response;
  }

}
