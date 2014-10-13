<?php
/**
 * @file
 */

/**
 * Class respresenting user preferences.
 */
class CultureFeed_Preferences {

  /**
   *
   * @var string
   */
  public $uid;

  /**
   *
   * @var array
   */
  public $activityPrivacyPreferences;

  /**
   * Constructor
   * Sets activity privacy preference by default.
   */
  public function __construct() {
    $this->activityPrivacyPreferences = array();
  }
}