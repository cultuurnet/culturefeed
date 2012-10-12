<?php

/**
 * @class
 * Representation of an EventDetail element in the cdb xml.
 */
class CultureFeed_Cdb_EventDetail extends CultureFeed_Cdb_Detail implements ICultureFeed_Cdb_Element {

  /**
   * @see ICultureFeed_Cdb_Element::appendToDOM()
   */
  public function appendToDOM(DOMElement $element) {

    $dom = $element->ownerDocument;

    $detailElement = $dom->createElement('eventdetail');
    $detailElement->setAttribute('lang', $this->language);
    $detailElement->appendChild($dom->createElement('shortdescription', htmlentities($this->shortDescription)));
    $detailElement->appendChild($dom->createElement('title', htmlentities($this->title)));

    $element->appendChild($detailElement);

  }

}