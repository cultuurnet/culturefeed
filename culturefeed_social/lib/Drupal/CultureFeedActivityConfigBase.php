<?php

/**
 * Generic configuration object for all type of activity.
 */
class CultureFeedActivityConfigBase {

  public $nodeTypes;
  public $allowedTypes;
  public $type;
  public $subject = '';
  public $subjectDo = '';
  public $subjectUndo = '';
  public $titleDo = '';
  public $titleDoFirst = '';
  public $titleShowAll = '';
  public $linkClassDo = '';
  public $linkClassUndo = '';
  public $viewPrefix = '';
  public $viewSuffix = '';
  public $label = '';

  /**
   * Constructor to load default values.
   */
  public function __construct() {
    $this->nodeTypes = array_keys(node_type_get_types());
    $this->allowedTypes = array('event');
  }

  /**
   * Factory method to retrieve the correct CultureFeedActivityConfig object.
   */
  public static function loadByType($type) {

    switch ($type) {

      case CultureFeed_Activity::TYPE_COMMENT:
        return new CultureFeedActivityConfigComment();

      case CultureFeed_Activity::TYPE_DETAIL:
        return new CultureFeedActivityConfigDetail();

      case CultureFeed_Activity::TYPE_FACEBOOK:
        return new CultureFeedActivityConfigFacebook();

      case CultureFeed_Activity::TYPE_FOLLOW:
        return new CultureFeedActivityConfigFollow();

      case CultureFeed_Activity::TYPE_IK_GA:
        return new CultureFeedActivityConfigGo();

      case CultureFeed_Activity::TYPE_LIKE:
        return new CultureFeedActivityConfigLike();

      case CultureFeed_Activity::TYPE_MAIL:
        return new CultureFeedActivityConfigMail();

      case CultureFeed_Activity::TYPE_PAGE_ADMIN:
        return new CultureFeedActivityConfigPageAdmin();

      case CultureFeed_Activity::TYPE_PAGE_MEMBER:
        return new CultureFeedActivityConfigPageMember();

      case CultureFeed_Activity::TYPE_PRINT:
        return new CultureFeedActivityConfigPrint();

      case CultureFeed_Activity::TYPE_RECOMMEND:
        return new CultureFeedActivityConfigRecommend();

      case CultureFeed_Activity::TYPE_TWITTER:
        return new CultureFeedActivityConfigTwitter();

      case CultureFeed_Activity::TYPE_VIEW:
        return new CultureFeedActivityConfigView();

    }

  }

}
