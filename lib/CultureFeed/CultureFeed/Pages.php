<?php

/**
 * @class
 * Interface for pages management
 */
interface CultureFeed_Pages {

  /**
   * Get the list from all users for a given page id.
   * @param string $id
   *   Page id to query.
   * @param array $roles
   *   Roles to filter on. (ADMIN / MEMBER / FOLLOWER)
   * @return CultureFeed_ResultSet
   */
  public function getUserList($id, $roles = array());

}
