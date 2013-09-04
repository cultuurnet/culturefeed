<?php

/**
 * Class to represent a mailing search query.
 */
class CultureFeed_SearchMailingsQuery {

  /**
   * Start position.
   *
   * @var integer
   */
  public $start;

  /**
   * Maximum number of results to return.
   *
   * @var integer
   */
  public $max;

  /**
   * Consumer key to filter on.
   * @var string
   */
  public $consumerKey;

  /**
   * Convert a CultureFeed_SearchMailingsQuery object to an array that can be used as data in POST requests that expect search user query info.
   *
   * @return array
   *   Associative array representing the object. For documentation of the structure, check the Culture Feed API documentation.
   */
  public function toPostData() {

    // For most properties we can rely on get_object_vars.
    $data = get_object_vars($this);
    $data = array_filter($data);

    return $data;
  }

}