<?php

class CultureFeedActivityConfigPageAdmin extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_PAGE_ADMIN;

  /**
   * Constructor to load default values.
   */
  public function __construct() {
    parent::__construct();

    $this->allowedTypes = array(
      CultureFeed_Activity::CONTENT_TYPE_CULTUREFEED_PAGE,
    );

    $this->viewPrefix = t('became administrator of');
    $this->label = t('Page administrator');
  }

}
