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
   * Description 1 of the advantage object.
   *
   * @var string
   */
  public $description1;

  /**
   * Description 2 of the advantage object.
   *
   * @var string
   */
  public $description2;

  /**
   * Pictures from the advantage
   *
   * @var array
   */
  public $pictures = array();

  /**
   * The date when the promotion will be pubished
   *
   * @var integer
   */
  public $publicationPeriodBegin;

  /**
   * The date when the promotion will be unpubished
   *
   * @var integer
   */
  public $publicationPeriodEnd;

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
   * The date the welcome advantage was cashed in.
   *
   * @var integer
   */
  public $cashingDate;

  /**
   * The counters of the promotion item
   *
   * @var array
   */
  public $counters = array();

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

  /**
   * @var CultureFeed_Uitpas_CardSystem Card system owning the welcome advantage
   */
  public $owningCardSystem;

  /**
   * @var CultureFeed_Uitpas_CardSystem[] Card systems the welcome advantage
   * applies to
   */
  public $applicableCardSystems = array();


  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {
    $welcome_advantage = new CultureFeed_Uitpas_Passholder_WelcomeAdvantage();
    $welcome_advantage->id = $object->xpath_int('id');
    $welcome_advantage->title = $object->xpath_str('title');
    $welcome_advantage->description1 = $object->xpath_str('description1');
    $welcome_advantage->description2 = $object->xpath_str('description2');   
    $welcome_advantage->pictures = $object->xpath_str('pictures/picture', TRUE);
    $welcome_advantage->publicationPeriodBegin = $object->xpath_time('publicationPeriodBegin');
    $welcome_advantage->publicationPeriodEnd = $object->xpath_time('publicationPeriodEnd');
    $welcome_advantage->points = $object->xpath_int('points');
    $welcome_advantage->cashedIn = $object->xpath_bool('cashedIn');
    $welcome_advantage->cashingDate = $object->xpath_time('cashingDate');
    foreach ($object->xpath('balies/balie') as $counter) {
      $welcome_advantage->counters[] = CultureFeed_Uitpas_Passholder_Counter::createFromXML($counter);
    }
    $welcome_advantage->creationDate = $object->xpath_time('creationDate');
    $welcome_advantage->cashingPeriodBegin = $object->xpath_time('cashingPeriodBegin');
    $welcome_advantage->cashingPeriodEnd = $object->xpath_time('cashingPeriodEnd');
    $welcome_advantage->grantingPeriodBegin = $object->xpath_time('grantingPeriodBegin');
    $welcome_advantage->grantingPeriodEnd = $object->xpath_time('grantingPeriodEnd');
    $welcome_advantage->cashingPeriodBegin = $object->xpath_time('cashingPeriodBegin');
    $welcome_advantage->validForCities = $object->xpath_str('validForCities/city', true);
    $welcome_advantage->maxAvailableUnits = $object->xpath_int('maxAvailableUnits');
    $welcome_advantage->unitsTaken = $object->xpath_int('unitsTaken');

    $owningCardSystem = $object->xpath('owningCardSystem', FALSE);
    if ($owningCardSystem instanceof CultureFeed_SimpleXMLElement) {
      $welcome_advantage->owningCardSystem = CultureFeed_Uitpas_CardSystem::createFromXml($owningCardSystem);
    }

    $applicableCardSystems = $object->xpath('applicableCardSystems/cardsystem');
    foreach ($applicableCardSystems as $applicableCardSystem) {
      $welcome_advantage->applicableCardSystems[] = CultureFeed_Uitpas_CardSystem::createFromXml($applicableCardSystem);
    }

    return $welcome_advantage;
  }

}
