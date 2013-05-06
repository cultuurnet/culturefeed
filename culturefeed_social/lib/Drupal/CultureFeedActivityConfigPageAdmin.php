<?php

class CultureFeedActivityConfigPageAdmin extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_PAGE_ADMIN;

  /**
   * Constructor to load default values.
   */
  public function __construct() {
    parent::__construct();

    $this->allowedTypes = array('page');

    $this->viewPrefix = 'werd administrator van';
  }

}
