<?php

class CultureFeedActivityConfigPageCreated extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_PAGE_CREATED;

  /**
   * Constructor to load default values.
   */
  public function __construct() {

    parent::__construct();
    $this->allowedTypes = array(
      CultureFeed_Activity::CONTENT_TYPE_CULTUREFEED_PAGE,
    );

    $this->action = t('page');
    $this->viewPrefix = t('has created a page');
    $this->label = t('Page created');
  }

}

