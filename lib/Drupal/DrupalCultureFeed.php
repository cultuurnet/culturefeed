<?php

/**
 * @class
 * Drupal layer for culturefeed.
 */
class DrupalCultureFeed extends DrupalCultureFeedBase {

  protected static $consumer_instance;
  protected static $user_instance;
  protected static $logged_in_user;

  public static function getInstance($token, $secret, $application_key = NULL, $shared_secret = NULL) {
    $endpoint = variable_get('culturefeed_api_location', CULTUREFEED_API_LOCATION);
    $oauth_client = self::getOAuthClient($endpoint, $token, $secret, $application_key, $shared_secret);

    $cf = new CultureFeed($oauth_client);

    if (self::isCacheEnabled()) {
      $cf = new DrupalCultureFeed_Cache($cf, self::getLoggedInUserId());
    }

    return $cf;
  }

  public static function createUser(CultureFeed_User $user) {
    self::getLoggedInUserInstance()->createUser($user);
  }

  public static function updateUser(CultureFeed_User $user, $fields = array()) {
    self::getLoggedInUserInstance()->updateUser($user, $fields);
  }

  public static function deleteUser($id) {
    self::getLoggedInUserInstance()->deleteUser($id);

    // Let other modules hook onto the user delete.
    module_invoke_all('culturefeed_user_delete', $id);
  }

  public static function getUser($id, $private = FALSE) {
    if ($private) {
      $user = self::getLoggedInUserInstance()->getUser($id, TRUE, TRUE);
    }
    else {
      $user = self::getConsumerInstance()->getUser($id, FALSE, FALSE);
    }
    parent::setAvailableCategories($user);
    return $user;
  }

  public static function getUserlightId($email, $home_zip = '') {
    return self::getConsumerInstance()->getUserlightId($email, $home_zip);
  }

  public static function searchUsers(CultureFeed_SearchUsersQuery $query) {
    return self::getConsumerInstance()->searchUsers($query);
  }

  public static function getSimilarUsers($id) {
    return self::getConsumerInstance()->getSimilarUsers($id);
  }

  public static function uploadUserDepiction($id, $file_data) {
    self::getLoggedInUserInstance()->uploadUserDepiction($id, $file_data);
  }

  public static function removeUserDepiction($id) {
    self::getLoggedInUserInstance()->removeUserDepiction($id);
  }

  public static function resendMboxConfirmationForUser($id) {
    self::getLoggedInUserInstance()->resendMboxConfirmationForUser($id);
  }

  public static function updateUserPrivacy($id, CultureFeed_UserPrivacyConfig $privacy_config) {
    self::getLoggedInUserInstance()->updateUserPrivacy($id, $privacy_config);
  }

  public static function getUserPreferences($id) {
    return self::getLoggedInUserInstance()->getUserPreferences($id);
  }

  public static function setUserPreferences($id, CultureFeed_Preferences $preferences) {
    self::getLoggedInUserInstance()->setUserPreferences($id, $preferences);
  }

  public static function getUserServiceConsumers($id) {
    return self::getLoggedInUserInstance()->getUserServiceConsumers($id);
  }

  public static function revokeUserServiceConsumer($user_id, $consumer_id) {
    self::getLoggedInUserInstance()->revokeUserServiceConsumer($user_id, $consumer_id);
  }

  public static function updateUserOnlineAccount($id, CultureFeed_OnlineAccount $account) {
    self::getLoggedInUserInstance()->updateUserOnlineAccount($id, $account);
  }

  public static function getUserOnlineAccounts($application_key = NULL, $shared_secret = NULL) {
    $cf_account = self::getLoggedInUser($application_key, $shared_secret);

    $online_accounts = array();

    if ($cf_account->holdsAccount) {
      foreach ($cf_account->holdsAccount as $online_account) {
        $online_accounts[$online_account->accountType] = $online_account;
      }
    }

    return $online_accounts;
  }

  public static function getUserOnlineAccount($account_type) {
    $online_accounts = self::getUserOnlineAccounts();

    if (isset($online_accounts[$account_type])) {
      return $online_accounts[$account_type];
    }

    return FALSE;
  }

  public static function deleteUserOnlineAccount($id, $account_type, $account_name) {
    self::getLoggedInUserInstance()->deleteUserOnlineAccount($id, $account_type, $account_name);
  }

  public static function createActivity(CultureFeed_Activity $activity) {

    $activity = self::getLoggedInUserInstance()->createActivity($activity);

    // Let other modules hook onto the activity creation result.
    module_invoke_all('culturefeed_social_activity_created', $activity);

    return $activity;
  }

  public static function postToSocial($id, $account_name, $account_type, $message, $image = NULL, $link = NULL) {
    return self::getLoggedInUserInstance()->postToSocial($id, $account_name, $account_type, $message, $image, $link);
  }

  public static function updateActivity($id, $private) {
    return self::getLoggedInUserInstance()->updateActivity($id, $private);
  }

