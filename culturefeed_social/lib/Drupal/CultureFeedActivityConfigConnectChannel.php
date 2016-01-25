<?php

class CultureFeedActivityConfigConnectChannel extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_CONNECT_CHANNEL;

  /**
   * Constructor to load default values.
   */
  public function __construct() {

    parent::__construct();
    $this->allowedTypes = array();

    $this->action = t('channel');
    $this->viewPrefix = t('connected channel');
    $this->label = t('Channel connected');

  }

}