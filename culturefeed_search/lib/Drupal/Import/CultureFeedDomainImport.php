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

  public $logMessages = array();

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
      $this->logMessages[] = array(
        'message' => 'Imported domain ' . (string) $domainAttributes['label'],
        'code' => 'success'
      );

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
      $parentId = trim((string) $termAttributes['parentid']);
      $record = array(
        'tid' => (string) $termAttributes['id'],
        'language' => LANGUAGE_NONE,
        'name' => (string) $termAttributes['label'],
        'did' => (string) $termAttributes['domain'],
        'parent' => empty($parentId) ? NULL : $parentId,
      );

      // Always save label as undefined language so we can fallback.
      $this->logMessages[] = array(
        'message' => 'Imported term ' . $record['name'] . ' ' . $parentId,
        'code' => 'success'
      );
      drupal_write_record('culturefeed_search_terms', $record);

      // Import other languages.
      foreach (array('nl', 'en', 'de', 'fr') as $language) {
        $key = 'label' . $language;
        if (isset($termAttributes[$key]) && !empty($termAttributes[$key])) {
          $record['language'] = $language;
          $record['name'] = (string) $termAttributes[$key];
          drupal_write_record('culturefeed_search_terms', $record);

          $this->logMessages[] = array(
            'message' => 'Imported term ' . $record['name'] . ' ' . $parentId . ' in language ' . $record['language'],
            'code' => 'success'
          );

        }
      }
    }

  }

}
