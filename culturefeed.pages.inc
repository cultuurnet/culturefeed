<?php

function culturefeed_oauth_connect($type = CultureFeed::AUTHORIZE_TYPE_REGULAR, $application_key = NULL) {
  global $conf;

  $conf['cache'] = FALSE;

  $shared_secret = NULL;

  if (isset($application_key)) {
    $shared_secret = culturefeed_get_consumer_shared_secret($application_key);
  }

  $cf = DrupalCultureFeed::getConsumerInstance($application_key, $shared_secret);

  $options = array('absolute' => TRUE);

  if (isset($_GET['destination'])) {
    $options['query']['destination'] = $_GET['destination'];
    unset($_GET['destination']);
  }

  if (isset($_GET['popup'])) {
    $options['query']['popup'] = 'true';
  }

  $callback_url = url('culturefeed/oauth/authorize' . (isset($application_key) ? '/' . $application_key : ''), $options);

  try {
    $token = $cf->getRequestToken($callback_url);
  }
  catch (Exception $e) {
    drupal_set_message('Er is een fout opgetreden bij het inloggen. Probeer later opnieuw.', 'error');
    watchdog_exception('culturefeed', $e);

    drupal_goto('<front>');
  }

  if (!$token) {
    return;
  }

  $_SESSION['oauth_token'] = $token['oauth_token'];
  $_SESSION['oauth_token_secret'] = $token['oauth_token_secret'];

  $auth_url = $cf->getUrlAuthorize($token, '', $type);

  drupal_goto($auth_url);
}

function culturefeed_oauth_authorize($application_key = NULL) {
  global $conf;

  $conf['cache'] = FALSE;

  if (isset($_GET['oauth_token']) && isset($_GET['oauth_verifier'])) {
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

      drupal_goto('<front>');
    }

    $account = culturefeed_get_or_create_user($token['userId'], $cf_account->nick, $token['oauth_token'], $token['oauth_token_secret'], $application_key);

    if ($account) {
      global $user;
      $user = $account;

      user_login_finalize();

      if (isset($_GET['popup'])) {
        // We don't want to render all blocks, so we return minimal html.
        print '<html>';
        print '<head>';
        print '<title>You should not see this</title>';
        print '<script type="text/javascript">try { window.opener.location.reload(); } catch (err) { } window.close();</script>';
        print '</head>';
        print '<body>';
        print '</body>';
        print '</html>';
      }
      else {
        if (isset($_GET['destination'])) {
          drupal_goto($_GET['destination']);
        }
        else {
          drupal_goto('');
        }
      }
    }
  }

  drupal_page_footer();

  exit();
}