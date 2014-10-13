<?php

/**
 * Class to represent a page membership.
 */
class CultureFeed_Pages_Membership {

  const MEMBERSHIP_ROLE_ADMIN = 'ADMIN';
  const MEMBERSHIP_ROLE_MEMBER = 'MEMBER';

  /**
   * Page from this membership
   * @var CultureFeed_Cdb_Item_Page
   */
  public $page;

  /**
   * User from this membership.
   * @var CultureFeed_User
   */
  public $user;

  /**
   * Member since, represented as a UNIX timestamp
   * @var int
   */
  public $creationDate;

  /**
   * Role for this page.
   */
  public $role;

  /**
   * Relation for this page
   * @var string
   */
  public $relation;

}