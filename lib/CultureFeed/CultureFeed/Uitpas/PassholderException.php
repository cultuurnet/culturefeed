<?php

class CultureFeed_Uitpas_PassholderException extends Exception {

  /**
   * @var CultureFeed_Uitpas_CardInfo[]
   */
  public $cardSystemLinks;

  /**
   * @param string $code.
   * @param CultureFeed_SimpleXMLElement $xml
   *
   * @return self
   */
  public static function createFromXML($code, $xml) {

    $exception = new static($code);

    if ($code == 'INSZ_ALREADY_USED') {

      $exception->cardSystemLinks = array();
      foreach ($xml->xpath('cardSystemLinks/cardSystemLink') as $cardSystemLink) {
        $exception->cardSystemLinks[] = CultureFeed_Uitpas_CardInfo::createFromXml($cardSystemLink, FALSE);
      }

    }

  }

}
