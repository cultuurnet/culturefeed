<?php

/**
 * Class to represent a user search query.
 */
class CultureFeed_SearchUsersQuery {

  /**
   * Sort option 'creationDate'.
   */
  const SORT_CREATIONDATE = 'creationDate';

  /**
   * Sort option 'userId'.
   */
  const SORT_USERID = 'userId';

  /**
   * Sort option 'firstName'.
   */
  const SORT_FIRSTNAME = 'firstName';

  /**
   * Sort option 'lastName'.
   */
  const SORT_LASTNAME = 'lastName';

  /**
   * Sort option 'lastLoginDate'.
   */
  const SORT_LASTLOGINDATE = 'lastLoginDate';

  /**
   * Sort option 'lastActivityDate'.
   */
  const SORT_LASTACTIVITYDATE = 'lastActivityDate';

  /**
   * Sort option 'numberOfActivities'.
   */
  const SORT_NUMBEROFACTIVITIES = 'numberOfActivities';

  /**
   * Sort option 'numberOfLikes'.
   */
  const SORT_NUMBEROFLIKES = 'numberOfLikes';

  /**
   * Sort option 'numberOfSharesFacebook'.
   */
  const SORT_NUMBEROFSHARESFACEBOOK = 'numberOfSharesFacebook';

  /**
   * Sort option 'numberOfSharesTwitter'.
   */
  const SORT_NUMBEROFSHARESTWITTER = 'numberOfSharesTwitter';

  /**
   * Sort option 'numberOfAttends'.
   */
  const SORT_NUMBEROFATTENDS = 'numberOfAttends';

  /**
   * Sort option 'numberOfActiveActivities'.
   */
  const SORT_NUMBEROFACTIVEACTIVITIES = 'numberOfActiveActivities';

  /**
   * Sort option 'numberOfActiveActivities'.
   */
  const SORT_ORDER_ASCENDING= 'ascending';

  /**
   * Sort option 'descending'.
   */
  const SORT_ORDER_DESCENDING = 'descending';

  /**
   * ID of the user.
   *
   * @var string
   */
  public $userId;

  /**
   * Nick of the user.
   *
   * @var string
   */
  public $nick;

  /**
   * First name (givenName), last name and/or nick.
   *
   * @var string
   */
  public $name;

  /**
   * E-mail address.
   *
   * @var string
   */
  public $mbox;

  /**
   * Gender.
   * Possible values are represented in the CultureFeed_User::GENDER_* constants.
   *
   * @var string
   */
  public $gender;

  /**
   * Date of birth represented as a UNIX timestamp.
   *
   * @var integer
   */
  public $dob;

  /**
   * Minimum date of birth represented as a UNIX timestamp.
   *
   * @var integer
   */
  public $dobMin;

  /**
   * Maximum date of birth represented as a UNIX timestamp.
   *
   * @var integer
   */
  public $dobMax;

  /**
   * Street of the home address.
   *
   * @var string
   */
  public $homeStreet;

  /**
   * ZIP of the home address.
   *
   * @var string
   */
  public $homeZip;

  /**
   * City of the home address.
   *
   * @var string
   */
  public $homeCity;

  /**
   * Country of the home address.
   *
   * @var string
   */
  public $homeCountry;

  /**
   * Coordinates of the home address.
   *
   * @var CultureFeed_Location
   */
  public $homeLocation;

  /**
   * Coordinates of the current location.
   *
   * @var CultureFeed_Location
   */
  public $currentLocation;

  /**
   * User status.
   * Possible values are represented in the CultureFeed_User::STATUS_* constants.
   *
   * @var string
   */
  public $status;

  /**
   * User points.
   *
   * @var integer
   */
  public $points;

  /**
   * Minimum user points.
   *
   * @var integer
   */
  public $pointsMin;

  /**
   * Maximum user points.
   *
   * @var integer
   */
  public $pointsMax;

  /**
   * User likes.
   *
   * @var integer
   */
  public $likes;

  /**
   * Minimum user likes.
   *
   * @var integer
   */
  public $likesMin;

  /**
   * Maximum user likes.
   *
   * @var integer
   */
  public $likesMax;

  /**
   * The service consumer id of the consumer where the activity was generated.
   *
   * @var string
   */
  public $createdBy;

  /**
   * Last login time represented as a UNIX timestamp.
   *
   * @var integer
   */
  public $lastLogin;

  /**
   * Minimum last login time represented as a UNIX timestamp.
   *
   * @var integer
   */
  public $lastLoginMin;

  /**
   * Maximum last login time represented as a UNIX timestamp.
   *
   * @var integer
   */
  public $lastLoginMax;

  /**
   * Start position.
   *
   * @var integer
   */
  public $start;

  /**
   * Maximum number of items to return.
   *
   * @var integer
   */
  public $max;

  /**
   * Sort field.
   * Possible values are represented in the SORT_* constants.
   *
   * @var string
   */
  public $sort;

  /**
   * Sort order.
   * Possible values are represented in the SORT_ORDER_* constants.
   *
   * @var string
   */
  public $order;

  /**
   * Convert a CultureFeed_SearchUsersQuery object to an array that can be used as data in POST requests that expect search user query info.
   *
   * @return array
   *   Associative array representing the object. For documentation of the structure, check the Culture Feed API documentation.
   */
  public function toPostData() {
    // For most properties we can rely on get_object_vars.
    $data = get_object_vars($this);

    // Represent homeLocation as a string.
    if (isset($data['homeLocation'])) {
      $data['homeLocation'] = (string)$data['homeLocation'];
    }

    // Represent currentLocation as a string.
    if (isset($data['currentLocation'])) {
      $data['currentLocation'] = (string)$data['currentLocation'];
    }

    // Represent dob as a W3C date.
    if (isset($data['dob'])) {
      $data['dob'] = date('c', $data['dob']);
    }

    // Represent dobMin as a W3C date.
    if (isset($data['dobMin'])) {
      $data['dobMin'] = date('c', $data['dobMin']);
    }

    // Represent dobMax as a W3C date.
    if (isset($data['dobMax'])) {
      $data['dobMax'] = date('c', $data['dobMax']);
    }

    // Represent lastLogin as a W3C date.
    if (isset($data['lastLogin'])) {
      $data['lastLogin'] = date('c', $data['lastLogin']);
    }

    // Represent lastLoginMin as a W3C date.
    if (isset($data['lastLoginMin'])) {
      $data['lastLoginMin'] = date('c', $data['lastLoginMin']);
    }

    // Represent lastLoginMax as a W3C date.
    if (isset($data['lastLoginMax'])) {
      $data['lastLoginMax'] = date('c', $data['lastLoginMax']);
    }

    $data = array_filter($data);

    return $data;
  }

}

