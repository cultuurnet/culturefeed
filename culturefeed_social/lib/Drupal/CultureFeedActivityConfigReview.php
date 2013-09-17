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

    $this->viewPrefix = t('has');
    $this->viewSuffix = t('wrote a review');
    $this->action = t('review');
    $this->label = t('Reviews');
    $this->titleDo = t('Write a review');
    $this->titleDoFirst = t('Be the first to write a review');
    $this->pointsOverviewPrefix = t('Wrote a review for');

  }

}