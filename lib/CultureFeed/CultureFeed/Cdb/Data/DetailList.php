<?php

/**
 * @class
 * Representation of a list of details in the cdb xml.
 */
abstract class CultureFeed_Cdb_Data_DetailList implements CultureFeed_Cdb_IElement, Iterator {

  /**
   * Current position in the list.
   * @var int
   */
  protected $position = 0;

  /**
   * The list of details.
   * @var array
   */
  protected $details = array();

  /**
   * Add a new detail to the list.
   * @param CultureFeed_Cdb_Data_Detail $detail
   *   Detail to add.
   */
  public function add(CultureFeed_Cdb_Data_Detail $detail) {
    $this->details[] = $detail;
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
    return $this->details[$this->position];
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
    return isset($this->details[$this->position]);
  }

}
