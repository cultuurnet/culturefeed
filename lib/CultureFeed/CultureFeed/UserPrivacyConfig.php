<?php

/**
 * Class to represent a user privacy config.
 */
class CultureFeed_UserPrivacyConfig {

  /**
   * Privacy status 'public'.
   */
  const PRIVACY_PUBLIC = 'public';

  /**
   * Privacy status 'private'.
   */
  const PRIVACY_PRIVATE = 'private';

  /**
   * Privacy status of the user's nick.
   * Possible values are represented in the PUBLIC_* constants.
   *
   * @var string
   */
  public $nick;

  /**
   * Privacy status of the user's givenName.
   * Possible values are represented in the PUBLIC_* constants.
   *
   * @var string
   */
  public $givenName;

  /**
   * Privacy status of the user's familyName.
   * Possible values are represented in the PUBLIC_* constants.
   *
   * @var string
   */
  public $familyName;

  /**
   * Privacy status of the user's mbox.
   * Possible values are represented in the PUBLIC_* constants.
   *
   * @var string
   */
  public $mbox;

  /**
   * gender.
   * Possible values are represented in the PUBLIC_* constants.
   *
   * @var string
   */
  public $gender;

  /**
   * Privacy status of the user's dob.
   * Possible values are represented in the PUBLIC_* constants.
   *
   * @var string
   */
  public $dob;

  /**
   * Privacy status of the user's depiction.
   * Possible values are represented in the PUBLIC_* constants.
   *
   * @var string
   */
  public $depiction;

  /**
   * Privacy status of the user's bio.
   * Possible values are represented in the PUBLIC_* constants.
   *
   * @var string
   */
  public $bio;

  /**
   * Privacy status of the user's homeAddress.
   * Possible values are represented in the PUBLIC_* constants.
   *
   * @var string
   */
  public $homeAddress;

  /**
   * Privacy status of the user's homeLocation.
   * Possible values are represented in the PUBLIC_* constants.
   *
   * @var string
   */
  public $homeLocation;

  /**
   * Privacy status of the user's currentLocation..
   * Possible values are represented in the PUBLIC_* constants.
   *
   * @var string
   */
  public $currentLocation;

  /**
   * Privacy status of the user's openId.
   * Possible values are represented in the PUBLIC_* constants.
   *
   * @var string
   */
  public $openId;

  /**
   * Convert a CultureFeed_UserPrivacyConfig object to an array that can be used as data in POST requests that expect search user query info.
   *
   * @return array
   *   Associative array representing the object. For documentation of the structure, check the CultureFeed API documentation.
   */
  public function toPostData() {
    // For most properties we can rely on get_object_vars.
    $data = array_filter(get_object_vars($this));

    return $data;
  }

}