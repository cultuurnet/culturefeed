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
      $startDate = DateTime::createFromFormat('d/m/Y', trim($dates[0]));
      $endDate = $startDate;
      if (isset($dates[1])) {
        $endDate = DateTime::createFromFormat('d/m/Y', trim($dates[1]));
      }

      // Set start date time on beginning of the day.
      $startDate->setTime(0, 0, 1);

      // Set end date time to end of the day day, to it searches on full day.
      $endDate->setTime(23, 59, 59);

      $this->parameters[] = new Parameter\DateRangeFilterQuery('startdate', $startDate->getTimestamp(), $endDate->getTimestamp());

    }

    // Add search on coordinates.
    if (isset($params['coordinates'])) {

      $coordinates = explode(',', $params['coordinates']);

      $distance = new Parameter\Spatial\Distance(CULTUREFEED_SEARCH_DEFAULT_PROXIMITY_RANGE);
      $point = new Parameter\Spatial\Point($coordinates[0], $coordinates[1]);
      $field = new Parameter\Spatial\SpatialField('physical_gis');
      $this->parameters[] = new Parameter\Spatial\GeoFilterQuery($point, $distance, $field);

    }

    // Add the location facet.
    if (isset($params['location'])) {

      $type = isset($params['type']) ? $params['type'] : 'city';

      if ($type == 'city') {

      }

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
      $params['search'] = '*:*';
    }
    $this->query[] = $params['search'];

    $this->parameters[] = new Parameter\Query(implode(' AND ', $this->query));

    drupal_alter('culturefeed_search_query', $this->parameters, $this->query);

    $searchService = culturefeed_get_search_service();
    $this->result = $searchService->search($this->parameters);
    $facetingComponent->obtainResults($this->result, \CultureFeed_Cdb_Default::CDB_SCHEME_URL);

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
