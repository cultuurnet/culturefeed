<?php

/**
 * Class to represent an activity.
 */
class CultureFeed_Activity {

  /**
   * Content type 'node' (drupal).
   */
  const CONTENT_TYPE_NODE = 'node';

  /**
   * Content type 'event'.
   */
  const CONTENT_TYPE_EVENT = 'event';

  /**
   * Content type 'actor'.
   */
  const CONTENT_TYPE_ACTOR = 'actor';

  /**
   * Content type 'book'.
   */
  const CONTENT_TYPE_BOOK = 'book';

  /**
   * Content type 'production'.
   */
  const CONTENT_TYPE_PRODUCTION = 'production';

  /**
   * Content type 'culturefeed page'.
   */
  const CONTENT_TYPE_CULTUREFEED_PAGE = 'culturefeed-page';

  /**
   * Consumer type that indicates the action "Viewed".
   */
  const TYPE_VIEW = 1;

  /**
   * Consumer type that indicates the action "Detail viewed".
   */
  const TYPE_DETAIL = 2;

  /**
   * Consumer type that indicates the action "I like this".
   */
  const TYPE_LIKE = 3;

  /**
   * Consumer type that indicates the action "I forwarded this via mail".
   */
  const TYPE_MAIL = 4;

  /**
   * Consumer type that indicates the action "Printed".
   */
  const TYPE_PRINT = 5;

  /**
   * Consumer type that indicates the action "Shared on Facebook".
   */
  const TYPE_FACEBOOK = 6;

  /**
   * Consumer type that indicates the action "Shared on Twitter".
   */
  const TYPE_TWITTER = 7;

  /**
   * Consumer type that indicates the action "I went to this event".
   */
  const TYPE_IK_GA = 8;

  const TYPE_TICKET = 9;

  const TYPE_ROUTE = 10;

  const TYPE_MORE_INFO = 11;

  const TYPE_UITPAS = 12;

  const TYPE_REGULAR_CHECKIN = 13;

  const TYPE_COMMENT = 14;

  const TYPE_RECOMMEND = 15;

  const TYPE_FOLLOW = 18;

  /**
   * ID of the activity object.
   *
   * @var string
   */
  public $id;

  /**
   * NodeId of the activity object.
   *
   * @var string
   */
  public $nodeId;

  /**
   * NodeTitle of the activity object.
   *
   * @var string
   */
  public $nodeTitle;

  /**
   * Privacy status of the activity.
   *
   * @var bool
   */
  public $private;

  /**
   * The service consumer id of the consumer where the activity was generated.
   *
   * @var string
   */
  public $createdVia;

  /**
   * The points for this activity.
   *
   * @var int
   */
  public $points;

  /**
   * The type of content this activity handles.
   * Possible values are represented in the CONTENT_TYPE_* constants.
   *
   * @var string
   */
  public $contentType;

  /**
   * The consumption type of this activity.
   * Possible values are represented in the TYPE_* constants.
   *
   * @var string
   */
  public $type;

  /**
   * Value for this activity.
   *
   * @var string
   */
  public $value;

  /**
   * ID of the user who generated this activity.
   *
   * @var string
   */
  public $userId;

  /**
   * Depiction of the user who generated this activity.
   *
   * @var string
   */
  public $depiction;

  /**
   * Nick of the user who generated this activity.
   *
   * @var string
   */
  public $nick;

  /**
   * Creation date of this activity represented as a UNIX timestamp.
   *
   * @var integer
   */
  public $creationDate;

  /**
   * ID of the parent activity. Only allowed for activities of type 'comment'.
   *
   * @var string
   */
  public $parentActivity;
  
  /**
   * Helper method to get a string value for an ID.
   * 
   * Requests to the /activities api will use the Integer values while requests
   * to the /search api will use the predefined names. 
   * This method maps the two with intention easy the usage.
   * 
   * @param Integer $type
   * @return String $activity type.
   */
  public static function getNameById($id) {
    
    $name = '';
    
    switch ($id) {

      case self::TYPE_RECOMMEND:
        $name = \CultuurNet\Search\ActivityStatsExtendedEntity::ACTIVITY_COUNT_RECOMMEND;
        break;
        
      case self::TYPE_LIKE:
        $name = \CultuurNet\Search\ActivityStatsExtendedEntity::ACTIVITY_COUNT_LIKE;
        break;
        
      case self::TYPE_COMMENT:
        $name = \CultuurNet\Search\ActivityStatsExtendedEntity::ACTIVITY_COUNT_COMMENT;
        break;

    }
    
    return $name;
    
  }

  /**
   * Convert a CultureFeed_Activity object to an array that can be used as data in POST requests that expect user info.
   *
   * @return array
   *   Associative array representing the object. For documentation of the structure, check the Culture Feed API documentation.
   */
  public function toPostData() {
    // For most properties we can rely on get_object_vars.
    $data = get_object_vars($this);

    // Represent private as a string (true/false);
    if (isset($data['private'])) {
      $data['private'] = $data['private'] ? 'true' : 'false';
    }

    // Represent creationDate as a W3C date.
    if (isset($data['creationDate'])) {
      $data['creationDate'] = date('c', $data['creationDate']);
    }

    $data = array_filter($data);
    
    // Unset the path which is only used internally.
    unset($data['path']);

    return $data;
  }

}

