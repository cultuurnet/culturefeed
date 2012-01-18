<?php

class CultureFeed {
  const TYPE_CORE = 'Core';
  const TYPE_UITPAS = 'UitPas';

  private $oauth_client;

  protected $core;
  protected $uitpas;

  public static function getInstance($type, $token, $secret, $application_key = NULL, $shared_secret = NULL) {
    switch ($type) {
      case CultureFeed::TYPE_CORE:
        return self::createInstance($this->core, $type);

      case CultureFeed::TYPE_UITPAS:
        return self::createInstance($this->uitpas, $type);
    }
  }

  private function createInstance(&$instance, $type) {
    if (!isset($instance)) {
        $instance = new $type($this->oauth_client);
      }

      return $instance;
  }

}