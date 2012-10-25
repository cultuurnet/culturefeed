<?php

/**
 * @class
 * Representation of a virtual address element in the cdb xml.
 */
class CultureFeed_Cdb_Data_VirtualAddress implements CultureFeed_Cdb_IElement {

  /**
   * Title from the virtual address.
   * @var string
   */
  protected $title;

  /**
   * Construct the virtual address.
   * @param string $title
   *   Title from the address.
   */
  public function __construct($title) {
    $this->title = $title;
  }

  /**
   * Set the title.
   * @param string $title
   *   Title to set.
   */
  public function setTitle($title) {
    $this->title = $title;
  }

  /**
   * Get the title.
   */
  public function getTitle() {
    return $this->title;
  }

  /**
   * @see CultureFeed_Cdb_IElement::appendToDOM()
   */
  public function appendToDOM(DOMElement $element) {

    $dom = $element->ownerDocument;

    $virtualElement = $dom->createElement('virtual');
    $virtualElement->appendChild($dom->createElement('title', $this->title));

    $element->appendChild($virtualElement);

  }

  /**
   * @see CultureFeed_Cdb_IElement::parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement)
   * @return CultureFeed_Cdb_Data_VirtualAddress
   */
  public static function parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement) {

  }

}
