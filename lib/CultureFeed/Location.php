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
   * Constructor for a new CultureFeed_Location instance.
   *
   * @param float $lat
   *   Latitude.
   * @param float $lng
   *   Longitude.
   */
  public function __construct($lat, $lng) {
    $this->lat = $lat;
    $this->lng = $lng;
  }

  /**
   * Implements PHP magic __toString method to convert the CultureFeed_Location to a string.
   *
   * @return string
   *   The string representation of the location in lat;lng format.
   */
  public function __toString() {
    return sprintf('%f;%f', $this->lat, $this->lng);
  }

}

