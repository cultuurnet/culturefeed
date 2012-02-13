<?php

class CultureFeed_Uitpas_Event_CheckinConstraint extends CultureFeed_Uitpas_ValueObject {

  /**
   * The periodType of the event
   *
   * @var string
   */
  public $periodType;

  /**
   * The periodVolume of the event
   *
   * @var integer
   */
  public $periodVolume;

  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {
    $constraint = new CultureFeed_Uitpas_Event_CheckinConstraint();
    $constraint->periodType = $object->xpath_str('periodType');
    $constraint->periodVolume = $object->xpath_int('periodVolume');

    return $constraint;
  }

}