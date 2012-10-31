<?php

/**
 * @class
 * Representation of a location element in the cdb xml.
 */
class CultureFeed_Cdb_Data_Location implements CultureFeed_Cdb_IElement {

  /**
   * Address from the location.
   * @var CultureFeed_Cdb_Data_Address
   */
  protected $address;

  /**
   * Location label.
   * @var string
   */
  protected $label;

  /**
   * Cdbid from location actor.
   */
  protected $cdbid;

  /**
   * Location actor.
   * @var
   */
  protected $actor;

  /**
   * Construct a new location.
   * @param CultureFeed_Cdb_Data_Address $address
   *   Address from the location.
   */
  public function __construct(CultureFeed_Cdb_Data_Address $address) {
    $this->address = $address;
  }

  /**
   * Get the address.
   */
  public function getAddress() {
    return $this->address;
  }

  /**
   * Get the cdbid for this location.
   */
  public function getCdbid() {
    return $this->cdbid;
  }

  /**
   * Get the label.
   */
  public function getLabel() {
    return $this->label;
  }

  /**
   * Set the address.
   * @param CultureFeed_Cdb_Data_Address $address
   *   Address to set.
   */
  public function setAddress(CultureFeed_Cdb_Data_Address $address) {
    $this->address = $address;
  }

  /**
   * Set the cdbid for this location.
   * @param string $cdbid
   */
  public function setCdbid($cdbid) {
    $this->cdbid = $cdbid;
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
   * @see CultureFeed_Cdb_IElement::appendToDOM()
   */
  public function appendToDOM(DOMELement $element) {

    $dom = $element->ownerDocument;

    $locationElement = $dom->createElement('location');

    if ($this->address) {
      $this->address->appendToDOM($locationElement);
    }

    if ($this->label) {
      $labelElement = $dom->createElement('label', htmlentities($this->label));
      if ($this->cdbid) {
        $labelElement->setAttribute('cdbid', $this->cdbid);
      }
      $locationElement->appendChild($labelElement);
    }

    $element->appendChild($locationElement);

  }

  /**
   * @see CultureFeed_Cdb_IElement::parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement)
   * @return CultureFeed_Cdb_Data_Location
   */
  public static function parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement) {

    if (empty($xmlElement->address)) {
      throw new CultureFeed_ParseException("Address missing for location element");
    }

    $address = CultureFeed_Cdb_Data_Address::parseFromCdbXml($xmlElement->address);
    $location = new CultureFeed_Cdb_Data_Location($address);

    if (!empty($xmlElement->label)) {
      $attributes = $xmlElement->label->attributes();
      $location->setLabel((string)$xmlElement->label);
      if (isset($attributes['cdbid'])) {
        $location->setCdbid((string)$attributes['cdbid']);
      }
    }

    return $location;

  }

}
