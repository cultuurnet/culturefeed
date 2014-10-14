<?php

namespace Drupal\culturefeed\Controller;

use Drupal\Core\Controller\ControllerBase;
use Symfony\Component\HttpFoundation\RedirectResponse;
use DrupalCultureFeed;

class CulturefeedController extends ControllerBase {

  public function connect() {

    $cf = DrupalCultureFeed::getConsumerInstance();
    $callback_url = $this->url('culturefeed.oauth.authorize', array(), array('absolute' => TRUE));

    // Fetch the request token.
    try {
      $token = $cf->getRequestToken($callback_url);
    }
    catch (Exception $e) {
      drupal_set_message(t('An error occurred while logging in. Please try again later.'), 'error');
      watchdog_exception('culturefeed', $e);
      $this->redirect('<front>');
    }

    if (!$token) {
      drupal_set_message(t('An error occurred while logging in. Please try again later.'), 'error');
      $this->redirect('<front>');
    }

    // Save the token and secret in the session.
    $_SESSION['oauth_token'] = $token['oauth_token'];
    $_SESSION['oauth_token_secret'] = $token['oauth_token_secret'];

    // Fetch the authorisation url...
    $auth_url = $cf->getUrlAuthorize($token, $callback_url, NULL, FALSE, NULL, NULL, 'nl', $this->config('culturefeed.api')->get('application_key'));

    // ... and redirect the user to it.
    return new RedirectResponse($auth_url, 302);

  }

  public function authorize() {
    return $this->redirect('culturefeed.api');
  }

}