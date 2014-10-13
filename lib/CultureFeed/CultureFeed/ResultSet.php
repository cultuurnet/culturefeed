<?php

/**
 * Class to represent a result set (results + total number of results).
 */
class CultureFeed_ResultSet {

  /**
   * The total number of objects in the complete set.
   *
   * The result set holds a slice of the complete set.
   *
   * @var int
   */
  public $total;

  /**
   * The objects in the slice.
   *
   * @var array
   */
  public $objects;

  /**
   * Constructor for a new CultureFeed_ResultSet instance.
   *
   * @param integer $total
   *   The total number of objects in the complete set.
   * @param array $objects
   *   The objects in the slice.
   */
  public function __construct($total = 0, $objects = array()) {
    $this->total = $total;
    $this->objects = $objects;
  }

}

