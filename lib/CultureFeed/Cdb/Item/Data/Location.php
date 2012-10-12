<?php

/**
 * @class
 * Representation of a location element in the cdb xml.
 */
class CultureFeed_Cdb_Location implements ICultureFeed_Cdb_Element {

  /**
   * Address from the location.
   * @var CultureFeed_Cdb_Address
   */
  protected $address;

  /**
   * Location label.
   * @var string
   */
  protected $label;

  /**
   * Location actor.
   * @var
   */
  protected $actor;

  /**
   * Construct a new location.
   * @param CultureFeed_Cdb_Address $address
   *   Address from the location.
   */
  public function __construct(CultureFeed_Cdb_Address $address) {
    $this->address = $address;
  }

  /**
   * Get the address.
   */
  public function getAddress() {
    return $this->address;
  }

  /**
   * Get the label.
   */
  public function getLabel() {
    return $this->label;
  }

  /**
   * Set the address.
   * @param CultureFeed_Cdb_Address $address
   *   Address to set.
   */
  public function setAddress(CultureFeed_Cdb_Address $address) {
    $this->address = $address;
  }

  /**
   * Set the label
   * @param string $label
   *   Label to set.
   */
  public function setLabel($label) {
    $this->label = $label;
  }

  /**
   * @see ICultureFeed_Cdb_Element::appendToDOM()
   */
  public function appendToDOM(DOMELement $element) {

    $dom = $element->ownerDocument;

    $locationElement = $dom->createElement('location');

    if ($this->address) {
      $this->address->appendToDOM($locationElement);
    }

    if ($this->label) {
      $locationElement->appendChild($dom->createElement('label', htmlentities($this->label)));
    }

    $element->appendChild($locationElement);

  }

}
