<?php

class CultureFeed_Uitpas_Passholder_PeriodConstraint extends CultureFeed_Uitpas_ValueObject {

  const TYPE_ABSOLUTE = 'ABSOLUTE';
  const TYPE_WEEK = 'WEEK';
  const TYPE_MONTH = 'MONTH';
  const TYPE_QUARTER = 'QUARTER';
  const TYPE_YEAR = 'YEAR';

  /**
   * The number of times a action can be cashed in within a given period
   *
   * @var int
   */
  public $periodVolume;

  /**
   * The period a passholder can cash the advantage
   *
   * @var string
   */
  public $periodType;

  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {
    $constraint = new CultureFeed_Uitpas_Passholder_PeriodConstraint();
    $constraint->periodVolume = $object->xpath_int('periodVolume');
    $constraint->periodType = $object->xpath_str('periodType');

    return $constraint;
  }

}