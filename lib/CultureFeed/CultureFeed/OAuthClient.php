<?php

/**
 * Interface to represent a OAuth request.
 */
interface CultureFeed_OAuthClient {

  /**
   * Do a GET request with only a consumer token.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function consumerGet($path, array $params = array(), $format = '');

  /**
   * Do a GET request with only a consumer token and return the response as XML.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function consumerGetAsXml($path, array $params = array());

  /**
   * Do a GET request with only a consumer token and return the response as JSON.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function consumerGetAsJson($path, array $params = array());

  /**
   * Do a POST request with only a consumer token.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function consumerPost($path, array $params = array(), $raw_post = TRUE, $has_file_upload = FALSE, $format = '');

  /**
   * Do a POST request with only a consumer token and return the response as XML.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function consumerPostAsXml($path, array $params = array(), $raw_post = TRUE, $has_file_upload = FALSE);

  /**
   * Do a POST request with only a consumer token and return the response as JSON.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function consumerPostAsJson($path, array $params = array(), $raw_post = TRUE, $has_file_upload = FALSE);

  /**
   * Do a GET request with a consumer token and user token.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function authenticatedGet($path, array $params = array(), $format = '');

  /**
   * Do a GET request with a consumer token and user token and return the response as XML.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function authenticatedGetAsXml($path, array $params = array());

  /**
   * Do a GET request with a consumer token and user token and return the response as JSON.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function authenticatedGetAsJson($path, array $params = array());

  /**
   * Do a POST request with a consumer token and user token.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function authenticatedPost($path, array $params = array(), $raw_post = TRUE, $has_file_upload = FALSE, $format = '');

  /**
   * Do a POST request with a consumer token and user token and return the response as XML.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function authenticatedPostAsXml($path, array $params = array(), $raw_post = TRUE, $has_file_upload = FALSE);

  /**
   * Do a POST request with a consumer token and user token and return the response as JSON.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function authenticatedPostAsJson($path, array $params = array(), $raw_post = TRUE, $has_file_upload = FALSE);

  /**
   * Do a OAuth signed request.
   *
   * @param string $path
   *   The path for the request relative to the endpoint.
   * @param string $params
   *   Post data for a POST request, query string for a GET request.
   * @param string $method
   *   HTTP method.
   * @param string $use_auth
   *   Should the request be signed with the user token and token secret or just the consumer token and secret?
   *   If $use_auth is TRUE, sign with user token, secret as well as consumer token and secret.
   *   If $use_auth is FALSE, sign only with consumer token and secret.
   * @param string $format
   *   The response format.
   *   Possible values are 'xml', 'json' and '' for default response (depending on request).
   * @param string $raw_post
   *   Should the post data (passed via $params) be passed as is ($raw_post TRUE) or should the OAuth params be added?
   * @return CultureFeed_HTTPResponse
   *   The response.
   *
   * @throws CultureFeed_Exception
   *   If an error message and code could be parsed from the response.
   * @throws CultureFeed_HTTPException
   *   If no error message and code could be parsed from the response.
   */
  public function request($path, array $params = array(), $method = 'GET', $use_auth = TRUE, $format = 'xml', $raw_post = TRUE, $has_file_upload = FALSE);

  /**
   * Getting the full URL based on a path and query.
   *
   * @param string $path
   *   The path relative to the endpoint.
   * @param array $query
   *   (optional) The query string represented as an array.
   * @return string
   *   The full URL.
   */
  public function getUrl($path, array $query = array());

}