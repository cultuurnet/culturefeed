<?php

class CultureFeed_Uitpas_Event_CheckinActivity extends CultureFeed_Uitpas_ValueObject {

  /**
   * The CultureFeed activity ID
   *
   * @var string
   */
  public $id;

  /**
   * The CDBID of the event
   *
   * @var string
   */
  public $nodeId;

  /**
   * The title of the node
   *
   * @var string
   */
  public $nodeTitle;

  /**
   * Privacy status
   *
   * @var bool
   */
  public $private;

  /**
   * The ServiceConsumer ID of the activity object
   *
   * @var string
   */
  public $createdVia;

  /**
   * The number of points
   *
   * @var integer
   */
  public $points;

  /**
   * The type of content (event, actor or page)
   *
   * @var string
   */
  public $contentType;

  /**
   * The consumption type of the activity object. CHECKIN = 12
   *
   * @var integer
   */
  public $type;

  /**
   * The value of the activity
   *
   * @var string
   */
  public $value;

  /**
   * The user ID of the author of the activity
   *
   * @var string
   */
  public $userId;

  /**
   * The depiction URL of the author of the activity
   *
   * @var string
   */
  public $depiction;

  /**
   * The nick of the author of the activity
   *
   * @var string
   */
  public $nick;

  /**
   * The creation date of the activity object
   *
   * @var integer
   */
  public $creationDate;

  /**
   * True if the author of the checkin has a kansenstatuut
   *
   * @var boolean
   */
  public $kansenStatuut;

  /**
   * The location of the checkin
   *
   * @var string
   */
  public $location;

  /**
   * The name of the organizer of the event
   *
   * @var string
   */
  public $organizer;

  /**
   * The location of the participant that's checking in to an event
   *
   * @var string
   */
  public $userHomeCity;

  /**
   * Latitude of the home location of the passholder who checked in.
   *
   * @var float
   */
  public $userHomeLocationLat;

  /**
   * Longitude of the home location of the passholder who checked in.
   *
   * @var float
   */
  public $userHomeLocationLon;

  /**
   * The first name of the passholder who checked in.
   * @var string
   */
  public $firstName;

  /**
   * The last name of the passholder who checked in.
   * @var string
   */
  public $lastName;

  /**
   * Total amount of points of the passholder who checked in.
   * @var integer
   */
  public $userPoints;
  
  /**
   * Latitude of the location where checked in.
   *
   * @var float
   */
  public $eventLocationLat;

  /**
   * Longitude of the location where checked in.
   *
   * @var float
   */
  public $eventLocationLon;
  
  /**
   * Gender of the passholder who checked in.
   * @var string
   */
  public $gender;  

  /**
   * Instantiates a new checkin activity object based on the passed XML representation.
   *
   * @param CultureFeed_SimpleXMLElement $object
   * @return CultureFeed_Uitpas_Event_CheckinActivity
   */
  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {
    $checkin = new CultureFeed_Uitpas_Event_CheckinActivity();
    $checkin->id = $object->xpath_str('id');
    $checkin->nodeId = $object->xpath_str('nodeID');
    $checkin->nodeTitle = $object->xpath_str('nodeTitle');
    $checkin->private = $object->xpath_bool('private');
    $checkin->createdVia = $object->xpath_str('createdVia');
    $checkin->points = $object->xpath_int('points');
    $checkin->contentType = $object->xpath_str('contentType');
    $checkin->type = $object->xpath_str('type');
    $checkin->value = $object->xpath_str('value');
    $checkin->userId = $object->xpath_str('userId');
    $checkin->depiction = $object->xpath_str('depiction');
    $checkin->nick = $object->xpath_str('nick');
    $checkin->creationDate = $object->xpath_time('creationDate');
    $checkin->kansenStatuut = $object->xpath_bool('kansenStatuut');
    $checkin->location = $object->xpath_str('location');
    $checkin->organiser = $object->xpath_str('organiser');
    $checkin->userHomeCity = $object->xpath_str('userHomeCity');
    $checkin->userHomeLocationLat = $object->xpath_float('userHomeLocationLat');
    $checkin->userHomeLocationLon = $object->xpath_float('userHomeLocationLon');
    $checkin->firstName = $object->xpath_str('firstName');
    $checkin->lastName = $object->xpath_str('lastName');
    $checkin->userPoints = $object->xpath_int('userPoints');
    $checkin->eventLocationLat = $object->xpath_float('eventLocationLat');
    $checkin->eventLocationLon = $object->xpath_float('eventLocationLon');
    $checkin->gender = $object->xpath_str('gender');

    return $checkin;
  }

}