<?php

class CultureFeedActivityConfigGo extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_IK_GA;

  /**
   * Constructor to load default values.
   */
  public function __construct() {
    parent::__construct();

    $this->allowedTypes = array('event');

    $this->viewPrefix = 'gaat naar';
    $this->label = 'Gaat naar';
    $this->loginRequiredMessage = 'U moet ingelogd zijn om aan te geven dat u gaat';

  }

}
