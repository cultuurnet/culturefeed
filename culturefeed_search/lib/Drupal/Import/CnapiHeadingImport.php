<?php
/**
 * @class
 *  Import class for all the culturefeed domains and the terms.
 */

use \Guzzle\Http\Client;

class CnapiHeadingImport {

  const END_POINT = 'http://build.uitdatabank.be/lib/1.1/';

  /**
   * Guzzle http client.
   * @var \Guzzle\Http\Client
   */
  private $client;

  /**
   * Import the culturefeed domains.
   */
  public function import() {

    $this->client = new Client($this::END_POINT);

    try {

      $body = $this->client->get('heading_categorisation.xml')->send()->getBody(TRUE);
      $this->importHeadings(new SimpleXMLElement($body));

    }
    catch (ClientErrorResponseException $e) {
      watchdog_exception('culturefeed_cnapi_import', $e);
    }

  }

  /**
   * Import all the cities from the xml.
   */
  private function importHeadings($xmlElement) {

    // Clear them first.
    db_query('TRUNCATE {cnapi_headings}');

    // Import rows.
    $headings = $xmlElement->xpath('heading_categorisation');
    foreach ($headings as $heading) {

      $headingAttributes = $heading->attributes();
      $record = array(
        'hid' => (int) $headingAttributes['heading_id'],
        'cid' => (string) $headingAttributes['cnet_id'],
      );

      drupal_write_record('cnapi_headings', $record);

    }

  }

}