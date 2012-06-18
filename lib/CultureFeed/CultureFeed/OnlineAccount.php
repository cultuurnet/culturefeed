<?php

/**
 * Class to represent an online account.
 */
class CultureFeed_OnlineAccount {

  /**
   * Type of the online service.
   */
  public $accountType;

  /**
   * Homepage of the online service.
   *
   * @var string
   */
  public $accountServiceHomepage;

  /**
   * Account name on the service.
   *
   * @var string
   */
  public $accountName;

  /**
   * Nick on the service.
   *
   * @var string
   */
  public $accountNick;

  /**
   * Depiction on the service.
   *
   * @var string
   */
  public $accountDepiction;

  /**
   * Privacy status of this account. If FALSE a user's activities on this service are in the public data for this user.
   *
   * @var bool
   */
  public $private;

  /**
   * Should activities be pushed through to this service.
   *
   * @var bool
   */
  public $publishActivities;

  /**
   * Convert a CultureFeed_OnlineAccount object to an array that can be used as data in POST requests that expect online account info.
   *
   * @return array
   *   Associative array representing the object. For documentation of the structure, check the Culture Feed API documentation.
   */
  public function toPostData() {
    // For most properties we can rely on get_object_vars.
    $data = get_object_vars($this);

    // Represent private as a string (true/false);
    if (isset($data['private'])) {
      $data['private'] = $data['private'] ? 'true' : 'false';
    }

    // Represent publishActivities as a string (true/false);
    if (isset($data['publishActivities'])) {
      $data['publishActivities'] = $data['publishActivities'] ? 'true' : 'false';
    }

    $data = array_filter($data);

    return $data;
  }

}

