<?php

/**
 * @class
 * Representation of a periods element in the cdb xml.
 */
class CultureFeed_Cdb_Data_Calendar_PeriodList extends CultureFeed_Cdb_Data_Calendar implements CultureFeed_Cdb_IElement {

  /**
   * Add a new period to the list.
   * @param CultureFeed_Cdb_Data_Calendar_Period $period
   *   Period to add.
   */
  public function add(CultureFeed_Cdb_Data_Calendar_Period $period) {
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
   * @see CultureFeed_Cdb_IElement::parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement)
   * @return CultureFeed_Cdb_Data_Calendar_PeriodList
   */
  public static function parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement) {

    $periodList = new CultureFeed_Cdb_Data_Calendar_PeriodList();

    if (!empty($xmlElement->periods->period)) {
      foreach ($xmlElement->periods->period as $periodElement) {
        $periodList->add(CultureFeed_Cdb_Data_Calendar_Period::parseFromCdbXml($periodElement));
      }
    }

    return $periodList;

  }

}

