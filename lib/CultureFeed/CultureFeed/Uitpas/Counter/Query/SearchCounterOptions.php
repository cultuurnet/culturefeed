<?php

class CultureFeed_Uitpas_Counter_Query_SearchCounterOptions extends CultureFeed_Uitpas_ValueObject {

  /**
   * If TRUE, only search for "Point of Sale" counters
   *
   * @var boolean
   */
  public $pointOfSale;
  
  /**
   * If TRUE, only search for school counters
   *
   * @var boolean
   */
  public $school;
  
  /**
   * Consumer key of the wanted counter
   *
   * @var string
   */
  public $key;
  
  /**
   * UID of the passholder. Will return a collection of counters where the city is the home city of the passholder.
   *
   * @var string
   */
  public $userId;

  /**
   * The city of the POS. Cannot be used in combination with the userId. (Required)
   *
   * @var string
   */
  public $city;

  /**
   * The type of the POS
   *
   * @var string
   */
  public $type;

  /**
   * Results offset.
   *
   * @var integer
   */
  public $start;

  /**
   * Maximum number of results.
   *
   * @var integer
   */
  public $max;

}