<?php

class CultureFeedActivityConfigFacebook extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_FACEBOOK;

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

    $this->titleDo = t('Share');
    $this->titleDoFirst = t('Share');
    $this->subject = t('Shared by');
    $this->viewPrefix = t('has');
    $this->viewSuffix = t('shared on Facebook');
    $this->label = t('Share on Facebook');
    $this->undoNotAllowedMessage = t('Successfully shared');
    $this->action = t('share');
    $this->pointsOverviewSuffix = t('shared on Facebook');

  }

}