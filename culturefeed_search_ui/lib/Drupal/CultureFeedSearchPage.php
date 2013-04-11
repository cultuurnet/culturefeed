<?php

use \CultuurNet\Search\Parameter;

/**
 * @file
 * Base class for search pages.
 */
class CultureFeedSearchPage {

  /**
   * Start index from items currently shown.
   * @var int
   */
  protected $start = 0;

  /**
   * Total results per page.
   * @var int
   */
  protected $resultsPerPage = 10;

  /**
   * List of paramaters to be given to the search.
   * @var \CultuurNet\Search\Parameter\AbstractParameter[]
   */
  protected $parameters = array();

  /**
   * Query to search on.
   * @var array
   */
  protected $query = array();


  /**
   * Search result from current search.
   * @var \CultuurNet\Search\SearchResult
   */
  protected $result;

  /**
   * Loads a search page.
   */
  public function loadPage() {
    return array();
  }

  /**
   * Add the active facet filters to the parameters.
   */
  protected function addFacetFilters($params) {

    // Add the date range facet.
    if (isset($params['date_range'])) {

      $dates = explode('-', $params['date_range']);
      $start_date = strtotime($dates[0]);
      $end_date = $start_date;
      if (isset($dates[1])) {
        $end_date = strtotime($dates[1]) +  24 * 60 * 60;
      }

      // Add 23:59:59 to the end date, so it searches on the end of that day
      $end_date +=  ((23 * 60 * 60) + (59 * 60) + 59);

      $this->parameters[] = new Parameter\DateRangeFilterQuery('startdate', $start_date, $end_date);

    }

    foreach ($params['facet'] as $facetFieldName => $facetFilter) {

      // Datetype is not a real facet, but a search field.
      if ($facetFieldName == 'datetype') {
        $facetFilterQuery = new Parameter\DateTypeQuery(implode(' OR ', $facetFilter));
      }
      else {

        array_walk($facetFilter, function (&$item) {
          $item = '"' . str_replace('"', '\"', $item) . '"';
        });
        $facetFilterQuery = new Parameter\FilterQuery($facetFieldName . ':(' . implode(' OR ', $facetFilter) . ')');
        // tag this so we can exclude when calculating facets
        $facetFilterQuery->setTags(array($facetFieldName));

      }

      $this->parameters[] = $facetFilterQuery;

    }

  }

  /**
   * Execute the search for current page.
   */
  protected function execute($params, $facetingComponent) {

    // Add start index (page number we want)
    $this->start = $params['page'] * $this->resultsPerPage;
    $this->parameters[] = new Parameter\Start($this->start);

    // Add items / page.
    $this->parameters[] = new Parameter\Rows($this->resultsPerPage);

    // Add grouping so returned events are not duplicate.
    $this->parameters[] = new Parameter\Group();

    if ('' == $params['search']) {
      $params['search'] = '*';
    }
    $this->query[] = 'text:(' . str_replace(')', '\\)', $params['search']) . ')';

    $this->parameters[] = new Parameter\Query(implode(' AND ', $this->query));

    drupal_alter('culturefeed_search_query', $this->parameters, $this->query);

    $searchService = culturefeed_get_search_service();
    $this->result = $searchService->search($this->parameters);
    $facetingComponent->obtainResults($this->result);

  }

  /**
   * Get the build from current search page.
   */
  protected function build() {

    pager_default_initialize($this->result->getTotalCount(), $this->resultsPerPage);

    $build = array();

    $build['results'] = array(
      '#theme' => 'culturefeed_search_page',
      '#searchresult' => $this->result,
    );

    if ($this->result->getTotalCount() > 0) {

      $build['pager-container'] =  array(
        '#type' => 'container',
        '#attributes' => array(),
        'pager_summary' => array(
          '#theme' => 'culturefeed_search_pager_summary',
          '#result' => $this->result,
          '#start' => $this->start,
        ),
        'pager' => array(
          '#theme' => 'pager',
          '#quantity' => 5
        ),
      );
    }

    drupal_set_title($this->getDrupalTitle());

    return $build;

  }

}
