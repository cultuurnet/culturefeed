<?php

class CultureFeed_Uitpas_Event_TicketSale extends CultureFeed_Uitpas_ValueObject {

  /**
   * The identification of the ticket sale
   *
   * @var integer
   */
  public $id;

  /**
   * Another identification of the ticket sale
   *
   * @var string
   */
  public $nodeId;


  /**
   * Another identification of the node
   *
   * @var string
   */
  public $nodeTitle;



  /**
   * The creation date of the ticket sale
   *
   * @var integer
   */
  public $creationDate;

  /**
   * The name of the balie
   *
   * @var string
   */
  public $createdVia;


  /**
   * The payed price of the ticket sale
   *
   * @var double
   */
  public $price;


  /**
   * The tariff of the ticket sale
   *
   * @var double
   */
  public $tariff;


  /**
   * The first name of the peron
   *
   * @var string
   */
  public $firstName;

  /**
   * The last name of the peron
   *
   * @var string
   */
  public $lastName;

  /**
   * The user Id of the person
   *
   * @var string
   */
  public $userId;

  /**
   * The organiser of the event
   *
   * @var string
   */
  public $organiser;

  /**
   * The home city of the user
   *
   * @var string
   */
  public $userHomeCity;

  /**
   *  Get the pass holder information based on userID
   */



  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {

    //dpm( print_r( $object , true ) );

    $ticket_sale = new CultureFeed_Uitpas_Event_TicketSale();

    $ticket_sale->id = $object->xpath_int('id');
    $ticket_sale->nodeId = $object->xpath_str('nodeId');
    $ticket_sale->nodeTitle = $object->xpath_str('nodeTitle');
    $ticket_sale->creationDate = $object->xpath_time('creationDate');
    $ticket_sale->createdVia = $object->xpath_str('createdVia');
    $ticket_sale->price = $object->xpath_float('price');
    $ticket_sale->tariff = $object->xpath_float('tariff');
    $ticket_sale->firstName = $object->xpath_str('firstName');
    $ticket_sale->lastName = $object->xpath_str('lastName');
    $ticket_sale->userId = $object->xpath_str('userId');
    $ticket_sale->userHomeCity = $object->xpath_str('userHomeCity');


    return $ticket_sale;
  }

}