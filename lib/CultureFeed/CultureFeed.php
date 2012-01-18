<?php

class CultureFeed {
  const TYPE_CORE = 'Core';
  const TYPE_UITPAS = 'UitPas';

  private static $oauth_client;

  protected static $core;
  protected static $uitpas;

  public static function getInstance($type, $token, $secret, $application_key = NULL, $shared_secret = NULL) {
    switch ($type) {
      case CultureFeed::TYPE_CORE:
        return self::createInstance(self::$core, $type);

      case CultureFeed::TYPE_UITPAS:
        return self::createInstance(self::$uitpas, $type);

      default:
        throw new InvalidArgumentException("Cannot find Culturefeed type $type!");
    }
  }

  private static function createInstance(&$instance, $type) {
    if (!isset($instance)) {
        $instance = new $type(self::$oauth_client);
      }

      return $instance;
  }

}