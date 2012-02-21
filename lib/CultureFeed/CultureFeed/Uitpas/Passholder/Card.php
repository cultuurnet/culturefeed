<?php

class CultureFeed_Uitpas_Passholder_Card extends CultureFeed_Uitpas_ValueObject {

  const STATUS_STOCK = 'STOCK';
  const STATUS_ACTIVE = 'ACTIVE';
  const STATUS_BLOCKED = 'BLOCKED';

  /**
   * The city of the uitpas
   *
   * @var string
   */
  public $city;

  /**
   * The number of the uitpas
   *
   * @var string
   */
  public $uitpasNumber;

  /**
   * True if the passholder has a kansenpas
   *
   * @var boolean
   */
  public $kansenpas;

  /**
   * Current status of the uitpas
   *
   * @var string
   */
  public $status;

  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {
    $card = new CultureFeed_Uitpas_Passholder_Card();
    $card->city= $object->xpath_str('city');
    $card->uitpasNumber= $object->xpath_str('uitpasNumber');
    $card->kansenpas= $object->xpath_bool('kansenpas');
    $card->status= $object->xpath_str('status');

    return $card;
  }

}