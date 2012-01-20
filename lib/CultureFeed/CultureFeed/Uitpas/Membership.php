<?php

class CultureFeed_Uitpas_Membership extends CultureFeed_Uitpas_ValueObject {

  /**
   * The user ID of the passholder. (Required)
   *
   * @var string
   */
  public $uid;

  /**
   * The name of the organization. (Required)
   *
   * @var string
   */
  public $organizationName;

  /**
   * The membership's organization end date. (Required)
   *
   * @var integer
   */
  public $endDate;

  protected function manipulatePostData(&$data) {
    if (isset($data['endDate'])) {
      $data['endDate'] = date('c', $data['endDate']);
    }
  }

}