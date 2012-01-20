<?php

class CultureFeed_Uitpas_Passholder extends CultureFeed_Uitpas_ValueObject {

  /**
   * The name of the passholder. (Required)
   *
   * @var string
   */
  public $name;

  /**
   * The first name of the passholder. (Required)
   *
   * @var string
   */
  public $firstName;

  /**
   * The e-mail of the passholder.
   *
   * @var string
   */
  public $email;

  /**
   * The INSZ number of the passholder. (Required)
   *
   * @var string
   */
  public $inszNumber;

  /**
   * The date of birth of the passholder. (Required)
   *
   * @var integer
   */
  public $dateOfBirth;

  /**
   * The gender of the passholder.
   *
   * @var string
   */
  public $gender;

  /**
   * The street of the passholder.
   *
   * @var string
   */
  public $street;

  /**
   * The number of the passholder.
   *
   * @var string
   */
  public $number;

  /**
   * The post box of the passholder.
   *
   * @var string
   */
  public $box;

  /**
   * The postal code of the passholder. (Required)
   *
   * @var string
   */
  public $postalCode;

  /**
   * The city of the passholder. (Required)
   *
   * @var string
   */
  public $city;

  /**
   * The telephone number of the passholder.
   *
   * @var string
   */
  public $telephone;

  /**
   * The nationality of the passholder.
   *
   * @var string
   */
  public $nationality;

  /**
   * The place of birth of the passholder.
   *
   * @var string
   */
  public $placeOfBirth;

  /**
   * The UitPas number of the passholder. (Required)
   *
   * @var string
   */
  public $uitpasNumber;

  /**
   * The price the passholder pays for his UitPas.
   *
   * @var string
   */
  public $price;

  /**
   * True if the passholder has a kansenstatuut.
   *
   * @var boolean
   */
  public $kansenStatuut;

  /**
   * End date kansenstatuut.
   *
   * @var integer
   */
  public $kansenStatuutValidUntil;

  /**
   * True if the data of the passholder was fetched using eID.
   *
   * @var boolean
   */
  public $verified;

  protected function manipulatePostData($data) {
    if (isset($data['dateOfBirth'])) {
      $data['dateOfBirth'] = date('c', $data['dateOfBirth']);
    }

    if (isset($data['kansenStatuutValidUntil'])) {
      $data['kansenStatuutValidUntil'] = date('c', $data['kansenStatuutValidUntil']);
    }
  }

}