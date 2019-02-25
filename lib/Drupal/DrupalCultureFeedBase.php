<?php
/**
 * @class
 * Base class for getting the correct culturefeed layer.
 */
abstract class DrupalCultureFeedBase {

  protected static $consumer_instance;

  protected static $user_instance;

  protected static $logged_in_account;

  protected static $logged_in_user;

  /**
   * @var CultureFeed_HttpClientFactory
   */
  protected static $httpClientFactory;


  public static function isCacheEnabled() {
    return variable_get('culturefeed_cache_status', CULTUREFEED_CACHE_DISABLED) == CULTUREFEED_CACHE_ENABLED;
  }

  public static function isCultureFeedUser($uid = NULL) {
    if (!$uid) {
      $account = self::getLoggedInAccount();
    }
    else {
      $account = user_load($uid);
    }

    if (isset($_GET['uid'])) {
      $query = drupal_get_query_parameters($_GET,array('q', 'uid'));
      drupal_goto('culturefeed/oauth/connect', array('query' => array('destination' => request_path() . '?' . drupal_http_build_query($query), 'skipConfirmation' => 'true')));
    }

    return isset($account->culturefeed_uid) && !empty($account->culturefeed_uid);
  }

  public static function getLoggedInUserId() {
    if (self::getLoggedInAccount() && self::isCultureFeedUser()) {
      return self::getLoggedInAccount()->culturefeed_uid;
    }

    return NULL;
  }

  public static function getLoggedInAccount() {
    if (isset(self::$logged_in_account)) {
      return self::$logged_in_account;
    }

    if (user_is_anonymous()) {
      return NULL;
    }

    global $user;

    self::$logged_in_account = user_load($user->uid);

    return self::$logged_in_account;
  }

  public static function getLoggedInUser($application_key = NULL, $shared_secret = NULL, $reset = FALSE) {
    if (!$application_key) {
      $application_key = variable_get('culturefeed_api_application_key', '');
      $shared_secret = variable_get('culturefeed_api_shared_secret', '');
    }

    if (!$reset && isset(static::$logged_in_user[$application_key])) {
      return static::$logged_in_user[$application_key];
    }

    if (!self::isCultureFeedUser()) {
      return NULL;
    }

    static::$logged_in_user[$application_key] = static::getLoggedInUserInstance($application_key, $shared_secret)->getUser(self::getLoggedInUserId(), TRUE, TRUE);
    self::setAvailableCategories(static::$logged_in_user[$application_key]);

    return static::$logged_in_user[$application_key];
  }

  /**
   * Sets the actor types available in current scope.
   * @param CultureFeed_User $user
   */
  protected static function setAvailableCategories(CultureFeed_User $user) {

    $actortypes = variable_get('culturefeed_pages_actor_types', array());

    $pageMemberships = $user->pageMemberships;
    if (!empty($pageMemberships)) {
      foreach ($user->pageMemberships as $key => $membership) {

        // Get the categories for this page.
        $categories = $membership->page->getCategories();

        // Set a flag to indicate this page can be used.
        $use = FALSE;
        if (!empty($categories)) {
          foreach ($categories as $categoryId) {
            if (in_array($categoryId, $actortypes)) {
              $use = TRUE;
            }
          }
        }

        if (!$use) {
          unset($user->pageMemberships[$key]);
        }
      }
    }

    $pageFollowing = $user->following;

    if (!empty($pageFollowing)) {
      foreach ($user->following as $key => $following) {

        $categories = $following->page->getCategories();

        // Set a flag to indicate this page can be used.
        $use = FALSE;
        if (!empty($categories)) {
          foreach ($categories as $categoryId) {
            if (in_array($categoryId, $actortypes)) {
              $use = TRUE;
            }
          }
        }

        if (!$use) {
          unset($user->following[$key]);
        }
      }
    }

  }

