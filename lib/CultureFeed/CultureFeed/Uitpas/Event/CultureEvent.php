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

  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {
    $event = new CultureFeed_Uitpas_Event_CultureEvent();
    $event->cdbid = $object->xpath_str('cdbid');
    $event->locationId = $object->xpath_str('locationId');
    $event->locationName = $object->xpath_str('locationName');
    $event->checkinAllowed = $object->xpath_bool('checkinAllowed');
    $event->checkinConstraintReason = $object->xpath_str('checkinConstraintReason');
    $event->buyConstraintReason = $object->xpath_str('buyConstraintReason');
    $event->price = $object->xpath_float('price');
    $event->tariff = $object->xpath_float('tariff');
    $event->title = $object->xpath_str('title');
    $event->calendar = CultureFeed_Uitpas_Calendar::createFromXML($object->xpath('ns6:calendar'));

    return $event;
  }

}