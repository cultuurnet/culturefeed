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

  /**
<<<<<<< HEAD
   * Id of a CardSystem owning the welcome advantages.
   * @var string
   */
  public $owningCardSystemId;

  /**
   * Filters the welcome advantages based on their spotlight status.
   * @var boolean
   */
  public $inSpotlight;

  /**
   * Id of a CardSystem to which the welcome advantages are applicable.
   * @var string
   */
  public $applicableCardSystemId;

  /** Maximum number of results. Default: 20
   *
   * @var integer
   */
  public $max = 20;

  /**
   * Results offset. Default: 0
   *
   * @var integer
   */
  public $start = 0;

  protected function manipulatePostData(&$data) {
    if (isset($data['cashingPeriodBegin'])) {
      $data['cashingPeriodBegin'] = date('c', $data['cashingPeriodBegin']);
    }

    if (isset($data['cashingPeriodEnd'])) {
      $data['cashingPeriodEnd'] = date('c', $data['cashingPeriodEnd']);
    }

    if (isset($data['cashedIn'])) {
      $data['cashedIn'] = $data['cashedIn'] ? 'true' : 'false';
    }

    if (isset($data['inSpotlight'])) {
      $data['inSpotlight'] = $data['inSpotlight'] ? 'true' : 'false';
    }
  }
}
