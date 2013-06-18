<?php

class CultureFeedActivityConfigRecommend extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_RECOMMEND;

  /**
   * Constructor to load default values.
   */
  public function __construct() {
    parent::__construct();

    $this->allowedTypes = array(
      CultureFeed_Activity::CONTENT_TYPE_EVENT,
      CultureFeed_Activity::CONTENT_TYPE_PRODUCTION,
      CultureFeed_Activity::CONTENT_TYPE_ACTOR,
      CultureFeed_Activity::CONTENT_TYPE_ACTIVITY,
      CultureFeed_Activity::CONTENT_TYPE_BOOK,
      CultureFeed_Activity::CONTENT_TYPE_NODE,
      CultureFeed_Activity::CONTENT_TYPE_CULTUREFEED_PAGE,
    );

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
    $this->pointsOverviewSuffix = 'aangeraden';
  }

}