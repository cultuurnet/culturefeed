<?php

/**
 * Class to represent a service consumer.
 */
class CultureFeed_Consumer {

  /**
   * Consumer key of this consumer.
   *
   * @var string
   */
  public $consumerKey;

  /**
   * Shared secret of the consumer.
   *
   * @var string
   */
  public $consumerSecret;

  /**
   * Group IDs for the consumer.
   *
   * @var array
   */
  public $group;

  /**
   * Creation date of this consumer represented as a UNIX timestamp.
   *
   * @var integer
   */
  public $creationDate;

  /**
   * ID of the consumer.
   *
   * @var integer
   */
  public $id;

  /**
   * Name of the consumer.
   *
   * @var string
   */
  public $name;

  /**
   * Description of the consumer.
   *
   * @var string
   */
  public $description;

  /**
   * URL of the logo of the consumer.
   *
   * @var string
   */
  public $logo;

  /**
   * Status of the consumer.
   *
   * @var string
   */
  public $status;

  /**
   * Domain of the consumer.
   *
   * @var string
   */
  public $domain;

  /**
   * Default callback URL of the consumer.
   *
   * @var string
   */
  public $callback;

  /**
   * Redirect URL after verification of an e-mail address.
   *
   * @var string
   */
  public $destinationAfterEmailVerification;

   /**
   * Extract an array useable as data in POST requests that expect consumer info.
   *
   * @return array
   *   Associative array representing the consumer object. For documentation of the structure, check the Culture Feed API documentation.
   */
  public function toPostData() {
    // For most properties we can rely on get_object_vars.
    $data = get_object_vars($this);

    // Represent creationDate as a W3C date.
    if (isset($data['creationDate'])) {
      $data['creationDate'] = date('c', $data['creationDate']);
    }
    
    $data = array_filter($data);

    return $data;
  }
}
