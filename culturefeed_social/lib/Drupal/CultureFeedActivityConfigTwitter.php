<?php

class CultureFeedActivityConfigTwitter extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_TWITTER;

  /**
   * Constructor to load default values.
   */
  public function __construct() {

    parent::__construct();
    $this->allowedTypes = array(
      CultureFeed_Activity::CONTENT_TYPE_EVENT,
      CultureFeed_Activity::CONTENT_TYPE_PRODUCTION,
      CultureFeed_Activity::CONTENT_TYPE_ACTOR,
      CultureFeed_Activity::CONTENT_TYPE_BOOK,
      CultureFeed_Activity::CONTENT_TYPE_NODE,
      CultureFeed_Activity::CONTENT_TYPE_CULTUREFEED_PAGE,
    );

    $this->undoAllowed = FALSE;
    $this->titleDo = 'Delen';
    $this->titleDoFirst = 'Delen';
    $this->subject = 'Gedeeld door';
    $this->viewPrefix = 'heeft';
    $this->viewSuffix = 'via Twitter gedeeld';
    $this->label = 'Delen via Twitter';
    $this->undoNotAllowedMessage = 'Succesvol gedeeld';

  }

}