<?php

class CultureFeedActivityConfigRecommend extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_RECOMMEND;

  /**
   * Constructor to load default values.
   */
  public function __construct() {
    parent::__construct();

    $this->allowedTypes = array('event', 'production', 'actor', 'book');

    $this->subject = 'Aangeraden door';
    $this->subjectDo = 'Activiteit aanraden';
    $this->subjectUndo = 'Activiteit niet meer aanraden';
    $this->titleDo = 'Activiteit ook aanraden';
    $this->titleDoFirst = 'Raad dit als eerste aan';
    $this->titleShowAll = 'Toon iedereen die dit aanraadt';
    $this->linkClassDo = 'recommend-link';
    $this->linkClassUndo = 'unrecommend-link';
    $this->viewPrefix = 'heeft';
    $this->viewSuffix = 'aangeraden';
    $this->label = 'Aanraden';
    $this->loginRequiredMessage = 'Om aan te raden moet u aangemeld zijn';
  }

}