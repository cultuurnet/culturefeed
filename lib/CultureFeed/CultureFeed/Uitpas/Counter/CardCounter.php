<?php

class CultureFeed_Uitpas_Counter_CardCounter extends CultureFeed_Uitpas_ValueObject {

  public $status;

  public $type;
  
  public $count;

  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {
    $card_counter = new CultureFeed_Uitpas_Counter_CardCounter();
    $card_counter->status = $object->xpath_str('status');
    $card_counter->type = $object->xpath_int('type');
    $card_counter->count = $object->xpath_int('count');

    return $card_counter;
  }

}