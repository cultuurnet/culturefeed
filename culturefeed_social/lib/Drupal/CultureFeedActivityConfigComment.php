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

    $this->subject = t('Reaction by');
    $this->subjectUndo = '';
    $this->titleDo = t('Write your own comment');
    $this->titleDoFirst = t('First to comment');
    $this->titleShowAll = t('Show everyone who responded to this page');
    $this->linkClassDo = 'comment-link';
    $this->linkClassUndo = 'uncomment-link';
    $this->viewPrefix = t('has given comment on');
    $this->viewSuffix = '';
    $this->label = t('Comment');
    $this->action = t('reaction');
    $this->loginRequiredMessage = t('To comment you must be logged');
    $this->onBehalfOfMessage = t('Post comment as');
    $this->pointsOverviewPrefix = t('Written response on');

  }

}