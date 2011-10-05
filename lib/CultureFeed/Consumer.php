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
   * Organization that owns the consumer.
   *
   * @var string
   */
  public $organization;

  /**
   * Description of the the consumer.
   *
   * @var string
   */
  public $description;

  /**
   * Logo of the the consumer.
   *
   * @var string
   */
  public $logo;

}

