<?php

class CultureFeedActivityConfigCashin extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_CASHIN;

  /**
   * Constructor to load default values.
   */
  public function __construct() {

    parent::__construct();
    $this->allowedTypes = array();

    $this->action = t('cashin');
    $this->viewPrefix = t('cashed in');
    $this->label = t('Cashed in');

  }

}