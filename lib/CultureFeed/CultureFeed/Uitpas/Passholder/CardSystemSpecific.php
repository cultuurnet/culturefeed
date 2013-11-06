<?php
/**
 * @file
 */ 

class CultureFeed_Uitpas_Passholder_CardSystemSpecific {

  /**
   * @var CultureFeed_Uitpas_Passholder_Card
   */
  public $currentCard;

  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {
    $cardSystemSpecific = new self();
    $cardSystemSpecific->currentCard = CultureFeed_Uitpas_Passholder_Card::createFromXML($object->xpath('currentCard', false));

    return $cardSystemSpecific;
  }
}
