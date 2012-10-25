<?php

/**
 * @class
 * Representation of a list of event details in the cdb xml.
 */
class CultureFeed_Cdb_Data_EventDetailList extends CultureFeed_Cdb_Data_DetailList implements CultureFeed_Cdb_IElement {

  /**
   * @see CultureFeed_Cdb_IElement::appendToDOM()
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
   * @see CultureFeed_Cdb_IElement::parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement)
   * @return CultureFeed_Cdb_Data_EventDetailList
   */
  public static function parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement) {

    $detailList = new CultureFeed_Cdb_Data_EventDetailList();
    if (!empty($xmlElement->eventdetail)) {
      foreach ($xmlElement->eventdetail as $detailElement) {
        $detailList->add(CultureFeed_Cdb_Data_EventDetail::parseFromCdbXml($detailElement));
      }
    }

    return $detailList;

  }

}
