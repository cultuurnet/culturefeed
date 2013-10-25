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
        $membershipPage = $membership->page;
        $cf_pages = self::getConsumerInstance()->pages();
        $page = $cf_pages->getPage($membershipPage->getId());
        $categories = $page->getCategories();

        // Set a flag to indicate this page can be used.
        $use = FALSE;
        foreach ($categories as $categoryId) {
          if (in_array($categoryId, $actortypes)) {
            $use = TRUE;
          }
        }
        if (!$use) {
          unset($user->pageMemberships[$key]);
        }
      }
    }

  }

  /**
   * @param string|null $application_key
   * @param string|null $shared_secret
   * @return ICultureFeed
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

    $http_client = new CultureFeed_DefaultHttpClient();

    // Enable the logging.
    if (module_exists('culturefeed_devel')) {
      $http_client->enableLogging();
    }

    $uri = @parse_url($endpoint);
    $proxy_server = variable_get('proxy_server', '');
    if ($proxy_server && _drupal_http_use_proxy($uri['host'])) {

      $http_client->setProxyServer($proxy_server);
      $http_client->setProxyPort(variable_get('proxy_port', 8080));
      $http_client->setProxyUsername(variable_get('proxy_username', ''));
      $http_client->setProxyPassword(variable_get('proxy_password', ''));

    }

    $oauth_client->setHttpClient($http_client);

    return $oauth_client;
  }

}