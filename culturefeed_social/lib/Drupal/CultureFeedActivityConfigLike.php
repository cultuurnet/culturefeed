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
    $this->subjectUndo = t('Unlike');
    $this->titleDo = t('I Like');
    $this->titleDoFirst = t('Be the first to like this');
    $this->titleShowAll = t('Show all users who likes this');
    $this->linkClassDo = 'like-link';
    $this->linkClassUndo = 'unlike-link';
    $this->viewPrefix = t('likes');
    $this->viewSuffix = t('this');
    $this->label = t('Like');
    $this->loginRequiredMessage = t('You must be !sign_in_link to like this item', array(
              '!sign_in_link' => drupal_render($this->loginMessageLink),
            ));
    $this->pointsOverviewSuffix = t('liked');

  }

}
