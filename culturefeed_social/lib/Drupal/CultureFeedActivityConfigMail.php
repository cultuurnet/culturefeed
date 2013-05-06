<?php

class CultureFeedActivityConfigMail extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_MAIL;

  /**
   * Constructor to load default values.
   */
  public function __construct() {
    parent::__construct();

    $this->allowedTypes = array('actor', 'event', 'production', 'book', 'page');

    $this->viewPrefix = 'heeft';
    $this->viewSuffix = 'gemaild';
  }

}
