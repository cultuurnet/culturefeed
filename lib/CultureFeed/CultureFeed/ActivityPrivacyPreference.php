<?php
/**
 * @file
 */

/**
 * Class representing an activity privacy preference.
 */
class CultureFeed_ActivityPrivacyPreference {

  public function __construct($activityType = NULL, $private = TRUE) {
    $this->activityType = $activityType;
    $this->private = $private;
  }

  /*
   * @var integer
   */
  public $activityType;

  /**
   * @var boolean
   */
  public $private;


}