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
    if ($use_cache && variable_get('culturefeed_curator_api_cache_enabled', TRUE) && !self::$cachedCuratorClient) {
      self::$cachedCuratorClient = new DrupalCultureFeedCuratorClient_Cache(new DrupalCultureFeedCuratorClient($use_cache), DrupalCultureFeed::getLoggedInUserId());
    }
    elseif (!$use_cache && !self::$curatorClient) {
      self::$curatorClient = new DrupalCultureFeedCuratorClient($use_cache);
    }

    return $use_cache ? self::$cachedCuratorClient : self::$curatorClient;
  }

  /**
   * Get external articles for a given CDB ID.
   *
   * @param $cdb_id
   *
   * @return \CultureFeed_CuratorArticle[]
   */
  public function getExternalArticlesForCdbItem($cdb_id) {

    $request = $this->client->get('news_articles', null, [
      'query' => ['about' => $cdb_id]
    ]);

    $response = $request->send();

    $json = $response->getBody(TRUE);

    $result = json_decode($json);
    $results = !empty($result->{'hydra:member'}) ? $result->{'hydra:member'} : [];

    $articles = [];
    $properties = array_keys(get_class_vars(CultureFeed_CuratorArticle::class));
    foreach ($results as $result) {
      $article = new CultureFeed_CuratorArticle();
      foreach ($properties as $property) {
        $article->{$property} = !empty($result->{$property}) ? $result->{$property} : '';
      }
      $articles[] = $article;
    }

    return $articles;
  }

}
