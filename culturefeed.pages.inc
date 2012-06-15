<?php

function culturefeed_oauth_connect($type = CultureFeed::AUTHORIZE_TYPE_REGULAR, $application_key = NULL) {
  global $conf;

  // We don't want to cache this type of page.
  $conf['cache'] = FALSE;

  // If an application key was passed, fetch the shared secret for it.
  $shared_secret = NULL;

  if (isset($application_key)) {
    $shared_secret = culturefeed_get_consumer_shared_secret($application_key);
  }

  // Load the consumer instance.
  $cf = DrupalCultureFeed::getConsumerInstance($application_key, $shared_secret);

  // Construct the callback url.
  $options = array('absolute' => TRUE);

  if (isset($_GET['destination'])) {
    $options['query']['destination'] = $_GET['destination'];
    unset($_GET['destination']);
  }

  if (isset($_GET['closepopup'])) {
    $options['query']['closepopup'] = 'true';
  }

  $callback_url = url('culturefeed/oauth/authorize' . (isset($application_key) ? '/' . $application_key : ''), $options);

  // Fetch the request token.
  try {
    $token = $cf->getRequestToken($callback_url);
  }
  catch (Exception $e) {
    drupal_set_message('Er is een fout opgetreden bij het inloggen. Probeer later opnieuw.', 'error');
    watchdog_exception('culturefeed', $e);

    drupal_goto('<front>');
  }

  if (!$token) {
    drupal_set_message('Er is een fout opgetreden bij het inloggen. Probeer later opnieuw.', 'error');
    drupal_goto('<front>');
  }

  // Save the token and secret in the session.
  $_SESSION['oauth_token'] = $token['oauth_token'];
  $_SESSION['oauth_token_secret'] = $token['oauth_token_secret'];

  // Fetch the authorisation url...
  $auth_url = $cf->getUrlAuthorize($token, $callback_url, $type);

  // ... and redirect the user to it.
  drupal_goto($auth_url);
}

function culturefeed_oauth_authorize($application_key = NULL) {
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

    $account = culturefeed_get_or_create_user($token['userId'], $cf_account, $token['oauth_token'], $token['oauth_token_secret'], $application_key);

    if ($account) {
      global $user;
      $user = $account;

      user_login_finalize();

      if (isset($_GET['closepopup'])) {
        // We don't want to render all blocks, so we return minimal html.
        print '<html>';
        print '<head>';
        print '<title>You should not see this</title>';
        print '<script type="text/javascript">try { window.opener.location.reload(); } catch (err) { } window.close();</script>';
        print '<p>Als deze pagina niet automatisch sluit, klik dan op onderstaande link om verder te gaan.</p>';
        print '<p><a href="' . url('') . '">' . url('', array('absolute' => TRUE)) . '</a></p>';
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
          drupal_goto();
        }
      }
    }
  }

  drupal_page_footer();

  exit();
}