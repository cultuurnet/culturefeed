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
   * @var string
   */
  public $buyConstraintReason;

  /**
   * @param CultureFeed_SimpleXMLElement $xml
   * @return CultureFeed_Uitpas_Passholder_EventBuyTicket
   */
  public static function createFromXml(CultureFeed_SimpleXMLElement $xml) {
    $buyTicket = new self();

    $buyTicket->tariff = $xml->xpath_float('tariff');
    $buyTicket->price = $xml->xpath_float('price');
    $buyTicket->cdbid = $xml->xpath_str('cdbid');
    $buyTicket->buyConstraintReason = $xml->xpath_str('buyConstraintReason');

    return $buyTicket;
  }
}
