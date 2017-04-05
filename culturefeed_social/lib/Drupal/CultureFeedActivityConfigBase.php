<?php

/**
 * Generic configuration object for all type of activity.
 */
class CultureFeedActivityConfigBase {

  public $nodeTypes;
  public $undoAllowed = TRUE;
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
  public $action = '';
  public $onBehalfOfMessage = 'Post comment as';
  public $undoNotAllowedMessage = '';
  public $pointsOverviewPrefix = '';
  public $pointsOverviewSuffix = '';

  protected $loginMessageLink = '';
  protected static $configs = array();

  /**
   * Constructor to load default values.
   */
  public function __construct() {
    $this->nodeTypes = array_keys(node_type_get_types());
    $this->allowedTypes = array('event');

    $this->loginMessageLink = array(
      '#theme' => 'link',
      '#path' => 'culturefeed/oauth/connect',
      '#text' => t("logged in"),
      '#options' => array(
        'html' => TRUE,
        'attributes' => array(
          'class' => array(
            'culturefeedconnect'
          ),
          'rel' => 'nofollow'
        ),
        'query' => array(
          'destination' => current_path(),
        ),
      ),
    );
  }

  /**
   * Factory method to retrieve the correct CultureFeedActivityConfig object.
   */
  public static function loadByType($type) {

    // Don't construct + drupal alter every time this type is requested.
    if (isset(self::$configs[$type])) {
      return self::$configs[$type];
    }

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

      case CultureFeed_Activity::TYPE_NEW_EVENT:
        $config = new CultureFeedActivityConfigNewEvent();
      break;

      case CultureFeed_Activity::TYPE_REVIEW:
        $config = new CultureFeedActivityConfigReview();
      break;

      case CultureFeed_Activity::TYPE_MEDIA_PHOTO:
        $config = new CultureFeedActivityMediaPhoto();
      break;

      case CultureFeed_Activity::TYPE_MEDIA_VIDEO:
        $config = new CultureFeedActivityMediaVideo();
      break;

      case CultureFeed_Activity::TYPE_PAGE_CREATED:
        $config = new CultureFeedActivityConfigPageCreated();
        break;

      case CultureFeed_Activity::TYPE_NEWS:
        $config = new CultureFeedActivityConfigNews();
        break;

      case CultureFeed_Activity::TYPE_MORE_INFO:
        $config = new CultureFeedActivityConfigMoreInfo();
        break;

      case CultureFeed_Activity::TYPE_ROUTE:
        $config = new CultureFeedActivityConfigRoute();
        break;
        
      case CultureFeed_Activity::TYPE_CONNECT_CHANNEL:
        $config = new CultureFeedActivityConfigConnectChannel();
        break;
        
      case CultureFeed_Activity::TYPE_CASHIN:
        $config = new CultureFeedActivityConfigCashin();
        break;
        
      case CultureFeed_Activity::TYPE_UITPAS:
        $config = new CultureFeedActivityConfigCheckin();
        break;

      default:
        return NULL;


    }

    drupal_alter('culturefeed_social_config_object', $config);

    self::$configs[$type] = $config;

    return $config;

  }

}
