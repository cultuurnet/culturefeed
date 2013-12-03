<?php

class CultureFeed_Uitpas_Counter_Employee extends CultureFeed_Uitpas_ValueObject {

  /**
   * The id of the counter
   *
   * @var string
   */
  public $id;

  /**
   * The consumer key of the counter
   *
   * @var string
   */
  public $consumerKey;

  /**
   * The name of the counter
   *
   * @var string
   */
  public $name;

  /**
   * The role of the member in the counter
   *
   * @var bool
   */
  public $role;

  /**
   * @var string
   */
  public $actorId;

  /**
   * @var CultureFeed_Uitpas_Counter_EmployeeCardSystem[]
   */
  public $cardSystems = array();


  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {
    $counter = new CultureFeed_Uitpas_Counter_Employee();
    $counter->id = $object->xpath_str('id');
    $counter->consumerKey = $object->xpath_str('consumerKey');
    $counter->name = $object->xpath_str('name');
    $counter->role = $object->xpath_str('role');
    $counter->actorId = $object->xpath_str('actorId');

    foreach ($object->xpath('cardSystems/cardSystem') as $card_system) {
      $counter->cardSystems[] = CultureFeed_Uitpas_Counter_EmployeeCardSystem::createFromXml($card_system);
    }

    return $counter;
  }

}
