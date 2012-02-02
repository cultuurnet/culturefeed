<?php

class CultureFeed_Uitpas_Passholder_Event extends CultureFeed_Uitpas_ValueObject {

  /**
   * The CDBID of the event
   *
   * @var string
   */
  public $cdbid;

  /**
   * The UitPas number
   *
   * @var string
   */
  public $uitpas_number;

  /**
   * NFC card chip number
   *
   * @var string
   */
  public $chip_number;

  /**
   * The consumer key of the counter from where the request originates
   *
   * @var string
   */
  public $consumer_key_counter;

}