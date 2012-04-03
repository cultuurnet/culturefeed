<?php

class CultureFeed_Uitpas_Counter_Member extends CultureFeed_Uitpas_ValueObject {

  /**
   * The id of the counter
   *
   * @var string
   */
  public $id;

  /**
   * The role of the member in the counter
   *
   * @var bool
   */
  public $nick;

  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {
    $member = new CultureFeed_Uitpas_Counter_Member();
    $member->id = $object->xpath_str('ns2:id');
    $member->nick = $object->xpath_str('ns3:nick');

    return $member;
  }

}