<?php

class CultureFeed_Uitpas_Passholder_Promotion extends CultureFeed_Uitpas_ValueObject {

  /**
   * The ID of the promotion
   *
   * @var integer
   */
  public $id;

  /**
   * The title of the promotion
   *
   * @var string
   */
  public $title;

  /**
   * The amount of points required for the promotion
   *
   * @var integer
   */
  public $points;

  /**
   * The counters to which the promotion applies
   *
   * @var CultureFeed_Uitpas_Passholder_Counter[]
   */
  public $counters = array();

  /**
   * True if the promotion has been cashed in
   *
   * @var boolean
   */
  public $cashedIn;

  /**
   * The creation date of the promotion
   *
   * @var integer
   */
  public $creationDate;

  /**
   * The date when the cashing for the promotion will start
   *
   * @var integer
   */
  public $cashingPeriodBegin;

  /**
   * The date when the cashing for the promotion will end
   *
   * @var integer
   */
  public $cashingPeriodEnd;

  /**
   * The cities for which the promotion applies
   *
   * @var array
   */
  public $validCities = array();

  /**
   * The amount of units available for this promotion
   *
   * @var integer
   */
  public $maxAvailableUnits;

  /**
   * The amount of units taken for this promotion
   *
   * @var integer
   */
  public $unitsTaken;

  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {
    $promotion = new CultureFeed_Uitpas_Passholder_Promotion();
    $promotion->id = $object->xpath_int('id');
    $promotion->title = $object->xpath_str('title');
    $promotion->points = $object->xpath_int('points');
    $promotion->cashedIn = $object->xpath_bool('cashedIn');
    $promotion->creationDate = $object->xpath_time('creationDate');
    $promotion->cashingPeriodBegin = $object->xpath_time('cashingPeriodBegin');
    $promotion->cashingPeriodEnd = $object->xpath_time('cashingPeriodEnd');
    $promotion->validCities = $object->xpath_str('validForCities/city', true);
    $promotion->maxAvailableUnits = $object->xpath_int('maxAvailableUnits');
    $promotion->unitsTaken = $object->xpath_int('unitsTaken');

    foreach ($object->xpath('balies/balie') as $counter) {
      $promotion->counters[] = CultureFeed_Uitpas_Passholder_Counter::createFromXML($counter);
    }

    return $promotion;
  }
}