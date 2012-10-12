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
}
