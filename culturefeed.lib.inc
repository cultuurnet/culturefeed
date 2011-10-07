<?php

class DrupalCultureFeed {

  protected static $consumer_instance;

  protected static $user_instance;

  protected static $logged_in_account;

  protected static $logged_in_user;
  
  public static function isCultureFeedUser($uid = NULL) {
    if (!$uid) {
      $account = self::getLoggedInAccount();
    }
    else {
      $account = user_load($uid);
    }

    return isset($account->culturefeed_userid) && !empty($account->culturefeed_userid);
  }

  public static function getLoggedInAccount() {
    if (isset(self::$logged_in_account)) {
      return self::$logged_in_account;
    }

    if (user_is_anonymous()) {
      return NULL;
    }

    global $user;

    self::$logged_in_account = user_load($user->uid);

    return self::$logged_in_account;
  }

  /**
   * @todo document this
   *
   * @return CultureFeed_User
   *   @todo document this
   */
  public static function getLoggedInUser($application_key = NULL, $shared_secret = NULL) {
    if (!$application_key) {
      $application_key = variable_get('culturefeed_api_application_key', '');
      $shared_secret = variable_get('culturefeed_api_shared_secret', '');
    }
    
    if (isset(self::$logged_in_user[$application_key])) {
      return self::$logged_in_user[$application_key];
    }

    if (!self::isCultureFeedUser()) {
      return NULL;
    }

    $account = self::getLoggedInAccount();

    self::$logged_in_user[$application_key] = self::getLoggedInUserInstance($application_key, $shared_secret)->getUser($account->culturefeed_userid, TRUE, TRUE);

    return self::$logged_in_user[$application_key];
  }

  /**
   * @todo document this
   *
   * @return CultureFeed
   *   @todo document this
   */
  public static function getLoggedInUserInstance($application_key = NULL, $shared_secret = NULL) {
    if (!$application_key) {
      $application_key = variable_get('culturefeed_api_application_key', '');
      $shared_secret = variable_get('culturefeed_api_shared_secret', '');
    }
    
    if (isset(self::$user_instance[$application_key])) {
      return self::$user_instance[$application_key];
    }

    $account = self::getLoggedInAccount();

    if (!isset($account->tokens[$application_key]->culturefeed_oauth_token) || !isset($account->tokens[$application_key]->culturefeed_oauth_secret)) {
      return;
    }

    self::$user_instance[$application_key] = self::getInstance($account->tokens[$application_key]->culturefeed_oauth_token, $account->tokens[$application_key]->culturefeed_oauth_secret);

    return self::$user_instance[$application_key];
  }

  /**
   * @todo document this
   *
   * @return CultureFeed
   *   @todo document this
   */
  public static function getConsumerInstance($application_key = NULL, $shared_secret = NULL) {
    if (!$application_key) {
      $application_key = variable_get('culturefeed_api_application_key', '');
      $shared_secret = variable_get('culturefeed_api_shared_secret', '');
    }
    
    if (isset(self::$consumer_instance[$application_key])) {
      return self::$consumer_instance[$application_key];
    }

    self::$consumer_instance[$application_key] = self::getInstance(NULL, NULL, $application_key, $shared_secret);

    return self::$consumer_instance[$application_key];
  }

  /**
   * @todo document this
   *
   * @return CultureFeed
   *   @todo document this
   */
  public static function getInstance($token, $secret, $application_key = NULL, $shared_secret = NULL) {
    if (!$application_key) {
      $application_key = variable_get('culturefeed_api_application_key', '');
      $shared_secret = variable_get('culturefeed_api_shared_secret', '');
    }
    
    if (!variable_get('culturefeed_proxy_enabled', FALSE)) {
      $oauth_client = new CultureFeed_DefaultOAuthClient($application_key, $shared_secret, $token, $secret);

    }
    else {
      $http_client = new CultureFeed_DefaultHttpClient();

      $http_client->proxy_server = variable_get('culturefeed_proxy_server', '');
      $http_client->proxy_port = variable_get('culturefeed_proxy_port', '');
      $http_client->proxy_username = variable_get('culturefeed_proxy_username', '');
      $http_client->proxy_password = variable_get('culturefeed_proxy_password', '');

      $oauth_client = new CultureFeed_DefaultOAuthClient($application_key, $shared_secret, $token, $secret);

      $oauth_client->setHttpClient($http_client);
    }

    $oauth_client->setEndpoint(variable_get('culturefeed_api_location', CULTUREFEED_API_LOCATION));
    
    return new CultureFeed($oauth_client);
  }
}