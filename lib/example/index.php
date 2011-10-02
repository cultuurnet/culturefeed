<?php

require 'common.inc';

if (!isset($_COOKIE['oauth_token'])) {
  print '<p><a href="connect.php">Connect</a></p>';
  exit;
}
else {
  print '<p><a href="logout.php">Log out</a></p>';
}

print '<pre>';

$cf = new CultureFeed(CULTUREFEED_API_APPLICATION_KEY, CULTUREFEED_API_SHARED_SECRET, $_COOKIE['oauth_token'], $_COOKIE['oauth_token_secret']);

$uid = $_COOKIE['oauth_user'];

/**
 * Getting a request token.
 */
//$result = $cf->getRequestToken();
//
//var_dump($result);

/*
 * Getting an authorization url for a certain token.
 */
//$result = $cf->getUrlAuthorize($result['oauth_token'], 'http://localhost');
//
//var_dump($result);

/**
 * Getting an access token.
 */
//$oauth_verifier = 'OAUTH VERIFIER HERE';
//$result = $cf->getAccessToken($oauth_verifier);
//
//var_dump($result);

/*
 * Creating a new user.
 */
$nick = 'someguy_'. time();

$user = new CultureFeedUser();

$user->nick = $nick;
$user->password = md5($nick);
$user->gender = CultureFeedUser::GENDER_MALE;
$user->mbox = 'davy+' . time() . '@krimson.be';
$user->status = CultureFeedUser::STATUS_PRIVATE;
$user->homeLocation = new CultureFeedLocation(12.0, 13.0);
$user->dob = mktime(0, 0, 0, 2, 20, 1981);

try {
  $result = $cf->createUser($user);
}
catch (Exception $e) {
  // handle exception
}

var_dump($result);

/*
 * Updating a user's info.
 */

//$user = new CultureFeedUser();
//
//$user->id = THE_USER;
//$user->mbox = 'davy+changed' . time() . '@krimson.be';
//
//$result = $cf->updateUser($user);
//
//var_dump($result);

/*
 * Deleting a user info.
 */

//$result = $cf->deleteUser(THE_USER);
//
//var_dump($result);

/*
 * Getting a user's info.
 */

//$result = $cf->getUser(THE_USER, TRUE, TRUE);
//
//var_dump($result);

/*
 * Searching for users.
 */
//$query = new CultureFeedSearchUsersQuery();
//$query->name = "krimson";
//$query->max = 10;
//$query->sort = CultureFeedSearchUsersQuery::SORT_FIRSTNAME;
//
//$result = $cf->searchUsers($query);
//
//var_dump($result);

/*
 * Get users similar to another user.
 */

//$result = $cf->getSimilarUsers(THE_USER);
//
//var_dump($result);

/*
 * Upload a user depiction.
 */

//$filepath = '/Users/dvdbremt/Downloads/test.jpg';
//$file_data = file_get_contents($filepath);
//$result = $cf->uploadUserDepiction(THE_USER, $file_data);
//
//var_dump($result);

/*
 * Resend confirmation mail.
 */

//$result = $cf->resendMboxConfirmationForUser(THE_USER);
//
//var_dump($result);

/*
 * Update user field privacy.
 */

//$privacy_config = new CultureFeedUserPrivacyConfig();
//$privacy_config->bio = CultureFeedUserPrivacyConfig::PRIVACY_PRIVATE;
//$privacy_config->depiction = CultureFeedUserPrivacyConfig::PRIVACY_PRIVATE;
//$privacy_config->mbox = CultureFeedUserPrivacyConfig::PRIVACY_PRIVATE;
//
//$result = $cf->updateUserPrivacy(THE_USER, $privacy_config);
//
//var_dump($result);

/*
 * Get user's service consumers.
 */

//$result = $cf->getUserServiceConsumers(THE_USER);
//
//var_dump($result);

/*
 * Revoke a user's service consumer.
 */

//$consumer_id = 'PUT CONSUMER ID HERE';
//$result = $cf->revokeUserServiceConsumer(THE_USER, $consumer_id);
//
//var_dump($result);

// @todo updateuseronlineaccount

// @todo deleteuseronlineaccount

// @todo create activity

// @todo update activity

// @todo delete activity

// @todo search activities

/*
 * Get top events.
 */

//$result = $cf->getTopEvents('active'); // @todo make type a const
//
//var_dump($result);

/*
 * Get recommendations for a user.
 */
//$result = $cf->getRecommendationsForUser(THE_USER, array('max' => 10)); // @todo make query a class
//
//var_dump($result);


/*
 * Get recommendations for an event.
 */
//$result = $cf->getRecommendationsForEvent(SOME_EVENT, array('max' => 10)); // @todo make query a class
//
//var_dump($result);


// @todo evaluate recommendation

/*
 * Get add network url.
 */

//$result = $cf->getUrlAddSocialNetwork('twitter');
//
//var_dump($result);

/*
 * Get change password url.
 */

//$result = $cf->getUrlChangePassword(THE_USER, 'http://localhost/callback');
//
//var_dump($result);

/*
 * Get logout url.
 */

//$result = $cf->getUrlLogout('http://localhost/callback');
//
//var_dump($result);

print '</pre>';

exit();

$activity = array(
  'userid' => $uid,
  'nodeId' => 'e12d585f-0c4c-4491-8b61-87c993588487',
  'contentType' => 'event',
  'type' => 1,
  'value' => 'ok',
  'private' => 'false',
);

try {
  $result = $cf->createActivity(new CultureFeedActivity($activity));
  var_dump($result);
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

print $cf->deleteUserOnlineAccount($uid, new CultureFeedOnlineAccount($account));