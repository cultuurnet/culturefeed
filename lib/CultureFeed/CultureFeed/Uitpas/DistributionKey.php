<?php

class CultureFeed_Uitpas_DistributionKey {

  /**
   * ID of the distribution key object.
   *
   * @var int
   */
  public $id;

  /**
   * Name of the distribution object.
   *
   * @var string
   */
  public $name;

  public static function create(CultureFeed_SimpleXMLElement $object) {
    $distribution_key = new CultureFeed_Uitpas_DistributionKey();
    $distribution_key->id = $object->xpath_int('id');
    $distribution_key->name = $object->xpath_str('name');

    return $distribution_key;
  }

}