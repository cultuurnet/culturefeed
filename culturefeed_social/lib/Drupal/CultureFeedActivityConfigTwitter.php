<?php

class CultureFeedActivityConfigTwitter extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_TWITTER;

  /**
   * Constructor to load default values.
   */
  public function __construct() {

    parent::__construct();
    $this->allowedTypes = array('actor', 'event', 'production', 'book', 'page');

    $this->subject = 'Gedeeld door';
    $this->viewPrefix = 'heeft';
    $this->viewSuffix = 'via Twitter gedeeld';

  }

}