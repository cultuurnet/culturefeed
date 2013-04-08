<?php

/**
 * Class to represent a page membership from a user.
 */
class CultureFeed_PageMembership {

  const PAGE_MEMBERSHIP_ROLE_ADMIN = 'ADMIN';
  const PAGE_MEMBERSHIP_ROLE_MEMBER = 'MEMBER';

  /**
   * Uid from the page where this membership belongs to.
   *
   * @var string
   */
  public $pageId;

  /**
   * Name from the page.
   * @var string
   */
  public $pageName;

  /**
   * Member since, represented as a UNIX timestamp
   * @var int
   */
  public $creationDate;

  /**
   * Role for this page.
   */
  public $role;

}