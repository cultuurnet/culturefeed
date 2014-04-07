<?php

class CultureFeedActivityConfigNews extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_NEWS;

  /**
   * Constructor to load default values.
   */
  public function __construct() {

    parent::__construct();
    $this->allowedTypes = array(
      CultureFeed_Activity::CONTENT_TYPE_EVENT,
    );

    $this->action = t('news');
    $this->viewPrefix = t('has created a news message');
    $this->label = t('News created');
  }

}

