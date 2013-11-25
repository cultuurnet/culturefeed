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
    $currentCard = $object->xpath('currentCard', false);
    if ($currentCard instanceof CultureFeed_SimpleXMLElement) {
      $cardSystemSpecific->currentCard = CultureFeed_Uitpas_Passholder_Card::createFromXML($currentCard);
    }

    return $cardSystemSpecific;
  }
}
