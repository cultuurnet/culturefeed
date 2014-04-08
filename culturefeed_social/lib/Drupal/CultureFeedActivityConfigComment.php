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

    $this->subject = t('Reply by');
    $this->subjectUndo = t('Undo');
    $this->titleDo = t('Post a comment');
    $this->titleDoFirst = t('Be the first to post a comment');
    $this->titleShowAll = t('Show all users who posted a comment');
    $this->linkClassDo = 'comment-link';
    $this->linkClassUndo = 'uncomment-link';
    $this->viewPrefix = t('has replied to');
    $this->viewSuffix = '';
    $this->label = t('Comment');
    $this->action = t('comment');
    $this->loginRequiredMessage = t('You must be !sign_in_link to post a comment', array(
              '!sign_in_link' => drupal_render($this->loginMessageLink),
            ));
    $this->onBehalfOfMessage = t('Reply as');
    $this->pointsOverviewPrefix = t('Posted a comment on');

  }

}