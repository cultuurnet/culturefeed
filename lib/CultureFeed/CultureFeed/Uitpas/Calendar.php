<?php

class CultureFeed_Uitpas_Calendar {

  /**
   * The timestamps of the calendar object
   *
   * @var CultureFeed_Uitpas_Calendar_Timestamp[]
   */
  public $data = array();

  public function add(CultureFeed_Uitpas_Calendar_Timestamp $timestamp) {
    $this->data[] = $timestamp;
  }

}