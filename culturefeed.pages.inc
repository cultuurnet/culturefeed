<?php

function culturefeed_oauth_connect() {
  $cf = DrupalCultureFeed::getConsumerInstance();

  $token = $cf->getRequestToken();

  if (!$token) {
    return;
  }

  $callback_url = url('culturefeed/oauth/authorize', array('query' => array('token' => $token['oauth_token'], 'token_secret' => $token['oauth_token_secret']), 'absolute' => TRUE));//"http://localhost/php-oauth/example/authorize.php?token=$parsed_token&token_secret=$parsed_secret";

  $auth_url = $cf->getUrlAuthorize($token, $callback_url);

  Header("Location: $auth_url");
  exit;
}

function culturefeed_oauth_authorize() {
  if (isset($_GET['oauth_token']) && isset($_GET['oauth_verifier'])) {
    $token = DrupalCultureFeed::getInstance($_GET['oauth_token'], $_GET['token_secret'])->getAccessToken($_GET['oauth_verifier']);

    $account = culturefeed_getUser($token['userId'], $token['oauth_token'], $token['oauth_token_secret']);

    if ($account) {
      $account->culturefeed_userid = $token['userId'];
      $account->culturefeed_oauth_token = $token['oauth_token'];
      $account->culturefeed_oauth_secret = $token['oauth_token_secret'];

      user_save($account);

      global $user;
      $user = $account;

      user_login_finalize();

      drupal_goto('');
      exit();
    }
  }
}