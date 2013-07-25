<?php

class CultureFeedActivityConfigPageMember extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_PAGE_MEMBER;

  /**
   * Constructor to load default values.
   */
  public function __construct() {
    parent::__construct();

    $this->allowedTypes = array(
      CultureFeed_Activity::CONTENT_TYPE_CULTUREFEED_PAGE,
    );

    $this->viewPrefix = t('became member of');
    $this->label = t('Membership');

  }

}
