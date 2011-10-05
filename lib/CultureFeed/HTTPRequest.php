<?php

/**
 * Class to represent a basic HTTP request.
 */
class CultureFeed_HTTPRequest {

  /**
   * Proxy server URI.
   *
   * @var string
   */
  protected $proxy_server;

  /**
   * Proxy server port.
   *
   * @var integer
   */
  protected $proxy_port;

  /**
   * Proxy server username.
   *
   * @var string
   */
  protected $proxy_username;

  /**
   * Proxy server password.
   *
   * @var string
   */
  protected $proxy_password;

  /**
   * Connection timeout (defaults to 15 seconds).
   *
   * @var integer
   */
  protected $timeout = 15;

  /**
   * Set the proxy server URI.
   *
   * @param string $proxy_server
   */
  public function setProxyServer($proxy_server) {
    $this->proxy_server = $proxy_server;
  }

  /**
   * Set the proxy server port.
   *
   * @param integer $proxy_port
   */
  public function setProxyPort($proxy_port) {
    $this->proxy_port = $proxy_port;
  }

  /**
   * Set the proxy server username.
   *
   * @param string $proxy_username
   */
  public function setProxyUsername($proxy_username) {
    $this->proxy_username = $proxy_username;
  }

  /**
   * Set the proxy server password
   *
   * @param string $proxy_password
   */
  public function setProxyPassword($proxy_password) {
    $this->proxy_password = $proxy_password;
  }

  /**
   * Set the connection timeout.
   *
   * @param integer $timeout
   */
  public function setTimeout($timeout) {
    $this->timeout = $timeout;
  }

  /**
   * Make an HTTP request
   *
   * @param string $url
   *   The URL for the request.
   * @param array $http_headers
   *   HTTP headers to set on the request.
   *   Represented as an array of header strings.
   * @param string $method
   *   The HTTP method.
   * @param string $post_data
   *   In case of a POST request, specify the post data a string.
   * @return CultureFeed_HTTPResponse
   *   The response.
   */
  public function request($url, $http_headers = array(), $method = 'GET', $post_data = '') {
    // Initialising some general CURL options (url, timeout, ...).
    $curl_options = array(
      CURLOPT_URL => $url,
      CURLOPT_TIMEOUT => $this->timeout,
      CURLOPT_RETURNTRANSFER => TRUE,
      CURLOPT_HEADER => FALSE,
    );

    // If a proxy server is set, configure CURL to use it.
    if (!empty($this->proxy_server)) {
      $curl_options[CURLOPT_PROXY] = $this->proxy_server;
      $curl_options[CURLOPT_PROXYPORT] = $this->proxy_port;
      if ($this->proxy_username) {
        $curl_options[CURLOPT_PROXYUSERPWD] = sprintf('%s:%s', $this->proxy_username, $this->proxy_password);
      }
    }

    // Set HTTP headers.
    if (!empty($http_headers)) {
      $curl_options[CURLOPT_HTTPHEADER] = $http_headers;
    }

    // If the method is POST, configure CURL for it and set the post data.
    if ($method == 'POST') {
      $curl_options[CURLOPT_POST] = TRUE;
      $curl_options[CURLOPT_POSTFIELDS] = $post_data;
    }

    // Do the CURL request.
    $ch = curl_init();
    curl_setopt_array($ch, $curl_options);

    $response  = curl_exec($ch);
    $curl_info = curl_getinfo($ch);
    $curl_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    
    curl_close($ch);
    
    return new CultureFeed_HTTPResponse($curl_code, $response);
  }

}