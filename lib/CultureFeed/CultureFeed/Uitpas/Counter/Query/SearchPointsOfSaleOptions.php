<?php

class CultureFeed_Uitpas_Counter_Query_SearchPointsOfSaleOptions extends CultureFeed_Uitpas_ValueObject {

  /**
   * The city of the POS. Cannot be used in combination with the userId. (Required)
   *
   * @var string
   */
  public $city;

  /**
   * The uid of the passholder
   *
   * @var string
   */
  public $userId;

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