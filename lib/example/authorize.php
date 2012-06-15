<?php

require 'common.php';

if (isset($_GET['oauth_token']) && isset($_GET['oauth_verifier'])) {
  $cf = new CultureFeed(new CultureFeed_DefaultOAuthClient($_COOKIE['key'], $_COOKIE['secret'], $_GET['oauth_token'], $_COOKIE['oauth_token_secret']));

  $token = $cf->getAccessToken($_GET['oauth_verifier']);

  setcookie('oauth_user', $token['userId']);
  setcookie('oauth_token', $token['oauth_token']);
  setcookie('oauth_token_secret', $token['oauth_token_secret']);

  header('Location: index.php');
  exit();
}
