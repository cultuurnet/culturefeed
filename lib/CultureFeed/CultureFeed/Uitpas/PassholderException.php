<?php

class CultureFeed_Uitpas_PassholderException extends CultureFeed_Exception {

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

    $message = $xml->xpath_str('/response/message');
    $exception = new static($message, $code);

    if ($code == 'INSZ_ALREADY_USED') {

      $exception->cardSystemLinks = array();
      foreach ($xml->xpath('cardSystemLinks/cardSystemLink') as $cardSystemLink) {
        $exception->cardSystemLinks[] = CultureFeed_Uitpas_CardInfo::createFromXml($cardSystemLink, FALSE);
      }
      $exception->userId = $xml->xpath_str('/response/userId');

    }

    return $exception;

  }

}
