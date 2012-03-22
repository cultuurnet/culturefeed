<?php

class CultureFeed_Uitpas_Passholder_Query_CheckInPassholderOptions extends CultureFeed_Uitpas_ValueObject {

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
  public $uitpasNumber;

  /**
   * NFC card chip number
   *
   * @var string
   */
  public $chipNumber;

  /**
   * The consumer key of the counter from where the request originates
   *
   * @var string
   */
  public $balieConsumerKey;

}