<?php

class CultureFeed_Uitpas_Association {

  /**
   * ID of the association.
   *
   * @var int
   */
  public $id;

  /**
   * Name of the association.
   *
   * @var string
   */
  public $name;

  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {
    $distribution_key = new CultureFeed_Uitpas_Association();
    $distribution_key->id = $object->xpath_int('id');
    $distribution_key->name = $object->xpath_str('name');

    return $distribution_key;
  }

}