<?php

use \CultuurNet\Search\Parameter;
use \CultuurNet\Search\Component\Facet\FacetComponent;

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
   * Default page title.
   * @var unknown
   */
  protected $defaultTitle = '';

  /**
   * Stores search facets with corresponding values for the active search.
   * @var \CultuurNet\Search\Component\Facet\FacetComponent
   */
  protected $facetComponent;

  /**
   * Gets the default sortkey.
   * @return String $sortKey
   */
  public function getDefaultSort() {
    return $this->defaultSortKey;
  }

  /**
   * Gets the search facets.
   */
  public function getFacetComponent() {
    return $this->facetComponent;
  }

  /**
   * Gets the search result.
   */
  public function getSearchResult() {
    return $this->result;
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
   * Set the default title.
   * @param string $title
   */
  public function setDefaultTitle($title) {
    $this->defaultTitle = $title;
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
   * Initializes the search with data from the URL query parameters.
   */
  public function initialize() {
    // Only initialize once.
    if (empty($this->facetComponent)) {
      $this->facetComponent = new Facet\FacetComponent();

      // Retrieve search parameters and add some defaults.
      $params = drupal_get_query_parameters();
      $params += array(
        'sort' => 'relevancy',
        'page' => 0,
        'search' => '',
        'facet' => array(),
      );

      $this->addFacetFilters($params);
      $this->addSort($params);

      $this->execute($params);
    }
  }

  /**
   * Loads a search page.
   */
  public function loadPage() {
    $this->initialize();
    return $this->build();
  }

  /**
   * Add the active facet filters to the parameters.
   */
  protected function addFacetFilters($params) {

    // Add the date range facet.
    if (isset($params['date_range'])) {

      $dates = explode('-', $params['date_range']);
      $startDate = DateTime::createFromFormat('d/m/Y', trim($dates[0]));
      if ($startDate) {
        $endDate = clone $startDate;
        if (isset($dates[1])) {
          $endDateTime = DateTime::createFromFormat('d/m/Y', trim($dates[1]));
          if ($endDateTime) {
            $endDate = $endDateTime;
          }
        }

        // Set start date time on beginning of the day.
        $startDate->setTime(0, 0, 1);

        // Set end date time to end of the day day, to it searches on full day.
        $endDate->setTime(23, 59, 59);

        $this->parameters[] = new Parameter\DateRangeFilterQuery('startdate', $startDate->getTimestamp(), $endDate->getTimestamp());
      }
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
    if (!empty($params['location'])) {

      // Check if postal was present.
      $city_parts = explode(' ', $params['location']);
      if (is_numeric($city_parts[0])) {
        $distance = isset($params['distance']) ? $params['distance'] : '0.00001';

        $this->parameters[] = new Parameter\Spatial\Zipcode($city_parts[0], $distance);
      }
      else {
        $location = '"' . str_replace('"', '\"', $params['location']) . '"';
        $this->parameters[] = new Parameter\FilterQuery('category_flandersregion_name' . ':' . $location);
      }

    }

    // Calculate actor if available.
    if (isset($params['actor']) && !empty($params['actor'])) {
      $id_search = array(
        'performer_cdbid' => $params['actor'],
        'location_cdbid' => $params['actor'],
        'organiser_cdbid' => $params['actor'],
      );
      $this->parameters[] = new Parameter\FilterQuery(implode(' OR ', $id_search));
    }

    foreach ($params['facet'] as $facetFieldName => $facetFilter) {
      // Filter out empty facets.
      $facetFilter = array_filter($facetFilter);

      if (!empty($facetFilter)) {
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

  }

  /**
   * Execute the search for current page.
   */
  protected function execute($params) {

    // Add start index (page number we want)
    $this->start = $params['page'] * $this->resultsPerPage;
    $this->parameters[] = new Parameter\Start($this->start);

    // Add items / page.
    $this->parameters[] = new Parameter\Rows($this->resultsPerPage);

    // Add grouping so returned events are not duplicate.
    $this->parameters[] = new Parameter\Group();

    // Always add spellcheck.
    $this->parameters[] = new Parameter\Parameter('spellcheck', 'true');

    if ('' == $params['search']) {
      $params['search'] = '*:*';
    }
    $this->query[] = $params['search'];

    $this->parameters[] = new Parameter\Query(implode(' AND ', $this->query));

    // Add in a boost for sort-type "relevancy".
    if ($params['sort'] == 'relevancy') {
      $this->query[0] = '{!boost%20b=sum(recommend_count,product(comment_count,10))}' . $this->query[0];
    }
    drupal_alter('culturefeed_search_query', $this->parameters, $this->query);

    $searchService = culturefeed_get_search_service();
    $this->result = $searchService->search($this->parameters);
    $this->facetComponent->obtainResults($this->result);

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
            'class' => array('more-link infinite-scroll')
          ),
        );

        if ($rest >= 0) {
          $build['pager-container']['pager'] = array(
            '#type' => 'link',
            '#title' => t('More results'),
            '#href' => $_GET['q'] . '/nojs',
            '#options' => array('query' => $params),
            '#ajax' => array(),
            '#attributes' => array(
              'class' => array('btn btn-primary btn-block btn-large')
          ),
          );
        }

        if ($rest <= $this->resultsPerPage) {
          $link = l(t('perform a new search.'), str_replace("/ajax", "", $_GET['q']));
          $build['pager-container']['pager'] = array(
            '#markup' => '<div class="alert"><p><strong>' . t('There are no more search results.') . '</strong></p>
          <p>' . t('Refine your search results or') . ' ' . $link . '</p></div>');
        }

        if ($this->fullPage) {
          $build['#prefix'] = '<div id="culturefeed-search-results-more-wrapper">';
          $build['#suffix'] = '</div>';
        }
      }
    }

    return $build;

  }

  /**
   * Get the title to show.
   */
  public function getDrupalTitle() {
    $active_filters = module_invoke_all('culturefeed_search_ui_active_filters', $this->facetComponent);
    if (!empty($active_filters)) {

      $labels = array();
      foreach ($active_filters as $active_filter) {

        if (!isset($active_filter['#label'])) {
          foreach ($active_filter as $subitem) {
            $labels[] = $subitem['#label'];
          }
        }
        else {
          $labels[] = $active_filter['#label'];
        }

      }

    }
    else {
      $labels[] = $this->defaultTitle;
    }

    if (isset($_GET['page']) && $_GET['page'] > 0) {
      $labels[] = ('pagina ' . ($_GET['page'] + 1));
    }

    if (!empty($labels)) {
      return implode(', ', $labels);
    }

  }

  /**
   * Get the active trail to show.
   */
  public function getActiveTrail() {
    return;
  }

}
