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

  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {
    $welcome_advantage = new CultureFeed_Uitpas_Passholder_WelcomeAdvantage();
    $welcome_advantage->id = $object->xpath_int('id');
    $welcome_advantage->title = $object->xpath_str('title');
    $welcome_advantage->points = $object->xpath_int('points');
    $welcome_advantage->cashedIn = $object->xpath_bool('cashedIn');

    return $welcome_advantage;
  }

}