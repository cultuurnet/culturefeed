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

    $object->registerXPathNamespace('rdf', 'http://www.w3.org/1999/02/22-rdf-syntax-ns');
    $object->registerXPathNamespace('foaf', 'http://xmlns.com/foaf/0.1/');

    $member->id = $object->xpath_str('rdf:id');
    $member->nick = $object->xpath_str('foaf:nick');

    return $member;
  }

}
