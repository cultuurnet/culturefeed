<?php

class CultureFeedActivityConfigComment extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_COMMENT;

  /**
   * Constructor to load default values.
   */
  public function __construct() {
    parent::__construct();

    $this->allowedTypes = array(
      CultureFeed_Activity::CONTENT_TYPE_EVENT,
      CultureFeed_Activity::CONTENT_TYPE_PRODUCTION,
      CultureFeed_Activity::CONTENT_TYPE_BOOK,
      CultureFeed_Activity::CONTENT_TYPE_ACTIVITY,
      CultureFeed_Activity::CONTENT_TYPE_NODE,
      CultureFeed_Activity::CONTENT_TYPE_CULTUREFEED_PAGE,
    );

    $this->subject = 'Reactie door';
    $this->subjectUndo = '';
    $this->titleDo = 'Schrijf zelf een reactie';
    $this->titleDoFirst = 'Reageer als eerste';
    $this->titleShowAll = 'Toon iedereen die reageerde op deze pagina';
    $this->linkClassDo = 'comment-link';
    $this->linkClassUndo = 'uncomment-link';
    $this->viewPrefix = 'heeft op';
    $this->viewSuffix = 'commentaar gegeven';
    $this->label = 'Commentaar';
    $this->action = 'reactie';
    $this->loginRequiredMessage = 'Om commentaar te geven moet je aangemeld zijn';
    $this->onBehalfOfMessage = 'Plaats commentaar als';
    $this->pointsOverviewPrefix = 'Reactie geschreven op';

  }

}