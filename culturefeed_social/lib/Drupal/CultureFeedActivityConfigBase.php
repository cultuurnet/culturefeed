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
  public $loginRequiredMessage = '';

  protected static $configs = array();

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

    // Don't construct + drupal alter every time this type is requested.
    if (isset(self::$configs[$type])) {
      return self::$configs[$type];
    }

    $config = '';
    switch ($type) {

      case CultureFeed_Activity::TYPE_COMMENT:
        $config = new CultureFeedActivityConfigComment();
      break;

      case CultureFeed_Activity::TYPE_DETAIL:
        $config = new CultureFeedActivityConfigDetail();
      break;

      case CultureFeed_Activity::TYPE_FACEBOOK:
        $config = new CultureFeedActivityConfigFacebook();
      break;

      case CultureFeed_Activity::TYPE_FOLLOW:
        $config = new CultureFeedActivityConfigFollow();
      break;

      case CultureFeed_Activity::TYPE_IK_GA:
        $config = new CultureFeedActivityConfigGo();
      break;

      case CultureFeed_Activity::TYPE_LIKE:
        $config = new CultureFeedActivityConfigLike();
      break;

      case CultureFeed_Activity::TYPE_MAIL:
        $config = new CultureFeedActivityConfigMail();
      break;

      case CultureFeed_Activity::TYPE_PAGE_ADMIN:
        $config = new CultureFeedActivityConfigPageAdmin();
      break;

      case CultureFeed_Activity::TYPE_PAGE_MEMBER:
        $config = new CultureFeedActivityConfigPageMember();
      break;

      case CultureFeed_Activity::TYPE_PRINT:
        $config = new CultureFeedActivityConfigPrint();
      break;

      case CultureFeed_Activity::TYPE_RECOMMEND:
        $config = new CultureFeedActivityConfigRecommend();
      break;

      case CultureFeed_Activity::TYPE_TWITTER:
        $config = new CultureFeedActivityConfigTwitter();
      break;

      case CultureFeed_Activity::TYPE_VIEW:
        $config = new CultureFeedActivityConfigView();
      break;

    }

    drupal_alter('culturefeed_social_config_object', $config);

    self::$configs[$type] = $config;

    return $config;

  }

}
