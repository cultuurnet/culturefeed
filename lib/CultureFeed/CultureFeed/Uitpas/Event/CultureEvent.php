<?php

class CultureFeed_Uitpas_Event_CultureEvent extends CultureFeed_Uitpas_ValueObject {

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

  public $checkinConstraint;

  /**
   * The checkin constraint of the event
   *
   * @var CultureFeed_Uitpas_Event_CheckinConstraint
   */
  public $checkinConstraintReason;

  /**
   * The reason the passholder cannot buy tickets for the event
   *
   * @var CultureFeed_Uitpas_Event_CheckinConstraint
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
    
    return $event;
  }

}