<?php

/**
 * Class to represent a recommendations query.
 */
class CultureFeed_RecommendationsQuery {

  /**
   * Category of event.
   *
   * @var string
   */
  public $category;

  /**
   * Minimum score for the recommendation.
   *
   * @var integer
   */
  public $scoreMin;

  /**
   * Coordinates of the event. Radius search possible.
   *
   * @var CultureFeed_Location
   */
  public $location;

  /**
   * Address of the location of the event.
   *
   * @var string
   */
  public $location_simple;

  /**
   * City of the location of the event.
   *
   * @var string
   */
  public $location_city;

  /**
   * Zip code.
   *
   * @var string|array
   */
  public $zipcode;

  /**
   * Tag(s) of the event.
   *
   * @var string|array
   */
  public $tag;

  /**
   * Maximum number of results to return.
   *
   * @var integer
   */
  public $max;

  /**
   * Convert a CultureFeed_RecommendationsQuery object to an array that can be used as data in POST requests that expect search recommendations query info.
   *
   * @return array
   *   Associative array representing the object. For documentation of the structure, check the Culture Feed API documentation.
   */
  public function toPostData() {
    // For most properties we can rely on get_object_vars.
    $data = get_object_vars($this);

    // Represent location as a string.
    if (isset($data['location'])) {
      $data['location'] = (string)$data['location'];
    }

    $data = array_filter($data);

    return $data;
  }

}

