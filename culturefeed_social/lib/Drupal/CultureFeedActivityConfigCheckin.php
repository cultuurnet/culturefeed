<?php

class CultureFeedActivityConfigCheckin extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_UITPAS;

  /**
   * Constructor to load default values.
   */
  public function __construct() {

    parent::__construct();
    $this->allowedTypes = array();

    $this->action = t('checkin');
    $this->viewPrefix = t('checked in');
    $this->label = t('Check in');

  }

}