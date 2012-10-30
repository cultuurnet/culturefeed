<?php

/**
 * @class
 * Representation of an EventDetail element in the cdb xml.
 */
class CultureFeed_Cdb_Data_EventDetail extends CultureFeed_Cdb_Data_Detail implements CultureFeed_Cdb_IElement {

  /**
   * @see CultureFeed_Cdb_IElement::appendToDOM()
   */
  public function appendToDOM(DOMElement $element) {

    $dom = $element->ownerDocument;

    $detailElement = $dom->createElement('eventdetail');
    $detailElement->setAttribute('lang', $this->language);

    if (!empty($this->shortDescription)) {
      $detailElement->appendChild($dom->createElement('shortdescription', htmlentities($this->shortDescription)));
    }

    $detailElement->appendChild($dom->createElement('title', htmlentities($this->title)));

    $element->appendChild($detailElement);

  }

  /**
   * @see CultureFeed_Cdb_IElement::parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement)
   * @return CultureFeed_Cdb_Data_EventDetailList
   */
  public static function parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement) {

    if (empty($xmlElement->title)) {
      throw new CultureFeed_ParseException("Title missing for eventdetail element");
    }

    $attributes = $xmlElement->attributes();
    if (empty($attributes['lang'])) {
      throw new CultureFeed_ParseException("Lang missing for eventdetail element");
    }

    $eventDetail = new Culturefeed_Cdb_Data_EventDetail();
    $eventDetail->setTitle((string)$xmlElement->title);
    $eventDetail->setLanguage((string)$attributes['lang']);

    if (!empty($xmlElement->shortdescription)) {
      $eventDetail->setShortDescription((string)$xmlElement->shortdescription);
    }

    return $eventDetail;

  }

}