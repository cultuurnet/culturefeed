<?php

class CultureFeed_Uitpas_Counter_Device extends CultureFeed_Uitpas_ValueObject {

  public $name;

  public $consumerKey;
  
  public $status;
  
  public $cdbid;
  
  public $counters = array();

  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {
    $device = new CultureFeed_Uitpas_Counter_Device();
    $device->name = $object->xpath_str('name');
    $device->consumerKey = $object->xpath_str('consumerKey');
    $device->status = $object->xpath_str('status');
    $device->cdbid = $object->xpath_str('cdbid');
    
    foreach ($object->xpath('balies/balie') as $balie) {
      $device->counters[] = CultureFeed_Uitpas_Counter::createFromXml($balie);
    }

    return $device;
  }

}