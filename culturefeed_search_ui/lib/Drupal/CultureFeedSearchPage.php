<?php

use CultuurNet\Search\Parameter\BooleanParameter;

use \CultuurNet\Search\Parameter;

/**
 * @file
 * Base class for search pages.
 */
class CultureFeedSearchPage {

  /**
   * Constants to define the available pager types.
   */
  const PAGER_NORMAL = 0;
  const PAGER_INFINITE_SCROLL = 1;

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
   * Indicator whether to render the full page or only the list items.
   * E.g. for ajax requests.
   * @var Boolean
   */
  protected $fullPage = TRUE;

  /**
   * Pager type to render with this page.
   * @var Int
   */
  protected $pagerType = self::PAGER_NORMAL;
  
  /**
   * Default sort key.
   * @var String
   */
  protected $defaultSortKey = 'relevancy';

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
   * Gets the default sortkey.
   * @return String $sortKey
   */
  public function getDefaultSort() {
    return $this->defaultSortKey;
  }

  /**
   * Sets the fullPage property.
   */
  public function setFullPage($fullPage) {
    $this->fullPage = $fullPage;
  }
  
  /**
   * Sets the default sortkey.
   * @param String $sortKey
   */
  public function setDefaultSort($sortKey) {
    $this->defaultSortKey = $sortKey;
  }

  /**
   * Sets the resultsPerPage property.
   */
  public function setResultsPerPage($resultsPerPage) {
    $this->resultsPerPage = $resultsPerPage;
  }

  /**
   * Sets the pagerType property.
   */
  public function setPagerType($pagerType) {
    $this->pagerType = $pagerType;
  }

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

      // Check if postal was present.
      $city_parts = explode(' ', $params['location']);
      if (is_numeric($city_parts[0])) {
        $distance = isset($params['distance']) ? $params['distance'] : '';
        $this->parameters[] = new Parameter\Spatial\Zipcode($city_parts[0], $distance);
      }
      else {
        $location = '"' . str_replace('"', '\"', $params['location']) . '"';
        $this->parameters[] = new Parameter\FilterQuery('category_flandersregion_name' . ':' . $location);
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
    $facetingComponent->obtainResults($this->result);

  }

  /**
   * Warm up static caches that will be needed for this request.
   * We do this before rendering, because data for by example the recommend link would generate 20 requests.
   * This way we can lower this to only 1 request.
   */
  protected function warmupCache() {

    // Do an activity search on all found nodeIds.
    $items = $this->result->getItems();
    $nodeIds = array();
    foreach ($items as $item) {
      $activity_content_type = culturefeed_get_content_type($item->getType());
      $nodeIds[] = culturefeed_social_get_activity_node_id($activity_content_type, $item);
    }

    $userDidActivity = &drupal_static('userDidActivity', array());

    // Get a list of all activities from this user, on the content to show.
    $userActivities = array();
    try {

      $userId = DrupalCultureFeed::getLoggedInUserId();

      $query = new CultureFeed_SearchActivitiesQuery();
      $query->nodeId = $nodeIds;
      $query->userId = $userId;
      $query->private = TRUE;

      $activities = DrupalCultureFeed::searchActivities($query);
      foreach ($activities->objects as $activity) {
        $userActivities[$activity->nodeId][$activity->contentType][] = $activity;
      }

    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_search_ui', $e);
    }

    // Fill up cache for following content types.
    $contentTypes = array(
      CultureFeed_Activity::CONTENT_TYPE_EVENT,
      CultureFeed_Activity::CONTENT_TYPE_PRODUCTION,
    );
    // Fill up the $userDidActivity variable. This is used in DrupalCulturefeed::userDidActivity().
    foreach ($nodeIds as $nodeId) {
      foreach ($contentTypes as $contentType) {
        // If user did this activitiy. Place it in the correct array.
        if (isset($userActivities[$nodeId][$contentType])) {
          $activities = new CultureFeed_ResultSet(count($userActivities[$nodeId][$contentType]), $userActivities[$nodeId][$contentType]);
        }
        // Otherwise create an empty result set.
        else {
          $activities = new CultureFeed_ResultSet(0, array());
        }
        $userDidActivity[$nodeId][$contentType][$userId] = $activities;
      }
    }

  }

  /**
   * Get the build from current search page.
   */
  protected function build() {

    pager_default_initialize($this->result->getTotalCount(), $this->resultsPerPage);

    $build = array();

    if ($this->fullPage) {
      $build['results'] = array(
        '#theme' => 'culturefeed_search_page',
        '#searchresult' => $this->result,
      );
    }
    else {
      $build['results'] = array(
        '#theme' => 'culturefeed_search_list',
        '#items' => $this->result,
        '#nowrapper' => TRUE,
      );
    }

    if ($this->result->getTotalCount() > 0) {

      $build['pager-container'] =  array(
        '#type' => 'container',
        '#attributes' => array(),
      );

      if ($this->pagerType == self::PAGER_NORMAL) {

        $build['pager-container']['pager_summary'] = array(
          '#theme' => 'culturefeed_search_pager_summary',
          '#result' => $this->result,
          '#start' => $this->start,
        );

        $build['pager-container']['pager'] = array(
          '#theme' => 'pager',
          '#quantity' => 5
        );

      }
      elseif ($this->pagerType == self::PAGER_INFINITE_SCROLL) {

        $params = drupal_get_query_parameters();
        $params += array(
          'page' => 0,
        );
        $rest = $this->result->getTotalCount() - ($params['page'] * $this->resultsPerPage);
        $params['page']++;

        $build['pager-container'] =  array(
          '#type' => 'container',
          '#attributes' => array(
            'class' => array('more-link')
          ),
        );

        if ($rest >= 0) {
          $build['pager-container']['pager'] = array(
            '#type' => 'link',
            '#title' => 'Meer resultaten',
            '#href' => $_GET['q'] . '/nojs',
            '#options' => array('query' => $params),
            '#ajax' => array(),
          );
        }

        if ($rest <= $this->resultsPerPage) {
          $link = l('nieuwe zoekopdracht', str_replace("/ajax", "", $_GET['q']));
          $build['pager-container']['pager'] = array(
            '#markup' => '<p><strong>Er zijn geen zoekresultaten meer.</strong></p>
          <p>Niet gevonden wat u zocht? Voer een ' . $link . ' uit.</p>');
        }

        if ($this->fullPage) {
          $build['#prefix'] = '<div id="culturefeed-search-results-more-wrapper">';
          $build['#suffix'] = '</div>';
        }
      }
    }

    drupal_set_title($this->getDrupalTitle());

    return $build;

  }

}
