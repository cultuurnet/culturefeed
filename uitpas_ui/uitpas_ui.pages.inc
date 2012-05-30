<?php
/**
 * @file
 * Page callbacks
 */

/**
 * Page callback for culturefeed/oauth/authorize adapted to redirect newly created users
 */

function uitpas_ui_oauth_authorize($application_key = NULL) {
  global $conf;
  // We don't want to cache this type of page.
  $conf['cache'] = FALSE;
  if (isset($_GET['oauth_token']) && isset($_GET['oauth_verifier'])) {
    // If an application key was passed, fetch the shared secret for it.
    $shared_secret = NULL;
    if (isset($application_key)) {
      $shared_secret = culturefeed_get_consumer_shared_secret($application_key);
    }
    try {
      $token = DrupalCultureFeed::getInstance($_GET['oauth_token'], $_SESSION['oauth_token_secret'], $application_key, $shared_secret)->getAccessToken($_GET['oauth_verifier']);
      unset($_SESSION['oauth_token']);
      unset($_SESSION['oauth_token_secret']);
      $cf_account = DrupalCultureFeed::getInstance($token['oauth_token'], $token['oauth_token_secret'], $application_key, $shared_secret)->getUser($token['userId']);
    }
    catch (Exception $e) {
      drupal_set_message('Er is een fout opgetreden bij het inloggen. Probeer later opnieuw.', 'error');
      watchdog_exception('culturefeed', $e);
      drupal_goto();
    }
    //account
    $account = culturefeed_get_or_create_user($token['userId'], $cf_account, $token['oauth_token'], $token['oauth_token_secret'], $application_key);
    if ($account) {
      global $user;
      $user = $account;
      user_login_finalize();
      //check if we have a passholder to redirect later
      try {
        if (isset($account->culturefeed_uid)) {
          $cf = DrupalCultureFeed::getLoggedInUserInstance();
          $cf->uitpas()->getPassholderByUser($account->culturefeed_uid);
        }
      }
      catch (Exception $e) {
        $_SESSION['uitpas_ui_passholder_redirect'] = TRUE;
      }
      //set our redirect
      if (isset($_GET['isnewpassholder'])) {
        drupal_goto('registered');
      }
      else if (isset($_GET['destination'])) {
        drupal_goto($_GET['destination']);
      }
    }
  }
  drupal_goto();
}