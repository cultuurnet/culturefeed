<?php

class CultureFeed_Uitpas_Passholder_Query_WelcomeAdvantagesOptions extends CultureFeed_Uitpas_ValueObject {

  /**
   * The UitPas number
   *
   * @var string
   */
  public $uitpas_number;

  /**
   * The consumer key of the counter from where the request originates
   *
   * @var string
   */
  public $balieConsumerKey;

  /**
   * Begin date of the cashing period
   *
   * @var integer
   */
  public $cashingPeriodBegin;

  /**
   * End date of the cashing period
   *
   * @var integer
   */
  public $cashingPeriodEnd;

  /**
   * Consumer key of the counter where the welcome advantages can be cashed in
   *
   * @var string
   */
  public $cashInBalieConsumerKey;

  /**
   * Returns the welcome advantages that have been cashed in (true), or the ones that haven't been cashed in.
   * If unset, both types of welcome advantages are being returned.
   *
   * @var boolean
   */
  public $cashedIn;

  protected function manipulatePostData(&$data) {
    if (isset($data['cashingPeriodBegin'])) {
      $data['cashingPeriodBegin'] = date('c', $data['cashingPeriodBegin']);
    }

    if (isset($data['cashingPeriodEnd'])) {
      $data['cashingPeriodEnd'] = date('c', $data['cashingPeriodEnd']);
    }
  }

}