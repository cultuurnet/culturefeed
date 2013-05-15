<?php

class CultureFeedActivityConfigLike extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_LIKE;

  /**
   * Constructor to load default values.
   */
  public function __construct() {
    parent::__construct();

    $this->allowedTypes = array('actor', 'event', 'production', 'book');

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
    $this->loginRequiredMessage = 'U moet ingelogd zijn om te liken';

  }

}
