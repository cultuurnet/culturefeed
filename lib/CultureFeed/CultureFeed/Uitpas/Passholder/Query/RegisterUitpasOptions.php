<?php

class CultureFeed_Uitpas_Passholder_Query_RegisterUitpasOptions extends CultureFeed_Uitpas_ValueObject {

  const REASON_LOSS_THEFT = 'LOSS_THEFT';
  const REASON_REMOVAL = 'REMOVAL';
  const REASON_LOSS_KANSENSTATUUT = 'LOSS_KANSENSTATUUT';
  const REASON_OBTAIN_KANSENSTATUUT = 'OBTAIN_KANSENSTATUUT';

  /**
   * The identification of the passholder
   *
   * @var unknown_type
   */
  public $uid;

  /**
   * The reason for the registration
   *
   * @var string
   */
  public $reason;

  /**
   * The uitpas number
   *
   * @var string
   */
  public $uitpasNumber;

  /**
   * Consumer key of the counter
   *
   * @var string
   */
  public $balieConsumerKey;

  /**
   * The price of the uitpas
   *
   * @var string
   */
  public $price;

  /**
   * End date of the cashing period
   *
   * @var integer
   */
  public $kansenStatuutEndDate;

  protected function manipulatePostData(&$data) {
    if (isset($data['kansenStatuutEndDate'])) {
      $data['kansenStatuutEndDate'] = date('c', $data['kansenStatuutEndDate']);
    }
  }

}