<?php

abstract class Component {

  /**
   * OAuth request object to do the request.
   *
   * @var CultureFeed_OAuthClient
   */
  protected $oauth_client;

  /**
   * Constructor for a new CultureFeed instance.
   *
   * @param CultureFeed_OAuthClient $oauth_client
   *   A OAuth client to make requests.
   */
  public function __construct(CultureFeed_OAuthClient $oauth_client) {
    $this->oauth_client = $oauth_client;
  }

}