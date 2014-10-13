<?php

/**
 * Class to represent a location's coordinates (latitude and longitude).
 */
class CultureFeed_Location {

  /**
   * Latitude.
   *
   * @var float
   */
  public $lat;

  /**
   * Longitude
   *
   * @var float
   */
  public $lng;

  /**
   * Radius
   *
   * @var integer
   */
  public $radius;

  /**
   * Constructor for a new CultureFeed_Location instance.
   *
   * @param float $lat
   *   Latitude.
   * @param float $lng
   *   Longitude.
   * @param integer $radius
   *   (optional) Radius.
   */
  public function __construct($lat, $lng, $radius = NULL) {
    $this->lat = $lat;
    $this->lng = $lng;
    $this->radius = $radius;
  }

  /**
   * Implements PHP magic __toString method to convert the CultureFeed_Location to a string.
   *
   * @return string
   *   The string representation of the location in lat;lng format.
   */
  public function __toString() {
    if (!$this->radius) {
      return sprintf('%f;%f', $this->lat, $this->lng);
    }
    else {
      return sprintf('%f;%f!%d', $this->lat, $this->lng, $this->radius);
    }
  }

}

