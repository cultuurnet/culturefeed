<?php

/**
 * @class
 * Representation of a exceptions element in the cdb xml.
 */
class CultureFeed_Cdb_Data_Calendar_Exceptions implements CultureFeed_Cdb_IElement, Iterator {

  /**
   * Current position in the list.
   * @var int
   */
  protected $position = 0;

  /**
   * The list of items.
   * @var array
   */
  protected $items = array();

  /**
   * Add a new exception to the list.
   * @param CultureFeed_Cdb_Data_Calendar_Timestamp $timestamp
   *   Timestamp to add.
   */
  public function add(CultureFeed_Cdb_Data_Calendar_Timestamp $timestamp) {
    $this->items[] = $timestamp;
  }

  /**
   * @see Iterator::rewind()
   */
  function rewind() {
    $this->position = 0;
  }

  /**
   * @see Iterator::current()
   */
  function current() {
    return $this->items[$this->position];
  }

  /**
   * @see Iterator::key()
   */
  function key() {
    return $this->position;
  }

  /**
   * @see Iterator::next()
   */
  function next() {
    ++$this->position;
  }

  /**
   * @see Iterator::valid()
   */
  function valid() {
    return isset($this->items[$this->position]);
  }

  /**
   * @see CultureFeed_Cdb_IElement::appendToDOM()
   */
  public function appendToDOM(DOMELement $element) {

    $dom = $element->ownerDocument;

    $exceptionsElement = $dom->createElement('exceptions');
    foreach ($this as $timestamp) {
      $timestamp->appendToDom($exceptionsElement);
    }

    $element->appendChild($exceptionsElement);

  }

  /**
   * @see CultureFeed_Cdb_IElement::parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement)
   * @return CultureFeed_Cdb_Data_Calendar_Exceptions
   */
  public static function parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement) {

    $exceptions = new CultureFeed_Cdb_Data_Calendar_Exceptions();

    foreach ($xmlElement as $exceptionElement) {
      $exceptions->add(CultureFeed_Cdb_Data_Calendar_Timestamp::parseFromCdbXml($exceptionElement));
    }

    return $exceptions;

  }

}
