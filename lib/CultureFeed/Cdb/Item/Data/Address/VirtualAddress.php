<?php

/**
 * @class
 * Representation of a virtual address element in the cdb xml.
 */
class CultureFeed_Cdb_VirtualAddress implements ICultureFeed_Cdb_Element {

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
   * @see ICultureFeed_Cdb_Element::appendToDOM()
   */
  public function appendToDOM(DOMElement $element) {

    $dom = $element->ownerDocument;

    $virtualElement = $dom->createElement('virtual');
    $virtualElement->appendChild($dom->createElement('title', $this->title));

    $element->appendChild($virtualElement);

  }

  /**
   * @see ICultureFeed_Cdb_Element::parseFromCdbXml($xmlElement)
   * @return CultureFeed_Cdb_PhysicalAddress
   */
  public static function parseFromCdbXml($xmlElement) {

  }

}
