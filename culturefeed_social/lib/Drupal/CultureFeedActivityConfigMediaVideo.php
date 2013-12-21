<?php

class CultureFeedActivityMediaVideo extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_MEDIA_VIDEO;

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

    $this->viewPrefix = t('has added a new video');;
    $this->pointsOverviewPrefix = t('Video added to');
    $this->titleDo = t('Add a video');
    $this->titleDoFirst = t('Add a video');
    $this->action = t('video');
    $this->label = t('Video added');
  }

}