<?php

class CultureFeed_Uitpas_Calendar {

  /**
   * The timestamps of the calendar object
   *
   * @var CultureFeed_Uitpas_Calendar_Timestamp[]
   */
  public $data = array();

  public function addTimestamp(CultureFeed_Uitpas_Calendar_Timestamp $timestamp) {
    $this->data[] = $timestamp;
  }

  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {
    $calendar = new CultureFeed_Uitpas_Calendar();

    foreach ($object->xpath('ns6:timestamps/ns6:timestamp') as $timeObject) {
      $timestamp = new CultureFeed_Uitpas_Calendar_Timestamp();
      $timestamp->date = $timeObject->xpath_time('ns6:date');
      $timestamp->start = $timeObject->xpath_time('ns6:timestart');
      $timestamp->end = $timeObject->xpath_time('ns6:timeend');

      $calendar->addTimestamp($timestamp);
    }

    return $calendar;
  }

}