<?php

class CultureFeed_Uitpas_Passholder_CashedInPointsPromotion extends CultureFeed_Uitpas_ValueObject {

  /**
   * ID of the advantage object.
   *
   * @var int
   */
  public $id;

  /**
   * True is the advantage object has been cashed in.
   *
   * @var boolean
   */
  public $title;

  /**
   * The counters of the promotion item
   *
   * @var array
   */
  public $location;
  
  public $points;
  
  public $cashingDate;

  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {
    // TODO: implement properly
    $promotion = new CultureFeed_Uitpas_Passholder_CashedInPointsPromotion();
    $promotion->id = $object->xpath_int('promotion/id');
    $promotion->title = $object->xpath_str('promotion/title');
    $promotion->points = $object->xpath_int('promotion/points');
    $promotion->cashingDate = $object->xpath_time('cashingDate');
    $promotion->location = $object->xpath_str('balie/name');
    $promotion->counters = array(CultureFeed_Uitpas_Passholder_Counter::createFromXML($object->balie));
    
    return $promotion;
  }

}