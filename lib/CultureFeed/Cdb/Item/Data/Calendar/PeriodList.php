<?php

/**
 * @class
 * Representation of a periods element in the cdb xml.
 */
class CultureFeed_Cdb_Calendar_PeriodList extends CultureFeed_Cdb_Calendar implements ICultureFeed_Cdb_Element {

  /**
   * Add a new period to the list.
   * @param DOMElement $timestamp
   *   Period to add.
   */
  public function add(CultureFeed_Cdb_Calendar_Period $period) {
    $this->items[] = $period;
  }

  /**
   * @see ICultureFeed_Cdb_Element::appendToDOM()
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

}

