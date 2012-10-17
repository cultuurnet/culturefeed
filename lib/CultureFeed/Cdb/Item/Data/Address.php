<?php

/**
 * @class
 * Representation of an address element in the cdb xml.
 */
class CultureFeed_Cdb_Address implements ICultureFeed_Cdb_Element {

  /**
   * Physical address.
   * @var CultureFeed_Cdb_PhysicalAddress
   */
  protected $physicalAddress;

  /**
   * Virtual address.
   * @var CultureFeed_Cdb_VirtualAddress
   */
  protected $virtualAddress;

  /**
   * Construct a new address.
   * @param CultureFeed_Cdb_PhysicalAddress $physical
   *   Physical address.
   * @param CultureFeed_Cdb_VirtualAddress $virtual
   *   Virtual address.
   */
  public function __construct(CultureFeed_Cdb_PhysicalAddress $physical = NULL, CultureFeed_Cdb_VirtualAddress $virtual = NULL) {
    $this->physicalAddress = $physical;
    $this->virtualAddress = $virtual;
  }

  /**
   * Get the physical address.
   */
  public function getPhysicalAddress() {
    return $this->physicalAddress;
  }

  /**
   * Get the virtual address.
   */
  public function getVirtualAddress() {
    return $this->virtualAddress;
  }

  /**
   * Set the physical address
   * @param CultureFeed_Cdb_PhysicalAddress $address
   *   Address to set.
   */
  public function setPhysicalAddress(CultureFeed_Cdb_PhysicalAddress $address) {
    $this->physicalAddress = $address;
  }

  /**
   * Set the virtual address.
   * @param CultureFeed_Cdb_VirtualAddress $address
   *   Address to set.
   */
  public function setVirtualAddress(CultureFeed_Cdb_VirtualAddress $address) {
    $this->virtualAddress = $address;
  }

  /**
   * @see ICultureFeed_Cdb_Element::appendToDOM()
   */
  public function appendToDOM(DOMELement $element) {

    $dom = $element->ownerDocument;

    $addressElement = $dom->createElement('address');
    $element->appendChild($addressElement);

    if ($this->physicalAddress) {
      $this->physicalAddress->appendToDOM($addressElement);
    }

    if ($this->virtualAddress) {
      $this->physicalAddress->appendToDOM($addressElement);
    }

  }

  /**
   * @see ICultureFeed_Cdb_Element::parseFromCdbXml($xmlElement)
   * @return CultureFeed_Cdb_Address
   */
  public static function parseFromCdbXml($xmlElement) {

    $physicalAddress = CultureFeed_Cdb_PhysicalAddress::parseFromCdbXml($xmlElement->physical_address);
    $address = new CultureFeed_Cdb_Address($physicalAddress);

    return $address;

  }

}
