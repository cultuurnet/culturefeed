<?php

class DrupalCultureFeed {

  protected static $consumer_instance;

  protected static $user_instance;
  
  protected static $logged_in_account;
  
  protected static $logged_in_user;
  
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
  public static function getLoggedInUser() {
    if (isset(self::$logged_in_user)) {
      return self::$logged_in_user;
    }
    
    if (user_is_anonymous()) {
      return NULL;
    }
    
    $account = self::getLoggedInAccount();
    
    if (!isset($account->culturefeed_userid)) {
      return NULL;
    }
    
    self::$logged_in_user = self::getLoggedInUserInstance()->getUser(self::$logged_in_account->culturefeed_userid, TRUE, TRUE);
    
    return self::$logged_in_user;
  }

  /**
   * @todo document this
   *
   * @return CultureFeed
   *   @todo document this
   */
  public static function getLoggedInUserInstance() {
    if (isset(self::$user_instance)) {
      return self::$user_instance;
    }
    
    $account = self::getLoggedInAccount();

    if (!isset($account->culturefeed_oauth_token) || !isset($account->culturefeed_oauth_secret)) {
      return;
    }

    self::$user_instance = self::getInstance($account->culturefeed_oauth_token, $account->culturefeed_oauth_secret);

    return self::$user_instance;
  }

  /**
   * @todo document this
   *
   * @return CultureFeed
   *   @todo document this
   */
  public static function getConsumerInstance() {
    if (isset(self::$consumer_instance)) {
      return self::$consumer_instance;
    }

    self::$consumer_instance = self::getInstance(NULL, NULL);

    return self::$consumer_instance;
  }

  /**
   * @todo document this
   *
   * @return CultureFeed
   *   @todo document this
   */
  public static function getInstance($token, $secret) {
    if (!variable_get('culturefeed_proxy_enabled', FALSE)) {
      $oauth_client = new CultureFeed_OAuthClient(variable_get('culturefeed_api_application_key', ''), variable_get('culturefeed_api_shared_secret', ''), $token, $secret);
      
    }
    else {
      $http_client = new CultureFeed_HTTPClient();
    
      $http_client->proxy_server = variable_get('culturefeed_proxy_server', '');
      $http_client->proxy_port = variable_get('culturefeed_proxy_port', '');
      $http_client->proxy_username = variable_get('culturefeed_proxy_username', '');
      $http_client->proxy_password = variable_get('culturefeed_proxy_password', '');
      
      $oauth_client = new CultureFeed_OAuthClient(variable_get('culturefeed_api_application_key', ''), variable_get('culturefeed_api_shared_secret', ''), $token, $secret);

      $oauth_client->setHttpClient($http_client);
    }

    return new CultureFeed($oauth_client);
  }
}