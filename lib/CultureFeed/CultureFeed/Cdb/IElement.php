<?php

/**
 * Interface for a cdb element.
 */
interface CultureFeed_Cdb_IElement {

  /**
   * Appends the current object to the passed DOM tree.
   *
   * @param DOMElement $element
   *   The DOM tree to append to.
   */
  public function appendToDOM(DOMElement $element);

  /**
   * Parse a new object from a given cdbxml element.
   * @param CultureFeed_SimpleXMLElement $xmlElement
   *   XML to parse.
   * @throws CultureFeed_ParseException
   */
  public static function parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement);

}
