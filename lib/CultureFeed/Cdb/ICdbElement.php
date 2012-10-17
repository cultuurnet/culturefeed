<?php

/**
 * Interface for a cdb element.
 */
interface ICultureFeed_Cdb_Element {

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
   */
  public static function parseFromCdbXml($xmlElement);

}
