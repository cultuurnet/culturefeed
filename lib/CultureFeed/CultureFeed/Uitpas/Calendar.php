<?php

class CultureFeed_Uitpas_Calendar {

  /**
   * The periods of the calendar object
   *
   * @var CultureFeed_Uitpas_Calendar_Timestamp[]
   */
  public $timestamps = array();
  
  /**
   * The periods of the calendar object
   *
   * @var CultureFeed_Uitpas_Calendar_Period[]
   */
  public $periods = array();

  public function addPeriod(CultureFeed_Uitpas_Calendar_Period $period) {
    $this->periods[] = $period;
  }
  
  public function addTimestamp(CultureFeed_Uitpas_Calendar_Timestamp $timestamp) {
    $this->timestamps[] = $timestamp;
  }

  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {
    $object->registerXPathNamespace('cdb', CultureFeed_Cdb_Default::CDB_SCHEME_URL);

    $calendar = new CultureFeed_Uitpas_Calendar();

    foreach ($object->xpath('cdb:periods/cdb:period') as $timeObject) {
      $period = new CultureFeed_Uitpas_Calendar_Period();
      $period->datefrom = $timeObject->xpath_time('cdb:datefrom');
      $period->dateto = $timeObject->xpath_time('cdb:dateto');

      $calendar->addPeriod($period);
    }
    
    foreach ($object->xpath('cdb:timestamps/cdb:timestamp') as $timeObject) {
      $timestamp = new CultureFeed_Uitpas_Calendar_Timestamp();
      $timestamp->date = $timeObject->xpath_time('cdb:date');
      $timestamp->timestart = $timeObject->xpath_str('cdb:timestart');
      $timestamp->timeend = $timeObject->xpath_str('cdb:timeend');

      $calendar->addTimestamp($timestamp);
    }

    return $calendar;
  }

}
