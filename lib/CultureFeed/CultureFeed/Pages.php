<?php

/**
 * @class
 * Interface for pages management
 */
interface CultureFeed_Pages {

  /**
   * Get the detail of a page.
   * @param string $id
   *   Page id to load.
   */
  public function getPage($id);

  /**
   * Get the list from all users for a given page id.
   * @param string $id
   *   Page id to query.
   * @param array $roles
   *   Roles to filter on. (ADMIN / MEMBER / FOLLOWER)
   * @return CultureFeed_ResultSet
   */
  public function getUserList($id, $roles = array());

  /**
   * Add a page.
   * @param array $params
   *   Params to create the page.
   */
  public function addPage(array $params);

  /**
   * Update a page.
   * @param Integer $id
   *   The page ID.
   * @param array $params
   *   Params to update the page.
   */
  public function updatePage($id, array $params);

  /**
   * Remove a page.
   * @param Integer $id
   *   The page ID of the page to remove (set invisible).
   */
  public function removePage($id);

  /**
   * Publish a page.
   * @param Integer $id
   *   The page ID of the page to publish (set visible).
   */
  public function publishPage($id);

  /**
   * Add image to an existing page.
   * @param string $id
   *   Id of the page.
   * @param array $params
   *   Params to create the image for the page.
   */
  public function addImage($id, array $params);

  /**
   * Remove an image of the existing page.
   * @param string $id
   *   Id of the page.
   */
  public function removeImage($id);

  /**
   * Add cover to an existing page.
   * @param string $id
   *   Id of the page.
   * @param array $params
   *   Params to create the cover for the page.
   */
  public function addCover($id, array $params);

  /**
   * Remove a cover of the existing page.
   * @param string $id
   *   Id of the page.
   */
  public function removeCover($id);

  /**
   * Change the permissions for a page.
   * @param Integer $id
   *   The page ID of the page to remove (set invisible).
   * @param array $params
   *   Params of permissions keys to set. E.g. allowMembers, allowComments, ...
   */
  public function changePermissions($id, array $params);

  /**
   * Add a member to the given page.
   * @param string $id
   *   Id of the page.
   * @param string $userId
   *   User ID of the user to add.
   * @param array $params
   *   Array of other data to be stored for the membership.
   */
  public function addMember($id, $userId, $params = array());

  /**
   * Update the membership data for a user on a page.
   * @param string $id
   *   The page ID to update.
   * @param string $userId
   *   The user id to update.
   * @param array $params
   *   Params of membership data to be updated.
   */
  public function updateMember($id, $userId, array $params);

  /**
   * Update the page so it has a new follower.
   * @param string $id
   *   The page ID to update.

   * @param array $params
   *   Params for the follower data to be updated.
   *   E.g. activityPrivate
   */
  public function follow($id, array $params = array());

  /**
   * Update the page so it looses a follower.
   * @param string $id
   *   The page ID to update.
   * @param string $userId
   *   User ID of the user which stops following the page.
   * @param array $params
   *   Params for the follower data to be updated.
   *   E.g. activityPrivate
   */
  public function defollow($id, $userId, array $params = array());

  /**
   * Delete a member from the given page.
   * @param string $id
   *   Id of the page.
   * @param string $userId
   *   User ID of the user to remote.
   */
  public function removeMember($id, $userId);

  /**
   * Add a new admin to a page
   * @param string $id
   *   The page ID to add to.
   * @param string $userId
   *   The user id to add as admin.
   * @param array $params
   *   Extra data to add.
   */
  public function addAdmin($id, $userId, $params = array());

  /**
   * Remove an admin from a page
   * @param string $id
   *   The page ID to remove from.
   * @param string $userId
   *   The user id to remove as admin.
   */
  public function removeAdmin($id, $userId);

  /**
   * Get the timeline of a page.
   * @param string $id
   *   The page ID where the timeline is requested for.
   * @param string $dateFrom
   *   ISO Date to set the startdate of the timeline. (optional)
   * @param array $activityTypes
   *   List of activity types to be shown in the timeline.
   * @return CultureFeed_ResultSet
   *   CultureFeed_ResultSet where the objects are of the CultureFeed_Activity type.
   */
  public function getTimeline($id, $dateFrom = NULL, $activityTypes = array());

  /**
   * Get the notifications for a page.
   * @param string $id
   *   The page ID where the notifications are requested for.
   * @param array $params
   *  Array with additional filter params
   * @return CultureFeed_ResultSet
   *   CultureFeed_ResultSet where the objects are of the CultureFeed_Activity type.
   */
  public function getNotifications($id, $params = array());

}
