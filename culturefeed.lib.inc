<?php

class DrupalCultureFeed extends CultureFeed {

  protected static $consumer_instance;

  protected static $user_instance;

  public static function getLoggedInUserInstance() {
    if (isset(self::$user_instance)) {
      return self::$user_instance;
    }

    global $user;

    if (!isset($user->culturefeed_oauth_token) || !isset($user->culturefeed_oauth_secret)) {
      return;
    }

    self::$user_instance = self::getInstance($user->culturefeed_oauth_token, $user->culturefeed_oauth_secret);

    return self::$user_instance;
  }

  public static function getConsumerInstance() {
    if (isset(self::$consumer_instance)) {
      return self::$consumer_instance;
    }

    self::$consumer_instance = self::getInstance(NULL, NULL);

    return self::$consumer_instance;
  }

  public static function getInstance($token, $secret) {
    $result = new DrupalCultureFeed(variable_get('culturefeed_api_application_key', ''), variable_get('culturefeed_api_shared_secret', ''), $token, $secret);

    if (variable_get('culturefeed_proxy_enabled', FALSE)) {
      $result->proxy_server = variable_get('culturefeed_proxy_server', '');
      $result->proxy_port = variable_get('culturefeed_proxy_port', '');
      $result->proxy_username = variable_get('culturefeed_proxy_username', '');
      $result->proxy_password = variable_get('culturefeed_proxy_password', '');
    }

    return $result;
  }
}