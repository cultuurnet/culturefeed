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

  public function __construct() {
    $this->activityPrivacyPreferences = array();
  }
}