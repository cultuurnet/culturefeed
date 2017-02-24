<?php
/**
 * @class
 *  Import class for all the culturefeed domains and the terms.
 */

use \Guzzle\Http\Client;

class CultureFeedDomainImport {

  const END_POINT = 'http://taxonomy.uitdatabank.be/api/';

  /**
   * Guzzle http client.
   * @var \Guzzle\Http\Client
   */
  protected $client;
  public $logMessages = array();

  /**
   * Import the culturefeed domains.
   */
  public function import() {

    $this->client = new Client($this::END_POINT);

    try {

      // Domains.
      $body = $this->client->get('domain')->send()->getBody(TRUE);
      $domains = $this->importDomains(new SimpleXMLElement($body));

      // Clear them first.
      db_query('TRUNCATE {culturefeed_search_terms}');

      // Import terms for every domain.
      foreach ($domains as $did) {
        $body = $this->client->get('domain/' . $did . '/classification')->send()->getBody(TRUE);
        $xmlElement = new SimpleXMLElement($body);
        $this->importTerms($xmlElement->categorisation->term);
      }
    }
    catch (ClientErrorResponseException $e) {
      watchdog_exception('culturefeed_domain_import', $e);
    }

  }

  /**
   * Import all the domains from the xml.
   */
  private function importDomains($xmlElement) {

    $domains = array();
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

      $domains[] = $record['did'];
    }

    return $domains;
  }

  /**
   * Import all the terms with given pids as parents.
   */
  private function importTerms($TermsElement, $pids = array()) {

    $parents = array();

    foreach ($TermsElement as $term) {

      $termAttributes = $term->attributes();

      // Do not import disabled terms
      if ($termAttributes['enabled'] == 'false' && drupal_is_cli() && function_exists('drush_main'))  {
        drush_log('skipped disabled term: ' . $termAttributes['id'], 'warning');
        continue;
      }

      // If it has children, also import them.
      if (isset($term->term)) {
        $child_pids = $pids;
        $child_pids['p' . (count($pids) + 1)] = (string) $termAttributes['id'];
        $this->importTerms($term->term, $child_pids);
      }

      $parentId = trim((string) $termAttributes['parentid']);
      $parents[(string) $termAttributes['id']] = $parentId;

      $record = array(
        'tid' => (string) $termAttributes['id'],
        'language' => LANGUAGE_NONE,
        'name' => (string) $termAttributes['label'],
        'did' => (string) $termAttributes['domain'],
        'parent' => empty($parentId) ? $parentId : NULL,
        'show_term' => $termAttributes['show'] == 'true' ? 1 : 0,
        'slug' => culturefeed_search_slug((string) $termAttributes['label'], 128),
      );

      $record += $pids;

      // Get all the parent ids (p1, p2, p3...)
      /* if (!empty($parentId)) {
        $parentWasTerm = TRUE;
        $currTerm = $term;
        $termParents = array();

        // Loop through parents untill we find a non-term node.
        while ($parentWasTerm) {

        // Get parent of current term, and check if it's also a term.
        $parent_node = $currTerm->xpath('parent::*');
        $parent = $parent_node[0];
        if (!$parent) {
        break;
        }

        if ($parent->getName() == 'term') {
        $termParents[] = (string) $parent->attributes()->id;
        $currTerm = $parent;
        }
        else {
        $parentWasTerm = FALSE;
        }
        }

        foreach ($termParents as $i => $id) {
        $record['p' . ($i + 1)] = $id;
        }

        } */

      // Always save label as undefined language so we can fallback.
      $this->logMessages[] = array(
        'message' => 'Imported term ' . $record['name'] . ' ' . $parentId . ' for domain ' . $record['did'],
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
