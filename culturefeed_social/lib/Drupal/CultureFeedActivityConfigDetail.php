<?php

class CultureFeedActivityConfigDetail extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_DETAIL;

  /**
   * Constructor to load default values.
   */
  public function __construct() {

    parent::__construct();
    $this->allowedTypes = array(
      CultureFeed_Activity::CONTENT_TYPE_EVENT,
      CultureFeed_Activity::CONTENT_TYPE_PRODUCTION,
      CultureFeed_Activity::CONTENT_TYPE_BOOK,
      CultureFeed_Activity::CONTENT_TYPE_NODE,
      CultureFeed_Activity::CONTENT_TYPE_CULTUREFEED_PAGE,
    );

    $this->subject = t('Viewed by');
    $this->viewPrefix = t('has');
    $this->viewSuffix = t('seen');
    $this->label = t('Viewed');

  }

}