<?php

class CultureFeedActivityConfigNewEvent extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_NEW_EVENT;

  /**
   * Constructor to load default values.
   */
  public function __construct() {
    parent::__construct();

    $this->allowedTypes = array();

    $this->action = t('new event');
    $this->viewPrefix = t('has');
    $this->viewSuffix = t('added');
    $this->label = t('Event created');

  }

}