<?php

class CultureFeedActivityConfigGo extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_IK_GA;

  /**
   * Constructor to load default values.
   */
  public function __construct() {
    parent::__construct();

    $this->allowedTypes = array(
      CultureFeed_Activity::CONTENT_TYPE_EVENT,
      CultureFeed_Activity::CONTENT_TYPE_PRODUCTION,
    );

    $this->titleDo = t('Attend');
    $this->titleDoFirst = t('Attend');
    $this->subjectUndo = t('I do not go');
    $this->viewPrefix = t('goes to');
    $this->label = t('Goes to');
    $this->loginRequiredMessage = t('You must be logged in to indicate that you attend');
    $this->pointsOverviewPrefix = t('Goes to');

  }

}
