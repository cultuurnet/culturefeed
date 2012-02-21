<?php

class CultureFeed_Uitpas_Counter extends CultureFeed_Uitpas_ValueObject {

  /**
   * The ID of the counter
   *
   * @var float
   */
  public $id;

  /**
   * The name of the counter
   *
   * @var string
   */
  public $name;

  /**
   * The type of the counter.
   *
   * @var string
   */
  public $type;

  /**
   * The street of the counter
   *
   * @var string
   */
  public $street;

  /**
   * The street number of the counter
   *
   * @var integer
   */
  public $numberABV;

  /**
   * The post box of the counter.
   *
   * @var string
   */
  public $box;

  /**
   * The postal code of the counter.
   *
   * @var string
   */
  public $postalCode;

  /**
   * The city of the counter.
   *
   * @var string
   */
  public $city;

  /**
   * The telephone number of the counter.
   *
   * @var string
   */
  public $telephoneNumber;

  /**
   * The gender of the counter.
   *
   * @var string
   */
  public $contactPerson;

  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {
    $counter = new CultureFeed_Uitpas_Counter();
    $counter->id = $object->xpath_float('id');
    $counter->name = $object->xpath_str('name');
    $counter->type = $object->xpath_str('type');
    $counter->street = $object->xpath_str('street');
    $counter->numberABV = $object->xpath_int('numberABV');
    $counter->box = $object->xpath_str('box');
    $counter->postalCode = $object->xpath_str('postalCode');
    $counter->city = $object->xpath_str('city');
    $counter->telephoneNumber = $object->xpath_str('telephoneNumber');
    $counter->contactPerson = $object->xpath_str('contactPerson');

    return $counter;
  }

}