<?php
/**
 * @file
 */

/**
 * Class representing a period constraint.
 */
class CultureFeed_PeriodConstraint {

  /**
   * @var integer
   */
  public $periodVolume;

  /**
   * @var string
   */
  public $periodType;

  /**
   * Constructor which creates a valid object.
   * 
   * @param string $periodType
   * @param integer $periodVolume
   */
  public function __construct($periodType, $periodVolume) {
    $this->periodType = $periodType;
    $this->periodVolume = $periodVolume;
  }

}