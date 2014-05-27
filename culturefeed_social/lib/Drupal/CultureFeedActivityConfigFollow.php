<?php

class CultureFeedActivityConfigFollow extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_FOLLOW;

  /**
   * Constructor to load default values.
   */
  public function __construct() {
    parent::__construct();

    $this->allowedTypes = array(
      CultureFeed_Activity::CONTENT_TYPE_CULTUREFEED_PAGE,
    );

    $this->subject = t('Follow this page');
    $this->subjectUndo = t('Unfollow');
    $this->titleDo = t('I follow this page');
    $this->titleDoFirst = t('Be the first to follow this page');
    $this->titleShowAll = t('Show all users who follow this page');
    $this->linkClassDo = 'follow-link';
    $this->linkClassUndo = 'unfollow-link';
    $this->viewPrefix = t('follows');
    $this->label = t('Follow');
    $this->loginRequiredMessage = t('You must be !sign_in_link to follow a page', array(
              '!sign_in_link' => drupal_render($this->loginMessageLink),
            ));

  }

}
