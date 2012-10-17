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

  /**
   * @see ICultureFeed_Cdb_Element::parseFromCdbXml($xmlElement)
   * @return CultureFeed_Cdb_EventDetailList
   */
  public static function parseFromCdbXml($xmlElement) {

    $attributes = $xmlElement->attributes();
    $eventDetail = new Culturefeed_Cdb_EventDetail();
    $eventDetail->setTitle((string)$xmlElement->title);
    $eventDetail->setShortDescription((string)$xmlElement->shortdescription);
    $eventDetail->setLanguage($attributes['lang']);

    return $eventDetail;

  }

}