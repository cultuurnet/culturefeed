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

    $object->registerXPathNamespace('rdf', 'http://www.w3.org/1999/02/22-rdf-syntax-ns');
    $object->registerXPathNamespace('foaf', 'http://xmlns.com/foaf/0.1/');

    $user->id = $object->xpath_str('rdf:id');
    $user->nick = $object->xpath_str('foaf:nick');

    return $user;
  }

}
