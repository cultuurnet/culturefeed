<?php

/**
 * Class to represent a user.
 */
class CultureFeed_User {

  /**
   * Gender 'male'.
   */
  const GENDER_MALE = 'male';

  /**
   * Gender 'female'.
   */
  const GENDER_FEMALE = 'female';

  /**
   * User status 'public'.
   */
  const STATUS_PUBLIC = 'public';

  /**
   * User status 'private'.
   */
  const STATUS_PRIVATE = 'private';

  /**
   * User status 'blocked'.
   */
  const STATUS_BLOCKED = 'blocked';

  /**
   * User status 'deleted'.
   */
  const STATUS_DELETED = 'deleted';

  /**
   * Lifestyle profile 'ontdekker'.
   */
  const LIFESTYLE_ONTDEKKER = 'ONT';

  /**
   * Lifestyle profile 'fijnproever'.
   */
  const LIFESTYLE_FIJNPROEVER = 'FP';

  /**
   * Lifestyle profile 'actieve ontspanner'.
   */
  const LIFESTYLE_ACTIEVE_ONTSPANNER = 'AO';

  /**
   * Lifestyle profile 'actiezoeker'.
   */
  const LIFESTYLE_ACTIE_ZOEKER = 'AZ';

  /**
   * ID of the user.
   *
   * @var string
   */
  public $id;

  /**
   * Nick of the user.
   *
   * @var string
   */
  public $nick;

  /**
   * Password of the user.
   *
   * @var string
   */
  public $password;

  /**
   * First name of the user.
   *
   * @var string
   */
  public $givenName;

  /**
   * Family name of the user.
   *
   * @var string
   */
  public $familyName;

  /**
   * E-mail of the user.
   *
   * @var string
   */
  public $mbox;

  /**
   * E-mail verification status.
   *
   * @var bool
   */
  public $mboxVerified;

  /**
   * Gender of the user.
   * Possible values are represented in the GENDER_* constants.
   *
   * @var string
   */
  public $gender;

  /**
   * Does the user have children
   * @var bool
   */
  public $hasChildren;

  /**
   * Date of birth of the user represented as a UNIX timestamp.
   *
   * @var integer
   */
  public $dob;

  /**
   * Depiction of the user.
   *
   * @var string
   */
  public $depiction;

  /**
   * Biography of the user.
   *
   * @var string
   */
  public $bio;

  /**
   * Street of the home address of the user.
   *
   * @var string
   */
  public $street;

  /**
   * Zip of the home address of the user.
   *
   * @var string
   */
  public $zip;

  /**
   * City of the home address of the user.
   *
   * @var string
   */
  public $city;

  /**
   * ISO 3166-1 alpha-2 code of the country of the home address of the user.
   *
   * @var string
   */
  public $country;

  /**
   * Lifestyle profile of the user.
   * @var string
   */
  public $lifestyleProfile;

  /**
   * Coordinates of the user's home address.
   *
   * @var CultureFeed_Location
   */
  public $homeLocation;

  /**
   * Coordinates of the user's current address.
   *
   * @var CultureFeed_Location
   */
  public $currentLocation;

  /**
   * Status of the user.
   * Possible values are represented in the STATUS_* constants.
   *
   * @var string
   */
  public $status;

  /**
   * OpenID handle of the user.
   *
   * @var string
   */
  public $openid;

  /**
   * Online accounts (social services) the user is connected with.
   * Represented as an array of CultureFeed_OnlineAccount objects.
   *
   * @var array
   */
  public $holdsAccount;



  /**
   * Field privacy status.
   *
   * @var CultureFeed_UserPrivacyConfig
   */
  public $privacyConfig;

  /**
   * Convert a CultureFeed_User object to an array that can be used as data in POST requests that expect user info.
   *
   * @return array
   *   Associative array representing the object. For documentation of the structure, check the Culture Feed API documentation.
   */
  public function toPostData($fields = array()) {
    // For most properties we can rely on get_object_vars.
    $data = get_object_vars($this);

    // Represent mboxVerified as a string (true/false);
    if (isset($data['mboxVerified'])) {
      $data['mboxVerified'] = $data['mboxVerified'] ? 'true' : 'false';
    }

    // Put street in the right field.
    if (isset($data['street'])) {
      $data['homeStreet'] = (string)$data['street'];
      unset($data['street']);
    }

    // Put zip in the right field.
    if (isset($data['zip'])) {
      $data['homeZip'] = (string)$data['zip'];
      unset($data['zip']);
    }

    // Put city in the right field.
    if (isset($data['city'])) {
      $data['homeCity'] = (string)$data['city'];
      unset($data['city']);
    }

    // Put country in the right field.
    if (isset($data['country'])) {
      $data['homeCountry'] = (string)$data['country'];
      unset($data['country']);
    }

    // Represent currentLocation as a string.
    if (isset($data['currentLocation'])) {
      $data['currentLocation'] = (string)$data['homeLocation'];
    }

    // Represent dob as a W3C date.
    if (!empty($data['dob'])) {
      $data['dob'] = date('c', $data['dob']);
    }

    if (isset($data['hasChildren'])) {
      $data['hasChildren'] = $data['hasChildren'] ? 'true' : 'false';
    }

    $data = array_filter($data);

    // If a field was unset (value made empty), re-add it.
    $valid_fields = array_keys(get_object_vars($this));
    foreach ($fields as $field) {
      if (!isset($data[$field]) && in_array($field, $valid_fields)) {
        $data[$field] = '';
      }
    }

    return $data;
  }

}

