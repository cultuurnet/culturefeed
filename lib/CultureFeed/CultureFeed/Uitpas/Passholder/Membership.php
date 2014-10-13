<?php

class CultureFeed_Uitpas_Passholder_Membership extends CultureFeed_Uitpas_ValueObject {

  /**
   * The id of the association the passholder is linked
   *
   * @var string
   */
  public $associationId;
  
  /**
   * The name of the association the passholder is linked
   *
   * @var string
   */
  public $name; 

  /**
   * The membership's organization end date. (Required)
   *
   * @var integer
   */
  public $endDate;
  
  protected function manipulatePostData(&$data) {
    
    if (isset($data['endDate'])) {
      $data['endDate'] = date('Y-m-d', $data['endDate']);
    }
  }
  
  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {

    $membership = new CultureFeed_Uitpas_Passholder_Membership();
    $membership->associationId = $object->xpath_str('association/id');
    $membership->name = $object->xpath_str('association/name');
    $membership->endDate = $object->xpath_time('endDate');

    return $membership;
  }

}