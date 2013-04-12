<?php
/**
 * @class
 *  Import class for all the culturefeed domains and the terms.
 */

use \Guzzle\Http\Client;

class CultureFeedDomainImport {

  const END_POINT = 'http://test.rest.uitdatabank.be/api/';

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

      // Domains.
      $body = $this->client->get('domain')->send()->getBody(TRUE);
      $this->importDomains(new SimpleXMLElement($body));

      // Terms.
      $body = $this->client->get('term')->send()->getBody(TRUE);
      $this->importTerms(new SimpleXMLElement($body));

    }
    catch (ClientErrorResponseException $e) {
      watchdog_exception('culturefeed_domain_import', $e);
    }

  }

  /**
   * Import all the domains from the xml.
   */
  private function importDomains($xmlElement) {

    // Clear them first.
    db_query('TRUNCATE {culturefeed_search_domains}');

    // Import rows.
    foreach ($xmlElement->categorisation->domain as $domain) {

      $domainAttributes = $domain->attributes();
      $record = array(
        'did' => (string)$domainAttributes['id'],
        'label' => (string) $domainAttributes['label'],
      );

      drupal_write_record('culturefeed_search_domains', $record);
      drush_log('Imported domain ' . (string) $domainAttributes['label'], 'success');

    }

  }

  /**
   * Import all the terms.
   */
  private function importTerms($xmlElement) {

    // Clear them first.
    db_query('TRUNCATE {culturefeed_search_terms}');

    foreach ($xmlElement->categorisation->term as $term) {

      $termAttributes = $term->attributes();
      $record = array(
        'tid' => (string) $termAttributes['id'],
        'name' => (string) $termAttributes['label'],
        'did' => (string) $termAttributes['domain'],
      );

      drupal_write_record('culturefeed_search_terms', $record);
      drush_log('Imported term ' . (string) $termAttributes['label'], 'success');

    }


  }

}