  public static function deleteActivity($id) {
    $result = self::getLoggedInUserInstance()->deleteActivity($id);
    module_invoke_all('culturefeed_social_activity_deleted', $result);
    return $result;
  }

  public static function deleteActivities($user_id, $node_id, $content_type, $activity_type) {
    $query = new CultureFeed_SearchActivitiesQuery();
    $query->type = $activity_type;
    $query->contentType = $content_type;
    $query->nodeId = $node_id;
    $query->userId = $user_id;
    $query->private = TRUE;

    $activities = self::searchActivities($query);

    if (empty($activities->objects)) {
      return;
    }

    $result = array();
    foreach ($activities->objects as $activity) {
      $result[] = self::deleteActivity($activity->id);
    }
    return $result;
  }

  public static function searchActivityUsers($nodeId, $type, $contentType, $start = NULL, $max = NULL) {
    return self::getConsumerInstance()->searchActivityUsers($nodeId, $type, $contentType, $start, $max);
  }

  public static function loadActivity($activity_id) {

    try {

      $query = new CultureFeed_SearchActivitiesQuery();
      $query->activityId = $activity_id;

      $result = DrupalCultureFeed::searchActivities($query);

      return current($result->objects);
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_pages', $e);
    }
  }

  public static function searchActivities(CultureFeed_SearchActivitiesQuery $query) {

    if (self::isCultureFeedUser()) {
      $data = self::getLoggedInUserInstance()->searchActivities($query);
    }
    else {
      $data = self::getConsumerInstance()->searchActivities($query);
    }

    return $data;
  }

  public static function getTotalActivities($userId, $type_contentType, $private = FALSE) {
    return self::getLoggedInUserInstance()->getTotalActivities($userId, $type_contentType, $private);
  }

  public static function getTotalPageActivities($pageId, $type_contentType, $private = FALSE) {
    if (self::isCultureFeedUser()) {
      return self::getLoggedInUserInstance()->getTotalPageActivities($pageId, $type_contentType, $private);
    }
    return self::getConsumerInstance()->getTotalPageActivities($pageId, $type_contentType, FALSE);
  }

  /**
   * @see CultureFeed::getActivityPointsTimeline()
   */
  public static function getActivityPointsTimeline($userId) {
    return self::getLoggedInUserInstance()->getActivityPointsTimeline($userId);
  }

  public static function getActivityPointsPromotion($promotionId) {
    if (self::isCultureFeedUser()) {
      return self::getLoggedInUserInstance()->getActivityPointsPromotion($promotionId);
    }
    return self::getConsumerInstance()->getActivityPointsPromotion($promotionId);
  }

  public static function getActivityPointsPromotions($params = array()) {
    if (self::isCultureFeedUser()) {
      return self::getLoggedInUserInstance()->getActivityPointsPromotions($params);
    }
    return self::getConsumerInstance()->getActivityPointsPromotions($params);
  }

  public static function cashInPromotion($userId, array $promotionId, array $promotionCount) {
    return self::getLoggedInUserInstance()->cashInPromotion($userId, $promotionId, $promotionCount);
  }

  public static function getTemplate($id) {
    return self::getLoggedInUserInstance()->getTemplate($id);
  }

  public static function createTemplate(CultureFeed_Template $template) {
    return self::getLoggedInUserInstance()->createTemplate($template);
  }

  public static function updateTemplate(CultureFeed_Template $template, $fields = array()) {
    self::getLoggedInUserInstance()->updateTemplate($template, $fields);
  }

  public static function getTemplateList() {
    return self::getLoggedInUserInstance()->getTemplateList();
  }

  public static function deleteTemplate($id) {
    self::getLoggedInUserInstance()->deleteTemplate($id);
  }

  public static function getServiceConsumer($consumerKey) {
    return self::getLoggedInUserInstance()->getServiceConsumer($consumerKey);
  }

  public static function getServiceConsumers($start = 0, $max = NULL) {
    return self::getLoggedInUserInstance()->getServiceConsumers($start, $max);
  }

  public static function getMailing($id) {
    return self::getLoggedInUserInstance()->getMailing($id);
  }

  public static function createMailing(CultureFeed_Mailing $mailing) {
    return self::getLoggedInUserInstance()->createMailing($mailing);
  }

  public static function updateMailing(CultureFeed_Mailing $mailing, $fields = array()) {
    self::getLoggedInUserInstance()->updateMailing($mailing, $fields);
  }

  public static function disableMailing($id) {
    self::getLoggedInUserInstance()->disableMailing($id);
  }

  public static function deleteMailing($id) {
    self::getLoggedInUserInstance()->deleteMailing($id);
  }

  public static function getMailingList(CultureFeed_SearchMailingsQuery $query) {
    return self::getLoggedInUserInstance()->getMailingList($query);
  }

  public static function sendTestMailing($user_id, $mailing_id) {
    self::getLoggedInUserInstance()->sendTestMailing($user_id, $mailing_id);
  }

