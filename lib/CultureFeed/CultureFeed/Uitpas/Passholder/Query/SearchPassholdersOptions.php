<?php

class CultureFeed_Uitpas_Passholder_Query_SearchPassholdersOptions extends CultureFeed_Uitpas_ValueObject {

  const SORT_CREATION_DATE = "creationDate";
  const SORT_UITPAS_NUMBER = "uitpasNumber";
  const SORT_FIRST_NAME = "firstName";
  const SORT_NAME = "name";
  const SORT_NUMBER_OF_CHECK_INS = "numberOfCheckins";
  const SORT_DOB = "dob";

  const ORDER_ASC = "ASC";
  const ORDER_DESC = "DESC";

  /**
   * The INSZ number of the passholder.
   *
   * @var string
   */
  public $inszNumber;

  /**
   * The UitPas number of the passholder.
   *
   * @var string
   */
  public $uitpasNumber;

  /**
   * The name of the passholder.
   *
   * @var string
   */
  public $name;

  /**
   * The first name of the passholder.
   *
   * @var string
   */
  public $firstName;

  /**
   * The street of the passholder.
   *
   * @var string
   */
  public $street;

  /**
   * The postal code of the passholder.
   *
   * @var string
   */
  public $postalCode;

  /**
   * The city of the passholder.
   *
   * @var string
   */
  public $city;

   /**
   * The e-mail of the passholder.
   *
   * @var string
   */
  public $email;

  /**
   * The date of birth of the passholder.
   *
   * @var integer
   */
  public $dob;

  /**
   * The minimum date of birth of the passholder.
   *
   * @var integer
   */
  public $dobMin;

  /**
   * The maximum date of birth of the passholder.
   *
   * @var integer
   */
  public $dobMax;

  /**
   * True if the passholder has a kansenstatuut.
   *
   * @var boolean
   */
  public $kansenStatuut;

  /**
   * W3CDate search pass holders that have a "kansenstatuut" after this date.
   *
   * @var integer
   */
  public $kansenStatuutBegin;

  /**
   * W3CDate search pass holders that have a "kansenstatuut" before this date.
   *
   * @var integer
   */
  public $kansenStatuutEnd;



  /**
   * The number of checkins
   *
   * @var int
   */
  public $numberOfCheckins;

  /**
   * The minimum number of checkins
   *
   * @var int
   */
  public $numberOfCheckinsMin;

  /**
   * The maximum number of checkins
   *
   * @var int
   */
  public $numberOfCheckinsMax;

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
  public $order;

  /**
   * Maximum number of results.
   *
   * @var integer
   */
  public $max;

  /**
   * Results offset.
   *
   * @var integer
   */
  public $start;

  /**
   * The consumer key of the counter from where the request originates
   *
   * @var string
   */
  public $balieConsumerKey;


  /**
   * The consumerKey van de school van de gezochte pashouders.
   *
   * @var string
   */
  public $schoolConsumerKey;

  /**
   * Include blocked users true. default = false.
   *
   * @var boolean
   */
  public $includeBlocked;



  protected function manipulatePostData(&$data) {
    if (isset($data['dob'])) {
      $data['dob'] = date('Y-m-d', $data['dob']);
    }

    if (isset($data['dobMin'])) {
      $data['dobMin'] = date('Y-m-d', $data['dobMin']);
    }

    if (isset($data['dobMax'])) {
      $data['dobMax'] = date('Y-m-d', $data['dobMax']);
    }
    if (isset($data['includeBlocked'])) {
      if ($data['includeBlocked']) {
        $data['includeBlocked'] = "true";
      }
      else {
        $data['includeBlocked'] = "false";
      }
    }

    if (isset($data['kansenStatuut'])) {
      $data['kansenStatuut'] = $data['kansenStatuut'] ? 'true' : 'false';
    }

    if (isset($data['kansenStatuutBegin'])) {
      $data['kansenStatuutBegin'] = date('Y-m-d', $data['kansenStatuutBegin']);
    }

    if (isset($data['kansenStatuutEnd'])) {
      $data['kansenStatuutEnd'] = date('Y-m-d', $data['kansenStatuutEnd']);
    }

  }

}
