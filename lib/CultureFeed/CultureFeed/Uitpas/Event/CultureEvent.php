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
  public $checkinContraint;

  /**
   * The reason a passholder cannot checkin
   *
   * @var ErrorCode
   */
  public $constraintReason;

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
   * @var float
   */
  public $title;

  /**
   * The calendar description of the event
   *
   * @var Calendar
   */
  public $calendar;

  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {
    $ticket_sale = new CultureFeed_Uitpas_Event_TicketSale();
    $ticket_sale->id = $object->xpath_int('id');
    $ticket_sale->creationDate = $object->xpath_time('creationDate');
    $ticket_sale->price = $object->xpath_float('price');
    $ticket_sale->cdbid = $object->xpath_str('cdbid');

    return $ticket_sale;
  }

}