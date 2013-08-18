<?php
/**
 *
 */
class CultureFeed_Uitpas_Passholder_EventCheckin {
  /**
   * @var boolean
   */
  public $checkinAllowed;

  /**
   * @var integer
   */
  public $numberOfPoints;

  /**
   * @var string
   */
  public $cdbid;

  /**
   * @param CultureFeed_SimpleXMLElement $xml
   * @return CultureFeed_Uitpas_Passholder_EventCheckin
   */
  public static function createFromXml(CultureFeed_SimpleXMLElement $xml) {
    $eventCheckin = new self();

    $eventCheckin->checkinAllowed = $xml->xpath_bool('checkinAllowed', TRUE);
    $eventCheckin->numberOfPoints = $xml->xpath_int('numberOfPoints', TRUE);
    $eventCheckin->cdbid = $xml->xpath_str('cdbid', TRUE);

    return $eventCheckin;
  }
}
