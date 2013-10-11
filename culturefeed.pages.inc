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

  if (isset($_GET['dob'])) {
    $options['query']['dob'] = $_GET['dob'];
  }

  // Add the language parameter.
  $options['query']['lang'] = culturefeed_get_preferred_language();

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

  $via = NULL;
  if (isset($_GET['via'])) {
    $via = $_GET['via'];
  }

  $skip_authorization = FALSE;
  if (isset($_GET['skipAuthorization'])) {
    $skip_authorization = TRUE;
  }

  // Fetch the authorisation url...
  $auth_url = $cf->getUrlAuthorize($token, $callback_url, $type, FALSE, $skip_authorization, $via);

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

    // Check if the user is already known in our system.
    $uid = db_query("SELECT uid FROM {culturefeed_user} cfu WHERE cfu.cf_uid = :cf_uid", array(':cf_uid' => $token['userId']))->fetchField();
    if (!$uid) {
      $account = culturefeed_create_user($cf_account);
    }
    else {
      $account = user_load($uid);
    }

    // If a token was passed, save it after deleting a possible previous entry.
    if ($token) {

      if (!isset($application_key)) {
        $application_key = variable_get('culturefeed_api_application_key', '');
      }

      db_delete('culturefeed_token')
      ->condition('cf_uid', $token['userId'])
      ->condition('application_key', $application_key)
      ->execute();

      db_insert('culturefeed_token')
      ->fields(array(
      'cf_uid' => $token['userId'],
      'token' => $token['oauth_token'],
      'secret' => $token['oauth_token_secret'],
      'application_key' => $application_key,
      ))
      ->execute();
    }

    if ($account) {

      global $user;
      $user = $account;

      user_login_finalize();

      if (isset($_GET['closepopup'])) {

        if (isset($_GET['destination'])) {
          $action = 'window.opener.location.href="' . url($_GET['destination'], array('absolute' => TRUE)) . '";';
        }
        else {

          $destination = '';
          drupal_alter('culturefeed_login_redirect_destination', $destination);

          if (!empty($destination)) {
            $action = 'window.opener.location.href="' . url($destination, array('absolute' => TRUE)) . '";';
          }
          else {
            $action = 'window.opener.location.reload();';
          }

        }

        // We don't want to render all blocks, so we return minimal html.
        print '<html>';
        print '<head>';
        print '<title>You should not see this</title>';
        print '<script type="text/javascript">try { ' . $action . ' } catch (err) { } window.close();</script>';
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
          $destination = '';
          $destination = drupal_alter('culturefeed_login_redirect_destination', $destination);
          drupal_goto($destination);
        }
      }
    }
  }

  drupal_page_footer();

  exit();
}