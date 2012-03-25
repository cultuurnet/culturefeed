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
  public $number;

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
  
  /**
   * The consumer key of the counter
   *
   * @var string
   */
  public $consumerKey;
  
  /**
   * The role of the counter
   *
   * @var string
   */
  public $role;

  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {
    $counter = new CultureFeed_Uitpas_Counter();
    $counter->id = $object->xpath_str('id');
    $counter->name = $object->xpath_str('name');
    $counter->type = $object->xpath_str('type');
    $counter->street = $object->xpath_str('street');
    $counter->number = $object->xpath_int('number');
    $counter->box = $object->xpath_str('box');
    $counter->postalCode = $object->xpath_str('postalCode');
    $counter->city = $object->xpath_str('city');
    $counter->telephoneNumber = $object->xpath_str('telephoneNumber');
    $counter->contactPerson = $object->xpath_str('contactPerson');
    $counter->consumerKey = $object->xpath_str('consumerKey');
    $counter->role = $object->xpath_str('role');

    return $counter;
  }

}