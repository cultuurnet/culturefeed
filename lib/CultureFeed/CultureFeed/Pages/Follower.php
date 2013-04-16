<?php

/**
 * @class
 * Follower from a page.
 */
class CultureFeed_Pages_Follower {

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
