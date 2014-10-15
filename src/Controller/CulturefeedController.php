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

  public function authorize($application_key = NULL) {

    if (isset($_GET['oauth_token']) && isset($_GET['oauth_verifier'])) {

      // If an application key was passed, fetch the shared secret for it.
      $shared_secret = NULL;

      try {
        $token = DrupalCultureFeed::getInstance($_GET['oauth_token'], $_SESSION['oauth_token_secret'], $application_key, $shared_secret)->getAccessToken($_GET['oauth_verifier']);

        unset($_SESSION['oauth_token']);
        unset($_SESSION['oauth_token_secret']);

        $cf_account = DrupalCultureFeed::getInstance($token['oauth_token'], $token['oauth_token_secret'], $application_key, $shared_secret)->getUser($token['userId']);
      }
      catch (Exception $e) {
        drupal_set_message(t('An error occurred while logging in. Please try again later.'), 'error');
        watchdog_exception('culturefeed', $e);

        drupal_goto();
      }

      $user_service = \Drupal::service('culturefeed.user');
      $account = $user_service->setUser($cf_account, $token);

      // If a token was passed, save it after deleting a possible previous entry.
      if ($token) {

        if (!isset($application_key)) {
          $application_key = $this->config('culturefeed.api')->get('application_key');
        }

        $query = \Drupal::entityQuery('culturefeed_token')
          ->condition('uitid', $token['userId']);
        $result = $query->execute();
        entity_delete_multiple('culturefeed_token', array_keys($result));

        entity_create('culturefeed_token', array(
          'uitid' => $token['userId'],
          'token' => $token['oauth_token'],
          'secret' => $token['oauth_token_secret'],
          'application_key' => $application_key,
        ))->save();

      }

      if ($account) {
        user_login_finalize($account);
      }

      return $this->redirect('<front>');

    }

  }

}
