<?php

class CultureFeed_Uitpas_Event_CultureEvent extends CultureFeed_Uitpas_ValueObject {

  const CHECKIN_CONSTRAINT_REASON_MAXIMUM_REACHED = 'MAXIMUM_REACHED';

  const CHECKIN_CONSTRAINT_REASON_INVALID_DATE_TIME = 'INVALID_DATE_TIME';

  /**
   * The identification of the event
   *
   * @var string
   */
  public $cdbid;

  /**
   * The ID of the location of the event
   *
   * @var string
   */
  public $locationId;

  /**
   * The name of the location of the event
   *
   * @var string
   */
  public $locationName;

  /**
   * The organiserId cdbid van de inrichter
   *
   * @var string
   */
   public $organiserId;

   /**
   * The organiserId cdbid van de inrichter
   * the API has an error and it needs actorId in order to register an event
   *
   * @var string
   */
   public $actorId;


   /**
   * The distributionId id van een verdeelsleutel
   *
   * @var string
   */
   public $distributionKey;

   /**
   * The volume constraint added for registering an event
   *
   * @var integer
   */
   public $volumeConstraint;

   /**
   * date format yyyy-mm-dd added for registering an event
   *
   * @var string
   */
   public $timeConstraintFrom;

   /**
   * date format yyyy-mm-dd added for registering an event
   *
   * @var string
   */
   public $timeConstraintTo;

   /**
   * added for registering an event
   *
   * @var string
   */
   public $periodConstraintVolume;


   /**
   * added for registering an event
   *
   * @var PeriodConstraint.PeriodType DAY, WEEK, MONTH, QUARTER, YEAR
   */
   public $periodConstraintType;

   /**
   * added for registering an event
   *
   * From API:
   * True, indien periodConstraint degressief is.
   * Dit is enkel mogelijk bij periodConstraintType YEAR.
   *
   * @var boolean
   */
   public $degressive;

   /**
   * added for registering an event
   *
   * @var PeriodConstraint.PeriodType DAY, WEEK, MONTH, QUARTER, YEAR
   */
   public $checkinPeriodConstraintType;


   /**
   * The checkin constraint added for registering an event
   *
   * @var integer
   */
   public $checkinPeriodConstraintVolume;


   /**
   * The organiserName van de inrichter
   *
   * @var string
   */
   public $organiserName;

   /**
   * The city
   *
   * @var string
   */
   public $city;

  /**
   * True if a given passholder can checkin on the event
   *
   * @var boolean
   */
  public $checkinAllowed;

  /**
   * The checkin constraint of the event
   *
   * @var CultureFeed_Uitpas_Event_CheckinConstraint
   */
  public $checkinConstraint;

  /**
   * The reason the passholder cannot check in on the event
   *
   * @var string
   */
  public $checkinConstraintReason;

  /**
   * The reason the passholder cannot buy tickets for the event
   *
   * @var string
   */
  public $buyConstraintReason;

  /**
   * The price of the event
   *
   * @var float
   */
  public $price;

  /**
   * The tariff of the event for a given passholder
   *
   * @var float
   */
  public $tariff;

  /**
   * The title of the event
   *
   * @var string
   */
  public $title;

  /**
   * The calendar description of the event
   *
   * @var Calendar
   */
  public $calendar;

   /**
   * The number of points of the event
   *
   * @var numberOfPoints
   */
  public $numberOfPoints;

  /*
   * The number of months grace period for buy tickets.
   */
  public $gracePeriodMonths;

  /**
   * Modify an array of data for posting.
   */
  protected function manipulatePostData(&$data) {
    // Set the actor ID.
    $data['actorId'] = $data['organiserId'];

    // These are allowed params for registering an event.
    $allowed = array();

    $allowed[] = "cdbid";
    $allowed[] = "locationId";
    $allowed[] = "actorId";
    $allowed[] = "distributionKey";
    $allowed[] = "volumeConstraint";
    $allowed[] = "timeConstraintFrom";
    $allowed[] = "timeConstraintTo";
    $allowed[] = "periodConstraintVolume";
    $allowed[] = "periodConstraintType";
    $allowed[] = "degressive";
    $allowed[] = "checkinPeriodConstraintType";
    $allowed[] = "checkinPeriodConstraintVolume";
    $allowed[] = "price";
    $allowed[] = "numberOfPoints";
    $allowed[] = "gracePeriodMonths";
    $allowed[] = "gracePeriod";

    foreach ($data as $key => $value) {
      if (!in_array($key, $allowed)) {
        unset($data[$key]);
      }
    }
  }




  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {

    $event = new CultureFeed_Uitpas_Event_CultureEvent();
    $event->cdbid = $object->xpath_str('cdbid');
    $event->locationId = $object->xpath_str('locationId');
    $event->locationName = $object->xpath_str('locationName');
    $event->organiserId = $object->xpath_str('organiserId');
    $event->organiserName = $object->xpath_str('organiserName');
    $event->city = $object->xpath_str('city');
    $event->checkinAllowed = $object->xpath_bool('checkinAllowed');
    $event->checkinConstraint = CultureFeed_Uitpas_Event_CheckinConstraint::createFromXml($object->xpath('checkinConstraint', false));
    $event->checkinConstraintReason = $object->xpath_str('checkinConstraintReason');
    $event->buyConstraintReason = $object->xpath_str('buyConstraintReason');
    $event->price = $object->xpath_float('price');
    $event->tariff = $object->xpath_float('tariff');
    $event->title = $object->xpath_str('title');
    $event->calendar = CultureFeed_Uitpas_Calendar::createFromXML($object->xpath('cdb:calendar', false));
    $event->numberOfPoints = $object->xpath_int('numberOfPoints');
    $event->gracePeriodMonths = $object->xpath_int('gracePeriodMonths');

    return $event;
  }

}
