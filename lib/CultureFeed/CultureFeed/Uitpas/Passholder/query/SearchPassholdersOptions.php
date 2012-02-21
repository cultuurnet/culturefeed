<?php

class CultureFeed_Uitpas_Passholder_Query_SearchPassholdersOptions extends CultureFeed_Uitpas_ValueObject {

  const SORT_CREATION_DATE = "creationDate";
  const SORT_UITPAS_NUMBER = "uitpasNumber";
  const SORT_FIRST_NAME = "firstName";
  const SORT_NAME = "name";
  const SORT_NUMBER_OF_CHECK_INS = "numberOfCheckins";

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

  protected function manipulatePostData(&$data) {
    if (isset($data['dob'])) {
      $data['dob'] = date('c', $data['dob']);
    }

    if (isset($data['dobMin'])) {
      $data['dobMin'] = date('c', $data['dobMin']);
    }

    if (isset($data['dobMax'])) {
      $data['dobMax'] = date('c', $data['dobMax']);
    }

  }

}