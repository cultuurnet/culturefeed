<?php

interface ICultureFeed {

  public function getConsumer();

  public function getToken();

  public function getRequestToken($callback = '');

  public function getUrlAuthorize($token, $callback = '', $type = CultureFeed::AUTHORIZE_TYPE_REGULAR, $skip_confirmation = FALSE, $skip_authorization = FALSE, $via = '');

  public function getAccessToken($oauth_verifier);

  public function createUser(CultureFeed_User $user);

  public function getUserPreferences($uid);

  public function setUserPreferences($uid, CultureFeed_Preferences $preferences);

  public function updateUser(CultureFeed_User $user, $fields = array());

  public function deleteUser($id);

  public function getUser($id, $private = FALSE, $use_auth = TRUE);

  public function searchUsers(CultureFeed_SearchUsersQuery $query);

  public function getSimilarUsers($id);

  public function uploadUserDepiction($id, $file_data);

  public function resendMboxConfirmationForUser($id);

  public function updateUserPrivacy($id, CultureFeed_UserPrivacyConfig $privacy_config);

  public function getUserServiceConsumers($id);

  public function revokeUserServiceConsumer($user_id, $consumer_id);

  public function updateUserOnlineAccount($id, CultureFeed_OnlineAccount $account);

  public function deleteUserOnlineAccount($id, $account_type, $account_name);

  public function createActivity(CultureFeed_Activity $activity);

  public function updateActivity($id, $private);

  public function deleteActivity($id);

  public function searchActivities(CultureFeed_SearchActivitiesQuery $query);

  public function searchActivityUsers($nodeId, $type, $contentType, $start = NULL, $max = NULL);

  public function getMailing($id);

  public function createMailing(CultureFeed_Mailing $mailing);

  public function updateMailing(CultureFeed_Mailing $mailing, $fields = array());

  public function disableMailing($id);

  public function deleteMailing($id);

  public function getMailingList(CultureFeed_SearchMailingsQuery $query);

  public function sendTestMailing($user_id, $mailing_id);

  public function sendMailing($id);

  public function searchMailings(CultureFeed_SearchMailingsQuery $query);

  public function subscribeToMailing($user_id, $mailing_id);

  public function unsubscribeFromMailing($user_id, $mailing_id);

  public function getMailingSubscriptions($user_id);

  public function getTopEvents($type, $max = 5);

  public function getRecommendationsForUser($id, CultureFeed_RecommendationsQuery $query = NULL);

  public function getRecommendationsForEvent($id, CultureFeed_RecommendationsQuery $query = NULL);

  public function evaluateRecommendation($id, $evaluation);

  public function getUrlAddSocialNetwork($network, $destination = '');

  public function getUrlChangePassword($id, $destination = '');

  public function getUrlLogout($destination = '');

  public function getServiceConsumers($start = 0, $max = NULL);

  public function createServiceConsumer(CultureFeed_Consumer $consumer);

  public function updateServiceConsumer(CultureFeed_Consumer $consumer);

  public function uitpas();

  public function getClient();

}