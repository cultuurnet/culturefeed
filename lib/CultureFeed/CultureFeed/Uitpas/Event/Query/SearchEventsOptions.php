<?php

class CultureFeed_Uitpas_Event_Query_SearchEventsOptions extends CultureFeed_Uitpas_ValueObject {

  const SORT_CREATION_DATE = "CREATION_DATE";
  const SORT_TITLE = "TITLE";
  const SORT_CASHING_PERIOD_END = "CASHING_PERIOD_END";
  const SORT_POINTS = "POINTS";

  const ORDER_ASC = "ASC";
  const ORDER_DESC = "DESC";

  /**
   * Consumer key of the counter
   *
   * @var string
   */
  public $balieConsumerKey;

  /**
   * Begin date
   *
   * @var integer
   */
  public $startDate;

  /**
   * End date
   *
   * @var integer
   */
  public $endDate;

  /**
   * Search for events with a given location ID
   *
   * @var string
   */
  public $locatieId;

  /**
   * Search for events with a given inrichter ID
   *
   * @var string
   */
  public $inrichterId;

  /**
   * Search for events with a given city
   *
   * @var string
   */
  public $city;

  /**
   * Sort field.
   *
   * @var string
   */
  public $sortField;

  /**
   * Sort direction
   *
   * @var string
   */
  public $sortOrder = self::ORDER_DESC;

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
   * The Uitpas of the passholder
   *
   * @var string
   */
  public $uitpasNumber;

  /**
   * A search term
   *
   * @var string
   */
  public $q;

  /**
   * The CDBID of the event
   *
   * @var string
   */
  public $cdbid;

  protected function manipulatePostData(&$data) {
    if (isset($data['startDate']) && is_integer($data['startDate'])) {
      $data['startDate'] = date('Y-m-d', $data['startDate']);
    }

    if (isset($data['endDate']) && is_integer($data['endDate'])) {
      $data['endDate'] = date('Y-m-d', $data['endDate']);
    }

    if (isset($data['basicSearch']) && $data['basicSearch']) {
      $data['basicSearch'] = 'true';
    }
    if (isset($data['basicSearch']) && ! $data['basicSearch']) {
      $data['basicSearch'] = 'false';
    }

  }

  /**
   * Read the data from a array and set the variables
   */
  public function readValues($values) {
    foreach($values as $k => $v) {
      $this->$k = $v;
    }

    if (preg_match( "/^[0-9[0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]$/", $this->startDate)) {
      $this->startDate = strtotime($this->startDate);
    }

    if (preg_match("/^[0-9[0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]$/", $this->endDate)) {
      $this->endDate = strtotime($this->endDate);
    }
  }

  /**
   * Read from a querystring
   */
   public function readQueryString( $str ) {
     $values = array();
     parse_str(urldecode($str) , $values);
     $this->readValues( $values );
   }
}
