<?php

class CultureFeedActivityConfigPrint extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_PRINT;

  /**
   * Constructor to load default values.
   */
  public function __construct() {

    parent::__construct();
    $this->allowedTypes = array('actor', 'event', 'production', 'book', 'page');

    $this->subject = 'Geprint door';
    $this->viewPrefix = 'heeft';
    $this->viewSuffix = 'afgedrukt';

  }

}