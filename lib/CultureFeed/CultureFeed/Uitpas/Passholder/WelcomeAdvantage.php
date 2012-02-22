<?php

class CultureFeed_Uitpas_Passholder_WelcomeAdvantage extends CultureFeed_Uitpas_ValueObject {

  /**
   * ID of the advantage object.
   *
   * @var int
   */
  public $id;

  /**
   * Title of the advantage object.
   *
   * @var string
   */
  public $title;

  /**
   * The number of points of the advantage object
   *
   * @var int
   */
  public $points;

  /**
   * True is the advantage object has been cashed in.
   *
   * @var boolean
   */
  public $cashedIn;

  /**
   * The counters of the promotion item
   *
   * @var array
   */
  public $counters;

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
   * Begin date of the granting period
   *
   * @var integer
   */
  public $grantingPeriodBegin;

  /**
   * End date of the granting period
   *
   * @var integer
   */
  public $grantingPeriodEnd;

  /**
   * The cities for which the promotion applies
   *
   * @var array
   */
  public $validForCities = array();

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
    $welcome_advantage = new CultureFeed_Uitpas_Passholder_WelcomeAdvantage();
    $welcome_advantage->id = $object->xpath_int('id');
    $welcome_advantage->title = $object->xpath_str('title');
    $welcome_advantage->points = $object->xpath_int('points');
    $welcome_advantage->cashedIn = $object->xpath_bool('cashedIn');
    $welcome_advantage->counters = $object->xpath_str('balies/name', true);
    $welcome_advantage->creationDate = $object->xpath_time('creationDate');
    $welcome_advantage->cashingPeriodBegin = $object->xpath_time('cashingPeriodBegin');
    $welcome_advantage->cashingPeriodEnd = $object->xpath_time('cashingPeriodEnd');
    $welcome_advantage->grantingPeriodBegin = $object->xpath_time('grantingPeriodBegin');
    $welcome_advantage->grantingPeriodEnd = $object->xpath_time('grantingPeriodEnd');
    $welcome_advantage->cashingPeriodBegin = $object->xpath_time('cashingPeriodBegin');
    $welcome_advantage->validForCities = $object->xpath_str('validForCities/city', true);
    $welcome_advantage->maxAvailableUnits = $object->xpath_int('maxAvailableUnits');
    $welcome_advantage->unitsTaken = $object->xpath_int('unitsTaken');

    return $welcome_advantage;
  }

}