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

    $this->viewPrefix = 'heeft een nieuwe video toegevoegd';
    $this->pointsOverviewPrefix = "Video toegevoegd aan";
    $this->titleDo = 'Voeg een video toe';
    $this->titleDoFirst = 'Voeg een video toe';

  }

}