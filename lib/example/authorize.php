<?php

require 'common.inc';

if (isset($_GET['oauth_token']) && isset($_GET['oauth_verifier'])) {
  $cf = new CultureFeed(CULTUREFEED_API_APPLICATION_KEY, CULTUREFEED_API_SHARED_SECRET, $_GET['oauth_token'], $_COOKIE['oauth_token_secret']);
  
  $token = $cf->getAccessToken($_GET['oauth_verifier']);

  setcookie('oauth_user', $token['userId']);
  setcookie('oauth_token', $token['oauth_token']);
  setcookie('oauth_token_secret', $token['oauth_token_secret']);

  header('Location:' . CULTUREFEED_API_SERVER . 'index.php');
  exit();
}
