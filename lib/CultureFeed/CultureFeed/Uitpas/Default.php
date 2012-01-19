<?php

/**
 *
 */
class CultureFeed_Uitpas_Default implements CultureFeed_Uitpas {

  /**
   *
   * CultureFeed object to make CultureFeed core requests.
   * @var ICultureFeed
   */
  protected $culturefeed;

  /**
   * OAuth request object to do the request.
   *
   * @var CultureFeed_OAuthClient
   */
  protected $oauth_client;

  /**
   *
   * Constructor for a new UitPas_Default instance
   * @param ICultureFeed $culturefeed
   */
  public function __construct(ICultureFeed $culturefeed) {
    $this->culturefeed = $culturefeed;
    $this->oauth_client = $culturefeed->getClient();
  }

}