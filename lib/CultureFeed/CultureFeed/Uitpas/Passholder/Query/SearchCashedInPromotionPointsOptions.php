<?php

class CultureFeed_Uitpas_Passholder_Query_SearchCashedInPromotionPointsOptions extends CultureFeed_Uitpas_ValueObject {

  const SORT_TITLE = "TITLE";
  const SORT_BALIE = "BALIE";
  const SORT_CITY = "CITY";
  const SORT_CASHING_DATE = "CASHINGDATE";

  const ORDER_ASC = "ASC";
  const ORDER_DESC = "DESC";

  /**
   * The UitPas number of the passholder.
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
   * Sort field.
   *
   * @var string
   */
  public $sort = self::SORT_CASHING_DATE;

  /**
   * Sort direction
   *
   * @var string
   */
  public $order = self::ORDER_DESC;

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

  /**
   * Can be used to see which pointspromotions are possible if the user had e.g. 5 extra points.
   *
   * @var integer
   */
  public $simulatedExtraPoints;

  protected function manipulatePostData(&$data) {
    if (isset($data['cashingPeriodBegin'])) {
      $data['cashingPeriodBegin'] = date('c', $data['cashingPeriodBegin']);
    }

    if (isset($data['cashingPeriodEnd'])) {
      $data['cashingPeriodEnd'] = date('c', $data['cashingPeriodEnd']);
    }
  }

}