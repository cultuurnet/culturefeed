<?php

/**
 * @class
 * Representation of geo information in the cdb xml.
 */
class CultureFeed_Cdb_GeoInformation implements ICultureFeed_Cdb_Element {

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
   * @see ICultureFeed_Cdb_Element::appendToDOM()
   */
  public function appendToDOM(DOMElement $element) {

    $dom = $element->ownerDocument;

    $gisElement = $dom->createElement('gis');
    $gisElement->appendChild($dom->createElement('xcoordinate', $this->xCoordinate));
    $gisElement->appendChild($dom->createElement('ycoordinate', $this->yCoordinate));

    $element->appendChild($gisElement);

  }

  /**
   * @see ICultureFeed_Cdb_Element::parseFromCdbXml($xmlElement)
   * @return CultureFeed_Cdb_GeoInformation
   */
  public static function parseFromCdbXml($xmlElement) {

  }

}
