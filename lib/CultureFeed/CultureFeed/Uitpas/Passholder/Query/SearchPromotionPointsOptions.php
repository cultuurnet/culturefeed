<?php

class CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions extends CultureFeed_Uitpas_ValueObject {

  const SORT_CREATION_DATE = "CREATION_DATE";
  const SORT_TITLE = "TITLE";
  const SORT_CASHING_PERIOD_END = "CASHING_PERIOD_END";
  const SORT_POINTS = "POINTS";

  const ORDER_ASC = "ASC";
  const ORDER_DESC = "DESC";

  const FILTER_POSSIBLE = "POSSIBLE";

  /**
   * List is cities where the pointspromotions must be valid. Possible values: Aalst, Lede, Haaltert, Erpe_Mere
   *
   * @var string
   */
  public $city;

  /**
   * Minimum number of points needed for the pointspromotion
   *
   * @var integer
   */
  public $minPoints;

  /**
   * Maximum number of points needed for the pointspromotion
   *
   * @var integer
   */
  public $maxPoints;

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
   * The user ID of the passholder
   *
   * @var string
   */
  public $uid;

  /**
   * The number of the uitpas of the passholder
   *
   * @var string
   */
  public $uitpasNumber;

  /**
   * Default: false. Indicates if the system must only show the pointspromotions for which the user has sufficient points
   *
   * @var boolean
   */
  public $filterOnUserPoints = false;

  /**
   * Can be used to see which pointspromotions are possible if the user had e.g. 5 extra points.
   *
   * @var integer
   */
  public $simulatedExtraPoints;
  
  /**
   * Default: false. Indicates that only unexpired points promotions are returned.
   *
   * @var boolean
   */
  public $unexpired = false;


  /**
   * Filter based on cashInState NOT_POSSIBLE_DATE_CONSTRAINT
   *
   * @var string POSSIBLE or NOT_POSSIBLE_DATE_CONSTRAINT or ...
   */
  public $cashInState = self::FILTER_POSSIBLE;




  protected function manipulatePostData(&$data) {
    if (isset($data['cashingPeriodBegin'])) {
      $data['cashingPeriodBegin'] = date('c', $data['cashingPeriodBegin']);
    }

    if (isset($data['cashingPeriodEnd'])) {
      $data['cashingPeriodEnd'] = date('c', $data['cashingPeriodEnd']);
    }

    if (isset($data['grantingPeriodBegin'])) {
      $data['grantingPeriodBegin'] = date('c', $data['grantingPeriodBegin']);
    }

    if (isset($data['grantingPeriodEnd'])) {
      $data['grantingPeriodEnd'] = date('c', $data['grantingPeriodEnd']);
    }
  }

}
