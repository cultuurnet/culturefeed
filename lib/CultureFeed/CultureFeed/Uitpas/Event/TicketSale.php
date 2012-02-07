<?php

class CultureFeed_Uitpas_Event_TicketSale extends CultureFeed_Uitpas_ValueObject {

  /**
   * The identification of the ticket sale
   *
   * @var integer
   */
  public $id;

  /**
   * The creation date of the ticket sale
   *
   * @var integer
   */
  public $creationDate;

  /**
   * The payed price of the ticket sale
   *
   * @var double
   */
  public $price;

  /**
   * The identification of the event for which the ticket has been purchased
   *
   * @var int
   */
  public $cdbid;

public static function createFromXML(CultureFeed_SimpleXMLElement $object) {
    $ticket_sale = new CultureFeed_Uitpas_Event_TicketSale();
    $ticket_sale->id = $object->xpath_int('id');
    $ticket_sale->creationDate = $object->xpath_time('creationDate');
    $ticket_sale->price = $object->xpath_float('price');
    $ticket_sale->cdbid = $object->xpath_str('cdbid');

    return $ticket_sale;
  }

}