<?php

class CultureFeed_Uitpas_Promotion_Query_WelcomeAdvantagesOptions extends CultureFeed_Uitpas_ValueObject {

  const SORT_CREATION_DATE = "CREATION_DATE";
  const SORT_TITLE = "TITLE";
  const SORT_CASHING_PERIOD_END = "CASHING_PERIOD_END";

  const ORDER_ASC = "ASC";
  const ORDER_DESC = "DESC";

  /**
   * Search for welcome advantages with a given city (one or more).
   *
   * @var string
   */
  public $city;

  /**
   * Search for welcome advantages with a given title
   *
   * @var string
   */
  public $title;

  /**
   * Consumer key of the counter
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
   * Begin date of the granting period
   *
   * @var integer
   */
  public $grantingPeriodBegin;

  /**
   * End date of the granting period
   *
   * @var integer
   */
  public $grantingPeriodEnd;

  /**
   * Sort field.
   *
   * @var string
   */
  public $sort = self::SORT_CREATION_DATE;

  /**
   * Sort direction
   *
   * @var string
   */
  public $order = self::ORDER_ASC;

  /**
   * Maximum number of results. Default: 20
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

    if (isset($data['grantPeriodBegin'])) {
      $data['grantPeriodBegin'] = date('c', $data['grantPeriodBegin']);
    }

    if (isset($data['grantPeriodEnd'])) {
      $data['grantPeriodBegin'] = date('c', $data['grantPeriodBegin']);
    }
  }

}