<?php define('SOME_EVENT', 'e12d585f-0c4c-4491-8b61-87c993588487'); ?>

<?php

if (!isset($_GET['action'])) {
  return;
}

/*
 * Creating a new user.
 */

if ($_GET['action'] == 'createUser') {
  
  $nick = 'someguy+'. time();
  
  $user = new CultureFeedUser();
  
  $user->nick = $nick;
  $user->password = md5($nick);
  $user->gender = CultureFeedUser::GENDER_MALE;
  $user->mbox = $nick . '@somedomain.be';
  $user->status = CultureFeedUser::STATUS_PUBLIC;
  $user->homeLocation = new CultureFeedLocation(12.0, 13.0);
  $user->dob = mktime(0, 0, 0, 2, 20, 1981);
  
  try {
    $result = $cf->createUser($user);
  }
  catch (Exception $e) {
    // handle exception
  }
  
  print "<pre>".print_r($result, TRUE)."</pre>";
  
}

/*
 * Updating a user's info.
 */

elseif ($_GET['action'] == 'updateUser') {
  
  $user = new CultureFeedUser();
  
  $user->id = $uid;
  $user->homeAddress = 'Some other address';
  $user->bio = 'Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.';
  
  $result = $cf->updateUser($user);
  
  print "<pre>".print_r($result, TRUE)."</pre>";
  
}

/*
 * Deleting a user info.
 */

elseif ($_GET['action'] == 'deleteUser') {
  
  $result = $cf->deleteUser($uid);
  
  print "<pre>".print_r($result, TRUE)."</pre>";
  
}

/*
 * Getting a user's info.
 */

elseif ($_GET['action'] == 'getUser') {
  
  $result = $cf->getUser($uid, TRUE, TRUE);
  
  print "<pre>".print_r($result, TRUE)."</pre>";
  
}

/*
 * Searching for users.
 */
 
elseif ($_GET['action'] == 'searchUsers') {
  
  $query = new CultureFeedSearchUsersQuery();
  $query->max = 10;
  $query->sort = CultureFeedSearchUsersQuery::SORT_FIRSTNAME;
  
  $result = $cf->searchUsers($query);
  
  print "<pre>".print_r($result, TRUE)."</pre>";
  
}

/*
 * Get users similar to another user.
 */

elseif ($_GET['action'] == 'getSimilarUsers') {
  
  $result = $cf->getSimilarUsers($uid);
  
  print "<pre>".print_r($result, TRUE)."</pre>";

}

/*
 * Upload a user depiction.
 */

elseif ($_GET['action'] == 'uploadUserDepiction') {
  
  $filepath = realpath(dirname(__FILE__)) . '/test.jpg';
  $file_data = file_get_contents($filepath);
  $result = $cf->uploadUserDepiction($uid, $file_data);
  
  print "<pre>".print_r($result, TRUE)."</pre>";
  
}

/*
 * Resend confirmation mail.
 */

elseif ($_GET['action'] == 'resendMboxConfirmationForUser') {
  
  $result = $cf->resendMboxConfirmationForUser($uid);
  
  print "<pre>".print_r($result, TRUE)."</pre>";

}

/*
 * Update user field privacy.
 */

elseif ($_GET['action'] == 'updateUserPrivacy') {
  
  $privacy_config = new CultureFeedUserPrivacyConfig();
  $privacy_config->bio = CultureFeedUserPrivacyConfig::PRIVACY_PRIVATE;
  $privacy_config->depiction = CultureFeedUserPrivacyConfig::PRIVACY_PRIVATE;
  $privacy_config->mbox = CultureFeedUserPrivacyConfig::PRIVACY_PRIVATE;
  
  $result = $cf->updateUserPrivacy($uid, $privacy_config);
  
  print "<pre>".print_r($result, TRUE)."</pre>";
  
}

/*
 * Get user's service consumers.
 */

elseif ($_GET['action'] == 'getUserServiceConsumers') {
  
  $result = $cf->getUserServiceConsumers($uid);
  
  print "<pre>".print_r($result, TRUE)."</pre>";
  
}

/*
 * Revoke a user's service consumer.
 */

elseif ($_GET['action'] == 'revokeUserServiceConsumer') {
  
  $consumer_id = 'PUT CONSUMER ID HERE';
  $result = $cf->revokeUserServiceConsumer($uid, $consumer_id);
  
  print "<pre>".print_r($result, TRUE)."</pre>";
  
}

//   @todo updateuseronlineaccount

//   @todo deleteuseronlineaccount

//   @todo create activity

//   @todo update activity

//   @todo delete activity

//   @todo search activities

/*
 * Get top events.
 */

elseif ($_GET['action'] == 'getTopEvents') {
  
  $result = $cf->getTopEvents('active'); // @todo make type a const
  
  print "<pre>".print_r($result, TRUE)."</pre>";
  
}

/*
 * Get recommendations for a user.
 */
 
elseif ($_GET['action'] == 'getRecommendationsForUser') {
  
  $result = $cf->getRecommendationsForUser($uid, array('max' => 10)); // @todo make query a class
  
  print "<pre>".print_r($result, TRUE)."</pre>";
  
}

/*
 * Get recommendations for an event.
 */
 
elseif ($_GET['action'] == 'getRecommendationsForEvent') {
  
  $result = $cf->getRecommendationsForEvent(SOME_EVENT, array('max' => 10)); // @todo make query a class
  
  print "<pre>".print_r($result, TRUE)."</pre>";
  
}

//   @todo evaluate recommendation

?>

<?php

/*$activity = array(
  'userid' => $uid,
  'nodeId' => 'e12d585f-0c4c-4491-8b61-87c993588487',
  'contentType' => 'event',
  'type' => 1,
  'value' => 'ok',
  'private' => 'false',
);

try {
  $result = $cf->createActivity(new CultureFeedActivity($activity));
  print "<pre>".print_r($result, TRUE)."</pre>";
}
catch (Exception $e) {
  var_dump($e);
}

exit();

$account = array(
  'accountType' => 'twitter',
  'accountName' => '23918389',
  'private' => TRUE,
  'publishActivities' => 'false',
);

print $cf->deleteUserOnlineAccount($uid, new CultureFeedOnlineAccount($account));*/