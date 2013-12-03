<?php

class CultureFeed_Uitpas_PassholderException extends Exception {

  /**
   * @var array
   */
  public $cardSystemLinks;

  /**
   * @param string $code.
   * @param CultureFeed_SimpleXMLElement $xml
   */
  public static function createFromXML($code, $xml) {

    $exception = new static();
    parent::__construct($code);

    if ($code == 'INSZ_ALREADY_USED') {

      $exception->cardSystemLinks = array();
      foreach ($xml->xpath('cardSystemLinks/cardSystemLink') as $cardSystemLink) {
        $exception->cardSystemLinks[] = CultureFeed_Uitpas_CardInfo::createFromXml($cardSystemLink, FALSE);
      }

    }

  }

}