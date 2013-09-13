<?php

/**
 * @class
 * Follower from a page.
 */
class CultureFeed_Pages_Follower {

  /**
   * Role key inside a user list for followers.
   * @var string
   */
  const ROLE = 'FOLLOWER';

  /**
   * @var CultureFeed_Cdb_Item_Page
   */
  public $page;

  /**
   * @var CultureFeed_User
   */
  public $user;

  /**
   * Following since, represented as a UNIX timestamp
   * @var int
   */
  public $creationDate;

}
