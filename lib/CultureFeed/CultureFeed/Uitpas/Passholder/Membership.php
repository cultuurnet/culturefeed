<?php

class CultureFeed_Uitpas_Passholder_Membership extends CultureFeed_Uitpas_ValueObject {

  /**
   * The association the passholder is linked
   *
   * @var string
   */
  public $association;

  /**
   * The membership's organization end date. (Required)
   *
   * @var integer
   */
  public $endDate;
  
  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {
    $membership = new CultureFeed_Uitpas_Passholder_Membership();
    $membership->association = $object->xpath_str('association');
    $membership->endDate = $object->xpath_time('endDate');

    return $membership;
  }

}