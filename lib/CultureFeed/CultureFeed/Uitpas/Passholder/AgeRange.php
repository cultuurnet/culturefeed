<?php
/**
 * @file
 */

class CultureFeed_Uitpas_Passholder_AgeRange {
  /**
   *
   * @var integer
   */
  public $ageFrom;

  /**
   * @var integer
   */
  public $ageTo;

  /**
   * Checks if a birthday is in the given age range.
   *
   * Source: http://www.phpro.org/examples/Calculate-Age-With-PHP.html
   *
   * @param DateTime $birthDate
   */
  public function inRange(DateTime $dateOfBirth) {
    $now = new DateTime();

    $age = $now->diff($dateOfBirth);
    $years = $age->y;

    $lowerApplies = !isset($this->ageFrom) || $years >= $this->ageFrom;
    $upperApplies = !isset($this->ageTo) || $years <= $this->ageTo;

    return $lowerApplies && $upperApplies;
  }
}