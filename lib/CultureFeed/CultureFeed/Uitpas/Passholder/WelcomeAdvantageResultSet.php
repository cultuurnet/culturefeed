<?php
/**
 *
 */

class CultureFeed_Uitpas_Passholder_WelcomeAdvantageResultSet extends CultureFeed_ResultSet {

  public static function createFromXML(CultureFeed_SimpleXMLElement $xml, $promotionElementName = 'promotion') {
    $advantages = array();
    $objects = $xml->xpath('/promotions/' . $promotionElementName);
    $total = count($objects);

    foreach ($objects as $object) {
      $advantages[] = CultureFeed_Uitpas_Passholder_WelcomeAdvantage::createFromXML($object);
    }

    return new self($total, $advantages);
  }
}
