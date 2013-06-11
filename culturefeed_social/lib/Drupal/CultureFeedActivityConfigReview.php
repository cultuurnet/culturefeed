<?php

class CultureFeedActivityConfigReview extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_REVIEW;

  /**
   * Constructor to load default values.
   */
  public function __construct() {
    parent::__construct();

    $this->allowedTypes = array(
      CultureFeed_Activity::CONTENT_TYPE_EVENT,
      CultureFeed_Activity::CONTENT_TYPE_PRODUCTION,
    );

    $this->viewPrefix = 'heeft';
    $this->viewSuffix = 'een beoordeling gegeven';
    $this->action = 'beoordeling';
    $this->label = 'Beoordeling';

  }

}