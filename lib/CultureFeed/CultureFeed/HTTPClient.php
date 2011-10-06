<?php

/**
 * Interface to represent a basic HTTP request.
 */
interface CultureFeed_HTTPClient {

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
  public function request($url, $http_headers = array(), $method = 'GET', $post_data = '');

}