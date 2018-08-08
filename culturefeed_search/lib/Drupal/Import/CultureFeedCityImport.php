<?php
/**
 * @class
 *  Import class for all the culturefeed domains and the terms.
 */

use \Guzzle\Http\Client;

class CultureFeedCityImport {

  /**
   * Guzzle http client.
   * @var \Guzzle\Http\Client
   */
  private $client;

  public $logMessages = array();

  /**
   * Import the culturefeed domains.
   */
  public function import() {

    $this->client = new Client($this->getEndpoint());

    try {

      // Cities.
      $body = $this->client->get('city')->send()->getBody(TRUE);
      $this->importCities(new SimpleXMLElement($body));

    }
    catch (ClientErrorResponseException $e) {
      watchdog_exception('culturefeed_city_import', $e);
    }

  }

  /**
   * Import all the cities from the xml.
   */
  private function importCities($xmlElement) {

    // Clear them first.
    db_query('TRUNCATE {culturefeed_search_cities}');

    // Import rows.
    foreach ($xmlElement->cities->city as $city) {

      $cityAttributes = $city->attributes();
      $record = array(
        'cid' => (string) $cityAttributes['id'],
        'name' => (string) $cityAttributes['label'],
        'zip' => (string) $cityAttributes['zip'],
        'slug'  => (string) $cityAttributes['zip'] . '-' . culturefeed_search_slug((string) $cityAttributes['label'], 251),
      );

      drupal_write_record('culturefeed_search_cities', $record);
      $this->logMessages[] = array(
        'message' => 'Imported city ' . (string) $cityAttributes['label'],
        'code' => 'success'
      );

    }

  }

  /**
   * Get the Culturefeed search taxonomy endpoint.
   */
  private function getEndpoint() {
    return variable_get('culturefeed_search_taxonomy_endpoint', CULTUREFEED_SEARCH_TAXONOMY_ENDPOINT);
  }

}