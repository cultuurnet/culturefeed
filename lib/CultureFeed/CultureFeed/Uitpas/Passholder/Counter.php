<?php

class CultureFeed_Uitpas_Passholder_Counter extends CultureFeed_Uitpas_ValueObject {

  /**
   * The ID of the counter
   *
   * @var integer
   */
  public $id;

  /**
   * The name of the counter
   *
   * @var string
   */
  public $name;

  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {
    $counter = new CultureFeed_Uitpas_Passholder_Counter();
    $counter->id = $object->xpath_int('id');
    $counter->name = $object->xpath_str('name');

    return $counter;
  }

}