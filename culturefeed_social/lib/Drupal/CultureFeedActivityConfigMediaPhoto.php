<?php

class CultureFeedActivityMediaPhoto extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_MEDIA_PHOTO;

  /**
   * Constructor to load default values.
   */
  public function __construct() {
    parent::__construct();

    $this->allowedTypes = array('event', 'production');

    $this->viewPrefix = 'heeft een nieuwe foto toegevoegd';

  }

}