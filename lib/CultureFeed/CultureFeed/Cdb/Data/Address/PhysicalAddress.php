<?php

/**
 * @class
 * Representation of a physical address element in the cdb xml.
 */
class CultureFeed_Cdb_Data_PhysicalAddress implements CultureFeed_Cdb_IElement {

  /**
   * Street from the address.
   * @var string
   */
  protected $street;

  /**
   * House number from the address.
   * @var string
   */
  protected $houseNumber;

  /**
   * City from the address.
   * @var string
   */
  protected $city;

  /**
   * Zipcode from the address.
   * @var string
   */
  protected $zip;

  /**
   * Country from the address.
   * @var string
   */
  protected $country;

  /**
   * Geo information from the address.
   * @var CultureFeed_Cdb_GeoInformation
   */
  protected $gis;

  /**
   * Get the street.
   */
  public function getStreet() {
    return $this->street;
  }

  /**
   * Get the housenumber.
   */
  public function getHouseNumber() {
    return $this->houseNumber;
  }

  /**
   * Get the city.
   */
  public function getCity() {
    return $this->city;
  }

  /**
   * Get the zip code.
   */
  public function getZip() {
    return $this->zip;
  }

  /**
   * Get the country.
   */
  public function getCountry() {
    return $this->country;
  }

  /**
   * Get the geo information.
   */
  public function getGeoInformation() {
    return $this->gis;
  }

 /**
   * Set the street.
   * @param string $street
   *   Street to set
   */
  public function setStreet($street) {
    $this->street = $street;
  }

  /**
   * Set the housenumber.
   * @param string $housenumber
   *   Housenumber to set.
   */
  public function setHouseNumber($houseNumber) {
    $this->houseNumber = $houseNumber;
  }

  /**
   * Set the city
   * @param string $city
   *   City to set
   */
  public function setCity($city) {
    $this->city = $city;
  }

  /**
   * Set the zip code.
   * @param string $zip
   *   Zip code to set.
   */
  public function setZip($zip) {
    $this->zip = $zip;
  }

  /**
   * Set the country.
   * @param string $country
   *   Country to set.
   */
  public function setCountry($country) {
    $this->country = $country;
  }

  /**
   * Set the geo information.
   * @param CultureFeed_Cdb_Data_GeoInformation $gis
   *   Geo information to set.
   */
  public function setGeoInformation(CultureFeed_Cdb_Data_GeoInformation $gis) {
    $this->gis = $gis;
  }

  /**
   * @see CultureFeed_Cdb_IElement::appendToDOM()
   */
  public function appendToDOM(DOMElement $element) {

    $dom = $element->ownerDocument;

    $physicalElement = $dom->createElement('physical');
    $physicalElement->appendChild($dom->createElement('city', $this->city));
    $physicalElement->appendChild($dom->createElement('country', $this->country));
    if ($this->gis) {
      $this->gis->appendToDOM($physicalElement);
    }

    $physicalElement->appendChild($dom->createElement('housenr', $this->houseNumber));
    $physicalElement->appendChild($dom->createElement('street', $this->street));
    $physicalElement->appendChild($dom->createElement('zipcode', $this->zip));

    $element->appendChild($physicalElement);

  }

  /**
   * @see CultureFeed_Cdb_IElement::parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement)
   * @return CultureFeed_Cdb_Data_PhysicalAddress
   */
  public static function parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement) {

    if (empty($xmlElement->city)) {
      throw new CultureFeed_ParseException("City is missing for physical address");
    }

    if (empty($xmlElement->country)) {
      throw new CultureFeed_ParseException("Country is missing for physical address");
    }

    if (empty($xmlElement->street)) {
      throw new CultureFeed_ParseException("Street is missing for physical address");
    }

    if (empty($xmlElement->zipcode)) {
      throw new CultureFeed_ParseException("Zip code is missing for physical address");
    }

    $physicalAddress = new CultureFeed_Cdb_Data_PhysicalAddress();
    $physicalAddress->setCity((string)$xmlElement->city);
    $physicalAddress->setCountry((string)$xmlElement->country);
    $physicalAddress->setStreet((string)$xmlElement->street);
    $physicalAddress->setZip((string)$xmlElement->zipcode);
    
    if (!empty($xmlElement->housenr)) {
      $physicalAddress->setHouseNumber((string)$xmlElement->housenr);
    }

    if (!empty($xmlElement->gis)) {
      $physicalAddress->setGeoInformation(CultureFeed_Cdb_Data_GeoInformation::parseFromCdbXml($xmlElement->gis));
    }

    return $physicalAddress;

  }

}
