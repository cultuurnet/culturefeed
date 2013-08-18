<?php
/**
 *
 */
class CultureFeed_Uitpas_Passholder_EventBuyTicket {
  /**
   * @var float
   */
  public $tariff;

  /**
   * @var float
   */
  public $price;

  /**
   * @var string
   */
  public $cdbid;

  /**
   * @param CultureFeed_SimpleXMLElement $xml
   * @return CultureFeed_Uitpas_Passholder_EventBuyTicket
   */
  public static function createFromXml(CultureFeed_SimpleXMLElement $xml) {
    $buyTicket = new self();

    $buyTicket->tariff = $xml->xpath_float('tariff', TRUE);
    $buyTicket->price = $xml->xpath_float('price', TRUE);
    $buyTicket->cdbid = $xml->xpath_str('cdbid', TRUE);

    return $buyTicket;
  }
}
