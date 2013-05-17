<?php

class CultureFeedActivityConfigNewEvent extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_NEW_EVENT;

  /**
   * Constructor to load default values.
   */
  public function __construct() {
    parent::__construct();

    $this->allowedTypes = array();

    $this->viewPrefix = 'heeft';
    $this->viewSuffix = 'aangemaakt';

  }

}