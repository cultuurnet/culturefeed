<?php

/**
 * @class
 * Representation of a list of event details in the cdb xml.
 */
class CultureFeed_Cdb_EventDetailList extends CultureFeed_Cdb_DetailList implements ICultureFeed_Cdb_Element {

  /**
   * @see ICultureFeed_Cdb_Element::appendToDOM()
   */
  public function appendToDOM(DOMElement $element) {

    $dom = $element->ownerDocument;

    $detailsElement = $dom->createElement('eventdetails');
    foreach ($this as $detail) {
      $detail->appendToDom($detailsElement);
    }

    $element->appendChild($detailsElement);

  }

  /**
   * @see ICultureFeed_Cdb_Element::parseFromCdbXml($xmlElement)
   * @return CultureFeed_Cdb_EventDetailList
   */
  public static function parseFromCdbXml($xmlElement) {

    $detailList = new CultureFeed_Cdb_EventDetailList();
    foreach ($xmlElement->eventdetail as $detailElement) {
      $detailList->add(CultureFeed_Cdb_EventDetail::parseFromCdbXml($detailElement));
    }

    return $detailList;

  }

}
