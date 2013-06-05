<?php

class CultureFeedActivityConfigReview extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_REVIEW;

  /**
   * Constructor to load default values.
   */
  public function __construct() {
    parent::__construct();

    $this->allowedTypes = array('event', 'production');

    $this->viewPrefix = 'heeft';
    $this->viewSuffix = 'een beoordeling gegeven';

  }

}