  /**
   * @param string|null $application_key
   * @param string|null $shared_secret
   * @return static
   * @throws Exception
   */
  public static function getLoggedInUserInstance($application_key = NULL, $shared_secret = NULL) {

    if (!$application_key) {
      $application_key = variable_get('culturefeed_api_application_key', '');
      $shared_secret = variable_get('culturefeed_api_shared_secret', '');
    }

    if (isset(self::$user_instance[$application_key])) {
      return self::$user_instance[$application_key];
    }

    $account = self::getLoggedInAccount();

    if (!isset($account->tokens) ||
    !isset($account->tokens[$application_key]) ||
    !isset($account->tokens[$application_key]->token) ||
    !isset($account->tokens[$application_key]->secret)) {
      throw new Exception('Not a valid token set.');
    }

    $token = $account->tokens[$application_key];
    static::$user_instance[$application_key] = static::getInstance($token->token, $token->secret, $application_key, $shared_secret);

    return static::$user_instance[$application_key];
  }

  /**
   * @param string $application_key
   * @param string $shared_secret
   * @return ICultureFeed
   */
  public static function getConsumerInstance($application_key = NULL, $shared_secret = NULL) {
    if (!$application_key) {
      $application_key = variable_get('culturefeed_api_application_key', '');
      $shared_secret = variable_get('culturefeed_api_shared_secret', '');
    }

    if (isset(static::$consumer_instance[$application_key])) {
      return static::$consumer_instance[$application_key];
    }

    static::$consumer_instance[$application_key] = static::getInstance(NULL, NULL, $application_key, $shared_secret);

    return static::$consumer_instance[$application_key];
  }

  /**
   * Get an OAuthClient instance for CultureFeed.
   * @param string $endpoint
   *   Endpoint for the client.
   */
  public static function getOAuthClient($endpoint, $token, $secret, $application_key = NULL, $shared_secret = NULL) {
    if (!$application_key) {
      $application_key = variable_get('culturefeed_api_application_key', '');
      $shared_secret = variable_get('culturefeed_api_shared_secret', '');
    }

    $oauth_client = new CultureFeed_DefaultOAuthClient($application_key, $shared_secret, $token, $secret);

    $oauth_client->setEndpoint($endpoint);

    $http_client_factory = static::getHttpClientFactory();
    if (!$http_client_factory) {
      $http_client = new CultureFeed_DefaultHttpClient();

      // Enable the logging.
      // We only do this when the default HTTP client is used, because
      // the HttpClient interface does not have a enableLogging() method
      // and a logger is actually something that should be injected through
      // dependency injection.
      if (module_exists('culturefeed_devel')) {
        $http_client->enableLogging();
      }
    }
    else {
      $http_client = $http_client_factory->createHttpClient();
    }

    $http_client->setTimeout(variable_get('culturefeed_http_client_timeout', 10));

    if ($http_client instanceof CultureFeed_ProxySupportingClient) {
      $uri = @parse_url($endpoint);
      $proxy_server = variable_get('proxy_server', '');
      if ($proxy_server && (!is_callable('_drupal_http_use_proxy') || _drupal_http_use_proxy($uri['host']))) {

        $http_client->setProxyServer($proxy_server);
        $http_client->setProxyPort(variable_get('proxy_port', 8080));
        $http_client->setProxyUsername(variable_get('proxy_username', ''));
        $http_client->setProxyPassword(variable_get('proxy_password', ''));
      }
    }

    $oauth_client->setHttpClient($http_client);

    return $oauth_client;
  }

  /**
   * @return null|CultureFeed_HttpClientFactory
   */
  public static function getHttpClientFactory() {
    return static::$httpClientFactory;
  }

  /**
   * @param CultureFeed_HttpClientFactory $http_client_factory
   */
  public static function setHttpClientFactory(CultureFeed_HttpClientFactory $http_client_factory) {
    static::$httpClientFactory = $http_client_factory;
  }

}
