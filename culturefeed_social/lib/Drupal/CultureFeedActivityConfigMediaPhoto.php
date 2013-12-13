<?php

class CultureFeedActivityMediaPhoto extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_MEDIA_PHOTO;

  /**
   * Constructor to load default values.
   */
  public function __construct() {
    parent::__construct();

    $this->allowedTypes = array(
      CultureFeed_Activity::CONTENT_TYPE_EVENT,
      CultureFeed_Activity::CONTENT_TYPE_PRODUCTION,
      CultureFeed_Activity::CONTENT_TYPE_ACTOR,
      CultureFeed_Activity::CONTENT_TYPE_ACTIVITY,
    );

    $this->viewPrefix = t('has added a new picture');
    $this->pointsOverviewPrefix = t('Picture added to');
    $this->titleDo = t('Add a picture');
    $this->titleDoFirst = t('Add a picture');
    $this->action = t('picture');
    $this->label = t('Picture added');
  }

}