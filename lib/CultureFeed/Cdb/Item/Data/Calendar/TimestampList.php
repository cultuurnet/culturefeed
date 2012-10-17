<?php

/**
 * @class
 * Representation of a timestamps element in the cdb xml.
 */
class CultureFeed_Cdb_Calendar_TimestampList extends CultureFeed_Cdb_Calendar implements ICultureFeed_Cdb_Element {

  /**
   * Add a new timestamp to the list.
   * @param DOMElement $timestamp
   *   Timestamp to add.
   */
  public function add(CultureFeed_Cdb_Calendar_Timestamp $timestamp) {
    $this->items[] = $timestamp;
  }

  /**
   * @see ICultureFeed_Cdb_Element::appendToDOM()
   */
  public function appendToDOM(DOMElement $element) {

    $dom = $element->ownerDocument;

    $calendarElement = $dom->createElement('calendar');
    $element->appendChild($calendarElement);

    $timestampsElement = $dom->createElement('timestamps');
    $calendarElement->appendChild($timestampsElement);

    foreach ($this as $timestamp) {
      $timestamp->appendToDom($timestampsElement);
    }

  }

  /**
   * @see ICultureFeed_Cdb_Element::parseFromCdbXml($xmlElement)
   * @return CultureFeed_Cdb_PeriodList
   */
  public static function parseFromCdbXml($xmlElement) {

    $timestampList = new CultureFeed_Cdb_TimestampList();
    foreach ($xmlElement->timestamp as $timestampElement) {
      $timestampList->add(CultureFeed_Cdb_Calendar_Timestamp::parseFromCdbXml($timestampElement));
    }

    return $timestampList;

  }

}

