<?php

function culturefeed_oauth_connect() {
  $cf = DrupalCultureFeed::getConsumerInstance();

  $token = $cf->getRequestToken();

  if (!$token) {
    return;
  }
  
  $_SESSION['oauth_token'] = $token['oauth_token'];
  $_SESSION['oauth_token_secret'] = $token['oauth_token_secret'];

  $callback_url = url('culturefeed/oauth/authorize', array('absolute' => TRUE));

  $auth_url = $cf->getUrlAuthorize($token, $callback_url);

  drupal_goto($auth_url);
}

function culturefeed_oauth_authorize() {
  if (isset($_GET['oauth_token']) && isset($_GET['oauth_verifier'])) {
    $token = DrupalCultureFeed::getInstance($_GET['oauth_token'], $_SESSION['oauth_token_secret'])->getAccessToken($_GET['oauth_verifier']);
    
    unset($_SESSION['oauth_token']);
    unset($_SESSION['oauth_token_secret']);

    $account = culturefeed_user_get($token['userId'], $token['oauth_token'], $token['oauth_token_secret']);

    if ($account) {
      $account->culturefeed_userid = $token['userId'];
      $account->culturefeed_oauth_token = $token['oauth_token'];
      $account->culturefeed_oauth_secret = $token['oauth_token_secret'];

      user_save($account);

      global $user;
      $user = $account;

      user_login_finalize();

      drupal_goto('');
    }
  }
}