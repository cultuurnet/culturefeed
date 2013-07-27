<?php

class CultureFeedActivityConfigRecommend extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_RECOMMEND;

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

    $this->subject = t('Recommended by');
    $this->subjectDo = t('Recommended activity');
    $this->subjectUndo = t('Do not recommend activity anymore');
    $this->titleDo = t('Also recommend activity');
    $this->titleDoFirst = t('Recommend this as the first');
    $this->titleShowAll = t('View everyone who recommends this');
    $this->linkClassDo = 'recommend-link';
    $this->linkClassUndo = 'unrecommend-link';
    $this->viewPrefix = t('has');
    $this->viewSuffix = t('recommended');
    $this->label = t('Recommend');
    $this->loginRequiredMessage = t('To recommend you have to be logged on');
    $this->pointsOverviewSuffix = t('recommended');
  }

}