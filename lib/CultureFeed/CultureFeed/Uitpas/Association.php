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

  /**
   * @param CultureFeed_SimpleXMLElement $object
   *
   * @return static
   */
  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {
    $instance = new static();
    $instance->id = $object->xpath_int('id');
    $instance->name = $object->xpath_str('name');

    return $instance;
  }

}
