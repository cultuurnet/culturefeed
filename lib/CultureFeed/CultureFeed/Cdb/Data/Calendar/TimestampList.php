<?php

/**
 * @class
 * Representation of a timestamps element in the cdb xml.
 */
class CultureFeed_Cdb_Data_Calendar_TimestampList extends CultureFeed_Cdb_Data_Calendar implements CultureFeed_Cdb_IElement {

  /**
   * Add a new timestamp to the list.
   * @param CultureFeed_Cdb_Data_Calendar_Timestamp $timestamp
   *   Timestamp to add.
   */
  public function add(CultureFeed_Cdb_Data_Calendar_Timestamp $timestamp) {
    $this->items[] = $timestamp;
  }

  /**
   * @see CultureFeed_Cdb_IElement::appendToDOM()
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
   * @see CultureFeed_Cdb_IElement::parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement)
   * @return CultureFeed_Cdb_Data_PeriodList
   */
  public static function parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement) {

    $timestampList = new CultureFeed_Cdb_Data_Calendar_TimestampList();
    if (!empty($xmlElement->timestamp)) {
      foreach ($xmlElement->timestamp as $timestampElement) {
        $timestampList->add(CultureFeed_Cdb_Data_Calendar_Timestamp::parseFromCdbXml($timestampElement));
      }
    }

    return $timestampList;

  }

}