  public static function sendMailing($id) {
    self::getLoggedInUserInstance()->sendMailing($id);
  }

  public static function searchMailings(CultureFeed_SearchMailingsQuery $query) {
    return self::getLoggedInUserInstance()->searchMailings($query);
  }

  public static function subscribeToMailing($user_id, $mailing_id, $use_auth = TRUE) {
    if ($use_auth) {
      self::getLoggedInUserInstance()->subscribeToMailing($user_id, $mailing_id, $use_auth);
    }
    else {
      self::getConsumerInstance()->subscribeToMailing($user_id, $mailing_id, $use_auth);
    }

  }

  public static function unsubscribeFromMailing($user_id, $mailing_id, $use_auth = TRUE) {
    if ($use_auth) {
      self::getLoggedInUserInstance()->unsubscribeFromMailing($user_id, $mailing_id, $use_auth);
    }
    else {
      self::getConsumerInstance()->unsubscribeFromMailing($user_id, $mailing_id, $use_auth);
    }
  }

  public static function getMailingSubscriptions($user_id, $use_auth = TRUE) {
    return self::getLoggedInUserInstance()->getMailingSubscriptions($user_id, $use_auth);
  }

  public static function getTopEvents($type, $max = 5) {
    return self::getConsumerInstance()->getTopEvents($type, $max);
  }

  public static function getRecommendationsForUser($id, CultureFeed_RecommendationsQuery $query = NULL, $skip_cache = FALSE) {
    return self::getLoggedInUserInstance()->getRecommendationsForUser($id, $query, $skip_cache);
  }

  public static function getRecommendationsForEvent($id, CultureFeed_RecommendationsQuery $query = NULL) {
    return self::getConsumerInstance()->getRecommendationsForEvent($id, $query);
  }

  public static function evaluateRecommendation($id, $evaluation) {
    self::getLoggedInUserInstance()->evaluateRecommendation($id, $evaluation);
  }

  /**
   * @see ICultureFeed::getNotificationsCount()
   */
  public static function getNotificationsCount($userId, $dateFrom = NULL) {
    return self::getLoggedInUserInstance()->getNotificationsCount($userId, $dateFrom);
  }

  /**
   * @see ICultureFeed::getNotifications()
   */
  public static function getNotifications($userId, $params = array()) {
    return self::getLoggedInUserInstance()->getNotifications($userId, $params);
  }

  public static function getNodeStatus($contentType, $nodeId, $userId) {
    return self::getLoggedInUserInstance()->getNodeStatus($contentType, $nodeId, $userId);
  }

  public static function followNode($contentType, $nodeId, $userId) {
    return self::getLoggedInUserInstance()->followNode($contentType, $nodeId, $userId);
  }

  public static function unFollowNode($contentType, $nodeId, $userId) {
    return self::getLoggedInUserInstance()->unFollowNode($contentType, $nodeId, $userId);
  }

  public static function getUrlAddSocialNetwork($network, $destination = '') {
    return self::getConsumerInstance()->getUrlAddSocialNetwork($network, $destination);
  }

  public static function getUrlChangePassword($id, $destination = '') {
    return self::getConsumerInstance()->getUrlChangePassword($id, $destination);
  }

  public static function getUrlLogout($destination = '') {
    return self::getConsumerInstance()->getUrlLogout($destination);
  }

  public static function userDidActivity($type, $nodeId, $contentType) {
    $userDidActivity = &drupal_static(__FUNCTION__, array());

    $user = self::getLoggedInUserId();

    if (!isset($userDidActivity[$nodeId][$contentType][$user])) {
      $query = new CultureFeed_SearchActivitiesQuery();
      $query->nodeId = $nodeId;
      $query->contentType = $contentType;
      $query->userId = $user;
      $query->private = TRUE;

      $activities = self::searchActivities($query);

      $userDidActivity[$nodeId][$contentType][$user] = $activities;
    }

    $activities = $userDidActivity[$nodeId][$contentType][$user];

    if (!empty($activities->objects)) {
      foreach ($activities->objects as $activity) {
        if ($activity->type == $type) {
          return TRUE;
        }
      }
    }

    return FALSE;
  }

  public static function activityGetCount($type, $nodeId, $contentType) {

    $activityCount = &drupal_static(__FUNCTION__, array());

    if (!isset($activityCount[$nodeId][$contentType])) {

      $query = new CultureFeed_SearchActivitiesQuery();
      $query->nodeId = $nodeId;
      $query->contentType = $contentType;

      $activities = self::searchActivities($query);

      $activityCount[$nodeId][$contentType] = $activities;
    }

    $activities = $activityCount[$nodeId][$contentType];

    $count = 0;
    if (!empty($activities->objects)) {
      foreach ($activities->objects as $activity) {
        if ($activity->type == $type && $activity->nodeId == $nodeId) {
          $count++;
        }
      }
    }

    return $count;

  }

}