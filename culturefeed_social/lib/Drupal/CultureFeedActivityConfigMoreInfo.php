<?php

class CultureFeedActivityConfigMoreInfo extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_PRINT;

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
      CultureFeed_Activity::CONTENT_TYPE_BOOK,
      CultureFeed_Activity::CONTENT_TYPE_NODE,
      CultureFeed_Activity::CONTENT_TYPE_CULTUREFEED_PAGE,
    );

    $this->subject = t('More info requested by');
    $this->viewPrefix = t('has requested more info on');
    $this->viewSuffix = t('');
    $this->label = t('More info requested');

  }

}