<?php

/**
 * @file
 * Contains Drupal\culturefeed\Authentication.
 */

namespace Drupal\culturefeed;

use DrupalCultureFeed;
use CultureFeed;
use Drupal\Core\Routing\UrlGenerator;
use Drupal\Core\Config\ConfigFactory;
use Exception;

class Authentication implements AuthenticationInterface {

  /**
   * The drupal culturefeed.
   *
   * @var \DrupalCultureFeed;
   */
  protected $drupalCultureFeed;

  /**
   * The culturefeed consumer instance.
   *
   * @var \Culturefeed;
   */
  protected $consumerInstance;

  /**
   * The url generator.
   *
   * @var \Drupal\Core\Routing\UrlGenerator;
   */
  protected $urlGenerator;

  /**
   * The config factory.
   *
   * @var \Drupal\Core\Config\ConfigFactory;
   */
  protected $config;

  /**
   * The application key.
   *
   * @var string;
   */
  protected $applicationKey;

  /**
   * The user map.
   *
   * @var string;
   */
  protected $userMap;

  /**
   * Constructs a Authentication object.
   *
   * @param DrupalCultureFeed $drupal_culturefeed
   *   The drupal culturefeed.
   * @param UrlGenerator $url_generator
   *   The url generator.
   * @param ConfigFactory $config_factory
   *   The config factory.
   * @param UserMapInterface $user_map
   *   The user map.
   */
  public function __construct(DrupalCultureFeed $drupal_culturefeed, UrlGenerator $url_generator, ConfigFactory $config_factory, UserMapInterface $user_map) {

    $this->drupalCultureFeed = $drupal_culturefeed;
    $this->consumerInstance = $this->drupalCultureFeed->getConsumerInstance();
    $this->urlGenerator = $url_generator;
    $this->config = $config_factory->get('culturefeed.api');
    $this->applicationKey = $this->config->get('application_key');
    $this->userMap = $user_map;

  }

  /**
   * {@inheritdoc}
   */
  public function connect() {

    $callback_url = $this->urlGenerator->generateFromRoute('culturefeed.oauth.authorize', array(), array('absolute' => TRUE));

    // Fetch the request token.
    try {
      $token = $this->consumerInstance->getRequestToken($callback_url);
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

    return $this->consumerInstance->getUrlAuthorize($token, $callback_url, NULL, FALSE, NULL, NULL, 'nl', $this->applicationKey);

  }

  /**
   * {@inheritdoc}
   */
  public function authorize() {

    if (isset($_GET['oauth_token']) && isset($_GET['oauth_verifier'])) {

      try {

        $token = $this->drupalCultureFeed->getInstance($_GET['oauth_token'], $_SESSION['oauth_token_secret'], '', NULL)->getAccessToken($_GET['oauth_verifier']);
        unset($_SESSION['oauth_token']);
        unset($_SESSION['oauth_token_secret']);
        $account = $this->drupalCultureFeed->getInstance($token['oauth_token'], $token['oauth_token_secret'], '', NULL)->getUser($token['userId']);

      }
      catch (Exception $e) {

        drupal_set_message(t('An error occurred while logging in. Please try again later.'), 'error');
        watchdog_exception('culturefeed', $e);

      }

      $account = $this->userMap->get($account, $token);

      // If a token was passed, save it after deleting a possible previous
      // entry.
      if ($token) {

        $query = \Drupal::entityQuery('culturefeed_token')
          ->condition('uitid', $token['userId']);
        $result = $query->execute();
        entity_delete_multiple('culturefeed_token', array_keys($result));

        entity_create('culturefeed_token', array(
          'uitid' => $token['userId'],
          'token' => $token['oauth_token'],
          'secret' => $token['oauth_token_secret'],
          'application_key' => $this->applicationKey,
        ))->save();

      }

      if ($account) {
        user_login_finalize($account);
      }

    }

  }

}
