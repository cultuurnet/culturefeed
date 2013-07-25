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

    $this->subject = t('Like this');
    $this->subjectDo = t('Like');
    $this->subjectUndo = t('I do not like');
    $this->titleDo = t('I Like');
    $this->titleDoFirst = t('Like this the first');
    $this->titleShowAll = t('Show everyone who likes this');
    $this->linkClassDo = 'like-link';
    $this->linkClassUndo = 'unlike-link';
    $this->viewPrefix = t('likes');
    $this->viewSuffix = t('this');
    $this->label = t('Like');
    $this->loginRequiredMessage = t('To like you have to be logged in');

  }

}
