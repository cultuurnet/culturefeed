<?php
/**
 * @file
 * DrupalCultureFeedCuratorClient
 */

use Guzzle\Http\Client;

/**
 * Singleton for the CultureFeed Curator Client.
 */
class DrupalCultureFeedCuratorClient {

  /**
   * @var DrupalCultureFeedCuratorClient
   */
  private static $curatorClient = NULL;

  /**
   * @var DrupalCultureFeedCuratorClient
   */
  private static $cachedCuratorClient = NULL;

  /**
   * @var Guzzle\Http\Client
   */
  private $client = NULL;

  /**
   * Constructor
   *
   * @param bool $use_cache
   */
  private function __construct($use_cache) {
    $endpoint = variable_get(
      'culturefeed_curator_api_endpoint',
      CULTUREFEED_CURATOR_API_ENDPOINT
    );

    $this->client = new Client($endpoint);
  }

  /**
   * getClient().
   *
   * @param bool $use_cache
   * @return \DrupalCultureFeedCuratorClient
   */
  public static function getClient($use_cache = TRUE) {
    if ($use_cache && !self::$cachedCuratorClient) {
      self::$cachedCuratorClient = new DrupalCultureFeedCuratorClient($use_cache);
    } else {
      self::$curatorClient = new DrupalCultureFeedCuratorClient($use_cache);
    }

    return $use_cache ? self::$cachedCuratorClient : self::$curatorClient;
  }

  public function getExternalArticlesForCdbItem($cdb_id) {

    $request = $this->client->get('news_articles', null, [
      'query' => ['about' => $cdb_id]
    ]);

    $response = $request->send();

    $json = $response->getBody(TRUE);

    // TODO: Define result object
    $results= json_decode($json)->{'hydra:member'};

    return $results;

  }

}