<?php

/**
 * @class
 * Representation of geo information in the cdb xml.
 */
class CultureFeed_Cdb_Data_GeoInformation implements CultureFeed_Cdb_IElement {

  /**
   * X coördinate from the location.
   * @var decimal
   */
  protected $xCoordinate;

  /**
   * Y coördinate from the location.
   * @var decimal
   */
  protected $yCoordinate;

  /**
   * Construct the geo information.
   * @param string $xCoordinate
   *   X coördinate from the location.
   * @param string $yCoordinate
   *   Y coördinate from the location.
   */
  public function __construct($xCoordinate, $yCoordinate) {
    $this->xCoordinate = $xCoordinate;
    $this->yCoordinate = $yCoordinate;
  }

  /**
   * Set the x coordinate.
   * @param string $coordinate
   *   Coördinate to set.
   */
  public function setXCoordinate($coordinate) {
    $this->xCoordinate = $coordinate;
  }

  /**
   * Set the x coordinate.
   * @param string $coordinate
   *   Coördinate to set.
   */
  public function setYCoordinate($coordinate) {
    $this->yCoordinate = $coordinate;
  }

  /**
   * Get the x coördinate.
   */
  public function getXCoordinate() {
    return $this->xCoordinate;
  }

  /**
   * Get the y coördinate.
   */
  public function getYCoordinate() {
    return $this->YCoordinate;
  }

  /**
   * @see CultureFeed_Cdb_IElement::appendToDOM()
   */
  public function appendToDOM(DOMElement $element) {

    $dom = $element->ownerDocument;

    $gisElement = $dom->createElement('gis');
    $gisElement->appendChild($dom->createElement('xcoordinate', $this->xCoordinate));
    $gisElement->appendChild($dom->createElement('ycoordinate', $this->yCoordinate));

    $element->appendChild($gisElement);

  }

  /**
   * @see CultureFeed_Cdb_IElement::parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement)
   * @return CultureFeed_Cdb_Data_GeoInformation
   */
  public static function parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement) {

    if (empty($xmlElement->xcoordinate) || empty($xmlElement->ycoordinate)) {
      throw new CultureFeed_ParseException("Coördinates are missing on gis element");
    }

    return new CultureFeed_Cdb_Data_GeoInformation((string)$xmlElement->xcoordinate, (string)$xmlElement->ycoordinate);

  }

}
