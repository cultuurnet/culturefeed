<?php
/**
 * @file
 */

class CultureFeed_Uitpas_Promotion_PassholderParameter {

  public $uid;

  public $uitpasNumber;

  public function params() {
    $params = array();

    // uitid and uitpasNumber are mutually exclusive
    if ($this->uid) {
      $params['uid'] = $this->uid;
    }
    elseif ($this->uitpasNumber) {
      $params['uitpasNumber'] = $this->uitpasNumber;
    }

    return $params;
  }
}