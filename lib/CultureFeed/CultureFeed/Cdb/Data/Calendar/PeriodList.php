<?php

/**
 * @class
 * Representation of a periods element in the cdb xml.
 */
class CultureFeed_Cdb_Data_Calendar_PeriodList extends CultureFeed_Cdb_Data_Calendar implements CultureFeed_Cdb_IElement {

  /**
   * Add a new period to the list.
   * @param DOMElement $timestamp
   *   Period to add.
   */
  public function add(CultureFeed_Cdb_Calendar_Period $period) {
    $this->items[] = $period;
  }

  /**
   * @see CultureFeed_Cdb_IElement::appendToDOM()
   */
  public function appendToDOM(DOMElement $element) {
    $dom = $element->ownerDocument;

    $calendarElement = $dom->createElement('calendar');
    $element->appendChild($calendarElement);

    $periodElement = $dom->createElement('periods');
    $calendarElement->appendChild($periodElement);

    foreach ($this as $period) {
      $period->appendToDom($periodElement);
    }
  }

  /**
   * @see CultureFeed_Cdb_IElement::parseFromCdbXml($xmlElement)
   * @return CultureFeed_Cdb_Data_PeriodList
   */
  public static function parseFromCdbXml($xmlElement) {

    $periodList = new CultureFeed_Cdb_Data_PeriodList();
    foreach ($xmlElement->period as $periodElement) {
      $periodList->add(CultureFeed_Cdb_Data_Calendar_Period::parseFromCdbXml($periodElement));
    }

    return $periodList;

  }

}

