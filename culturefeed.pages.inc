<?php

function culturefeed_oauth_connect($type = CultureFeed::AUTHORIZE_TYPE_REGULAR) {
  $cf = DrupalCultureFeed::getConsumerInstance();

  $token = $cf->getRequestToken();

  if (!$token) {
    return;
  }

  $_SESSION['oauth_token'] = $token['oauth_token'];
  $_SESSION['oauth_token_secret'] = $token['oauth_token_secret'];

  $options = array('absolute' => TRUE);
  
  if (isset($_GET['destination'])) {
    $options['query']['destination'] = $_GET['destination'];
    unset($_GET['destination']);
  }
  
  if (isset($_GET['popup'])) {
    $options['query']['popup'] = 'true'; // @todo add destination
  }

  $callback_url = url('culturefeed/oauth/authorize', $options);

  $auth_url = $cf->getUrlAuthorize($token, $callback_url, $type);

  drupal_goto($auth_url);
}

function culturefeed_oauth_authorize() {
  if (isset($_GET['oauth_token']) && isset($_GET['oauth_verifier'])) {
    $token = DrupalCultureFeed::getInstance($_GET['oauth_token'], $_SESSION['oauth_token_secret'])->getAccessToken($_GET['oauth_verifier']);

    unset($_SESSION['oauth_token']);
    unset($_SESSION['oauth_token_secret']);

    $account = culturefeed_user_get($token['userId'], $token['oauth_token'], $token['oauth_token_secret']);

    if ($account) {
      global $user;
      $user = $account;

      user_login_finalize();
      
      if (isset($_GET['popup'])) {
        drupal_add_js('window.opener.location.reload(); window.close();', 'inline');
      }
      else {
        if ($destination = drupal_get_destination())
        drupal_goto('');
      }
    }
  }
  
  return '';
}