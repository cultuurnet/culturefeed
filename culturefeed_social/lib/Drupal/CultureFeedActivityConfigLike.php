<?php

class CultureFeedActivityConfigLike extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_LIKE;

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

    $this->subject = 'Vinden dit leuk';
    $this->subjectDo = 'Vind ik leuk';
    $this->subjectUndo = 'Vind ik niet meer leuk';
    $this->titleDo = 'Vind ik leuk';
    $this->titleDoFirst = 'Vind dit als eerste leuk';
    $this->titleShowAll = 'Toon iedereen die dit leuk vindt';
    $this->linkClassDo = 'like-link';
    $this->linkClassUndo = 'unlike-link';
    $this->viewPrefix = 'vindt';
    $this->viewSuffix = 'leuk';
    $this->label = 'Like';
    $this->loginRequiredMessage = 'Om te liken moet je aangemeld zijn';

  }

}
