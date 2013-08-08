<?php
/**
 *
 */
class CultureFeed_Uitpas_Passholder_PointsPromotionResultSet extends CultureFeed_ResultSet {

  public static function createFromXML(CultureFeed_SimpleXMLElement $xml, $elementName = 'promotion') {
    $promotions = array();
    $objects = $xml->xpath('/promotions/' . $elementName);
    $total = $xml->xpath_int('/total');

    foreach ($objects as $object) {
      $promotions[] = CultureFeed_Uitpas_Passholder_PointsPromotion::createFromXML($object);
    }

    return new self($total, $promotions);
  }
}
