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
   * @var string
   */
  public $checkinReason;

  /**
   * @param CultureFeed_SimpleXMLElement $xml
   * @return CultureFeed_Uitpas_Passholder_EventCheckin
   */
  public static function createFromXml(CultureFeed_SimpleXMLElement $xml) {
    $eventCheckin = new self();

    $eventCheckin->checkinAllowed = $xml->xpath_bool('checkinAllowed');
    $eventCheckin->numberOfPoints = $xml->xpath_int('numberOfPoints');
    $eventCheckin->cdbid = $xml->xpath_str('cdbid');
    $eventCheckin->checkinReason = $xml->xpath_str('checkinReason');

    return $eventCheckin;
  }
}
