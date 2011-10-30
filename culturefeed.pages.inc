<?php

function culturefeed_oauth_connect($type = CultureFeed::AUTHORIZE_TYPE_REGULAR) {
  $cf = DrupalCultureFeed::getConsumerInstance();

  $options = array('absolute' => TRUE);

  if (isset($_GET['destination'])) {
    $options['query']['destination'] = $_GET['destination'];
    unset($_GET['destination']);
  }

  if (isset($_GET['popup'])) {
    $options['query']['popup'] = 'true';
  }

  $callback_url = url('culturefeed/oauth/authorize', $options);

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

function culturefeed_oauth_authorize() {
  if (isset($_GET['oauth_token']) && isset($_GET['oauth_verifier'])) {
    try {
      $token = DrupalCultureFeed::getInstance($_GET['oauth_token'], $_SESSION['oauth_token_secret'])->getAccessToken($_GET['oauth_verifier']);
      
      unset($_SESSION['oauth_token']);
      unset($_SESSION['oauth_token_secret']);
      
      $cf_account = DrupalCultureFeed::getInstance($token['oauth_token'], $token['oauth_token_secret'])->getUser($token['userId']);
    }
    catch (Exception $e) {
      drupal_set_message('Er is een fout opgetreden bij het inloggen. Probeer later opnieuw.', 'error');
      watchdog_exception('culturefeed', $e);
      
      drupal_goto('<front>');
    }

    $account = culturefeed_get_or_create_user($token['userId'], $cf_account->nick, $token['oauth_token'], $token['oauth_token_secret']);

    if ($account) {
      global $user;
      $user = $account;

      user_login_finalize();

      if (isset($_GET['popup'])) {
        // We don't want to render all blocks, so we return minimal html.
        print '<html>';
        print '<head>';
        print '<title>You should not see this</title>';
        print '<script type="text/javascript">window.opener.location.reload(); window.close();</script>';
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