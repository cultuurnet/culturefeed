<?php

class CultureFeed_Uitpas_Passholder_Membership extends CultureFeed_Uitpas_ValueObject {

  /**
   * The association the passholder is linked
   *
   * @var string
   */
  public $associationId;

  /**
   * The membership's organization end date. (Required)
   *
   * @var integer
   */
  public $endDate;
  
  public $uid;
  
  public $balieConsumerKey;

  protected function manipulatePostData(&$data) {
    if (isset($data['endDate'])) {
      $data['endDate'] = date('Y-m-d', $data['endDate']);
    }
  }

}