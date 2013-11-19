<?php
/**
 * @file
 */ 

class CultureFeed_Uitpas_Passholder_CardSystemSpecific {

  /**
   * @var CultureFeed_Uitpas_Passholder_Card
   */
  public $currentCard;

  /**
   * The e-mail preference
   *
   * @var string
   */
  public $emailPreference;

  /**
   * The SMS preference
   *
   * @var string
   */
  public $smsPreference;

  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {
    $cardSystemSpecific = new self();
    $currentCard = $object->xpath('currentCard', false);
    if ($currentCard instanceof CultureFeed_SimpleXMLElement) {
      $cardSystemSpecific->currentCard = CultureFeed_Uitpas_Passholder_Card::createFromXML($currentCard);
    }

    $cardSystemSpecific->emailPreference = $object->xpath_str('emailPreference');
    $cardSystemSpecific->smsPreference = $object->xpath_str('smsPreference');

    return $cardSystemSpecific;
  }
}
