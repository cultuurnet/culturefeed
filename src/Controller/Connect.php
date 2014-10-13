<?php

namespace Drupal\culturefeed\Controller;

use Drupal\Core\Controller\ControllerBase;

class Connect extends ControllerBase {

  public function login() {

    $cf = DrupalCultureFeed::getConsumerInstance($application_key, $shared_secret);

    $callback_url = url('culturefeed/oauth/authorize');

    // Fetch the request token.
    try {
      $token = $cf->getRequestToken($callback_url);
    }
    catch (Exception $e) {
      drupal_set_message(t('An error occurred while logging in. Please try again later.'), 'error');
      watchdog_exception('culturefeed', $e);

      drupal_goto('<front>');
    }

    if (!$token) {
      drupal_set_message(t('An error occurred while logging in. Please try again later.'), 'error');
      drupal_goto('<front>');
    }

    // Save the token and secret in the session.
    $_SESSION['oauth_token'] = $token['oauth_token'];
    $_SESSION['oauth_token_secret'] = $token['oauth_token_secret'];

    // Fetch the authorisation url...
    $auth_url = $cf->getUrlAuthorize($token, $callback_url, NULL, FALSE, NULL, NULL, 'nl', variable_get('culturefeed_api_application_key', ''));

    // ... and redirect the user to it.
    drupal_goto($auth_url);

  }

}