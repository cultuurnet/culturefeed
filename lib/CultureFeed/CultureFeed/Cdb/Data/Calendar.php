<?php

abstract class CultureFeed_Cdb_Data_Calendar implements Iterator {

  /**
   * Open type: the event is open.
   * @var string
   */
  const OPEN_TYPE_OPEN = 'open';

  /**
   * Open type: the event is closed.
   * @var string
   */
  const OPEN_TYPE_CLOSED = 'closed';

  /**
   * Open type: the event is appointment only.
   * @var string
   */
  const OPEN_TYPE_BYAPPOINTMENT = 'byappointment';

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
   * Validate a given date.
   * @param string $value
   *   Date to validate.
   *
   * @throws Exception
   */
  public static function validateDate($value) {
    if (!preg_match('/^[0-9]{4}-(0[1-9]|1[0-2])-(0[1-9]|[1-2][0-9]|3[0-1])$/', $value)) {
      throw new UnexpectedValueException('Invalid date: ' . $value);
    }
  }

  /**
   * Validate a given time.
   * @param string $value
   *   Time to validate.
   * @throws Exception
   */
  public static function validateTime($value) {
    if (!preg_match('/^([0-9]{2}):([0-9]{2}):([0-9]{2})$/', $value)) {
      throw new UnexpectedValueException('Invalid time: ' . $value);
    }
  }

}
