<?php

class CultureFeed_Uitpas_Counter_CardCounter extends CultureFeed_Uitpas_ValueObject {

  const STATUS_SENT_TO_BALIE = 'SENT_TO_BALIE';

  const STATUS_LOCAL_STOCK = 'LOCAL_STOCK';

  /**
   * @var string
   */
  public $status;

  /**
   * @var bool
   */
  public $kansenstatuut;

  /**
   * @var int
   */
  public $count;

  /**
   * @var CultureFeed_Uitpas_CardSystem
   */
  public $cardSystem;

  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {
    $card_counter = new CultureFeed_Uitpas_Counter_CardCounter();
    $card_counter->status = $object->xpath_str('status');
    $card_counter->kansenstatuut = $object->xpath_bool('kansenstatuut');
    $card_counter->count = $object->xpath_int('count');

    $card_counter->cardSystem = CultureFeed_Uitpas_CardSystem::createFromXML($object->xpath('cardSystem', FALSE));

    return $card_counter;
  }

}
