<?php

class CultureFeed_Uitpas_Passholder_UitIdUser extends CultureFeed_Uitpas_ValueObject {

  /**
   * The ID of the user
   *
   * @var string
   */
  public $id;

  /**
   * The nick of the user
   *
   * @var string
   */
  public $nick;

  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {
    $user = new CultureFeed_Uitpas_Passholder_UitIdUser();
    $user->id = $object->xpath_str('ns2:id');
    $user->nick = $object->xpath_str('ns2:nick');

    return $user;
  }

}