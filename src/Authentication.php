<?php

/**
 * @file
 * Contains Drupal\culturefeed\Authentication.
 */

namespace Drupal\culturefeed;

use Drupal\Core\Routing\UrlGenerator;
use Drupal\Core\Entity\EntityManagerInterface;
use Drupal\Core\Entity\Query\QueryFactory;
use Symfony\Component\HttpFoundation\Request;
use Drupal\Core\Language\LanguageInterface;
use Exception;

class Authentication implements AuthenticationInterface {

  /**
   * The culturefeed instance.
   *
   * @var \Drupal\culturefeed\CultureFeedFactoryInterface;
   */
  protected $instance;

  /**
   * The url generator.
   *
   * @var \Drupal\Core\Routing\UrlGenerator;
   */
  protected $urlGenerator;

  /**
   * The user map.
   *
   * @var string;
   */
  protected $userMap;

  /**
   * The entity Manager.
   *
   * @var \Drupal\Core\Entity\EntityManagerInterface;
   */
  protected $entityManager;

  /**
   * The query factory.
   *
   * @var \Drupal\Core\Entity\Query\QueryFactory;
   */
  protected $entityQuery;

  /**
   * Constructs a Authentication object.
   *
   * @param CultureFeedFactoryInterface $instance
   *   The culturefeed instance.
   * @param UrlGenerator $url_generator
   *   The url generator.
   * @param UserMapInterface $user_map
   *   The user map.
   * @param EntityManagerInterface $entity_manager
   *   The entity manger.
   * @param QueryFactory $entity_query
   *   The query factory.
   */
  public function __construct(CultureFeedFactoryInterface $instance, UrlGenerator $url_generator, UserMapInterface $user_map, EntityManagerInterface $entity_manager, QueryFactory $entity_query) {

    $this->instance = $instance;
    $this->urlGenerator = $url_generator;
    $this->userMap = $user_map;
    $this->entityManager = $entity_manager;
    $this->entityQuery = $entity_query;

  }

  /**
   * {@inheritdoc}
   */
  public function connect(Request $request, LanguageInterface $language) {

    $options = array('absolute' => TRUE);
    if ($request->query->get('destination')) {
      $options['query']['destination'] = $request->query->get('destination');
      $request->query->remove('destination');
    }
    $callback_url = $this->urlGenerator->generateFromRoute('culturefeed.oauth.authorize', array(), $options);
    $instance = $this->instance->create();

    // Fetch the request token.
    try {
      $token = $instance->getRequestToken($callback_url);
    }
    catch (Exception $e) {
      drupal_set_message(t('An error occurred while logging in. Please try again later.'), 'error');
      watchdog_exception('culturefeed', $e);
      return '<front>';
    }

    if (!$token) {
      drupal_set_message(t('An error occurred while logging in. Please try again later.'), 'error');
      return '<front>';
    }

    $_SESSION['oauth_token'] = $token['oauth_token'];
    $_SESSION['oauth_token_secret'] = $token['oauth_token_secret'];

    return $instance->getUrlAuthorize($token, $callback_url, NULL, FALSE, NULL, NULL, $language->getId());

  }

  /**
   * {@inheritdoc}
   */
  public function authorize(Request $request) {

    $query = $request->query;
    if ($query->get('oauth_token') && $query->get('oauth_verifier')) {

      try {

        $instance = $this->instance->create($query->get('oauth_token'), $_SESSION['oauth_token_secret']);
        $token = $instance->getAccessToken($query->get('oauth_verifier'));
        unset($_SESSION['oauth_token']);
        unset($_SESSION['oauth_token_secret']);
        $instance = $this->instance->create($token['oauth_token'], $token['oauth_token_secret']);
        $account = $instance->getUser($token['userId']);

      }
      catch (Exception $e) {

        drupal_set_message(t('An error occurred while logging in. Please try again later.'), 'error');
        watchdog_exception('culturefeed', $e);

      }

      $account = $this->userMap->get($account, $token);

      // If a token was passed, save it after deleting a possible previous
      // entry.
      if ($token) {

        $storage = $this->entityManager->getStorage('culturefeed_token');

        $query = $this->entityQuery->get('culturefeed_token')->condition('uitid', $token['userId']);
        $result = $query->execute();
        $entities = $storage->loadMultiple(array_keys($result));
        $storage->delete($entities);

        $consumer = $instance->getConsumer();

        $storage->create(array(
          'uitid' => $token['userId'],
          'token' => $token['oauth_token'],
          'secret' => $token['oauth_token_secret'],
          'application_key' => $consumer->key,
        ))->save();

      }

      if ($account) {
        user_login_finalize($account);
      }

    }

  }

}
