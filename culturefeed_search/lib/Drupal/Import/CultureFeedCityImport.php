<?php
/**
 * @class
 *  Import class for all the culturefeed domains and the terms.
 */

use \Guzzle\Http\Client;

class CultureFeedCityImport {

  const END_POINT = 'http://taxonomy.uitdatabank.be/api/';

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

    $this->client = new Client($this::END_POINT);

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

}