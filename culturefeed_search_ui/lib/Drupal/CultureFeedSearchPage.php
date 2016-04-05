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
   * Current page number that is shown.
   */
  protected $pageNumber = 1;

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
  protected $defaultSortKey = \CultuurNet\Search\ActivityStatsExtendedEntity::ACTIVITY_COUNT_PAGE_FOLLOW;

  /**
   * List of input parameters to be given to the search.
   * @var \CultuurNet\Search\Parameter\AbstractParameter[]
   */
  protected $parameters = array();

  /**
   * The group value to send to the service.
   * @var string|boolean
   */
  protected $group = TRUE;

  /**
   * Query to search on.
   * @var array
   *   An array containing search strings. The respective values will be treated
   *   as required search terms, joined with "AND". If a single value contains
   *   multiple words separated by spaces, these will be treated as "AND".
   */
  protected $query = array();

  /**
   * Apache Solr local parameters.
   *
   * @see http://wiki.apache.org/solr/LocalParams
   *
   * @var array
   *   An array containing Apache Solr local parameters as key-value pairs.
   */
  protected $localParams = array();

  /**
   * Search result from current search.
   * @var \CultuurNet\Search\SearchResult
   */
  protected $result;

  /**
   * Default page title. This is a fallback in case no other title is provided.
   * @var string
   */
  protected $defaultTitle = '';

  /**
   * The page title to use. If empty this will be overridden by $defaultTitle.
   * @var string
   */
  protected $title = '';

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
   * Set the default title. Used as a fallback when there is no page title.
   * @param string $title
   */
  public function setDefaultTitle($title) {
    $this->defaultTitle = $title;
  }

  /**
   * Set the page title.
   *
   * @param string $title
   *   The text to set as page title. If this is not set the defaultTitle will
   *   be used instead.
   */
  public function setTitle($title) {
    $this->title = $title;
  }

  /**
   * Get the page title.
   *
   * @return string
   *   The currently set page title. If no page title has been set an empty
   *   string will be returned.
   */
  public function getTitle() {
    return $this->title;
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
   * Sets the search query.
   *
   * @param array $query
   *   The search query array.
   */
  public function setQuery(array $query) {
    $this->query = $query;
  }

  /**
   * Gets the search query.
   *
   * @return array
   *   The search query array.
   */
  public function getQuery() {
    return $this->query;
  }

  /**
   * Adds a search term to the search query.
   *
   * @param string $term
   *   The search term to add. This will be a required term.
   *
   * @return array
   *   The updated search query array.
   */
  public function addQueryTerm($term) {

    // Replace special characters with normal ones.
    $term = culturefeed_search_transliterate($term);

    // Replace AND to a space.
    $term = str_replace(' AND ', ' ', $term);

    $query_parts = explode(' OR ', $term);
    array_walk($query_parts, function(&$part) {

      // Strip of words between quotes. The spaces don't need to be replaced to AND for them.
      preg_match_all('/".*?"/', $part, $matches);
      foreach ($matches[0] as $match) {
        $part = str_replace($match, '', $part);
      }

      // Replace spaces between multiple search words by 'AND'.
      $part = str_replace(' ',' AND ', trim($part));

      // Add back the words between quotes.
      if (!empty($matches[0])) {
        if (empty($part)) {
          $part .= implode(' AND ', $matches[0]);
        }
        else {
          $part .= ' AND ' . implode(' AND ', $matches[0]);
        }
      }

    });

    $this->query[] = implode(' OR ', $query_parts);

    return $this->query;
  }

  /**
   * Gets the Apache Solr local parameters.
   *
   * @return array
   *   An array containing Apache Solr local parameters as key-value pairs.
   */
  public function getLocalParams() {
    return $this->localParams;
  }

  /**
   * Sets the Apache Solr local parameters.
   *
   * @param array $local_params
   *   An array containing Apache Solr local parameters as key-value pairs.
   */
  public function setLocalParams(array $localParams) {
    $this->localParams = $localParams;
  }

  /**
   * Sets an Apache Solr local parameter pair.
   *
   * @param string $key
   *   The local parameter key.
   * @param string $value
   *   The local parameter value.
   *
   * @return array
   *   The updated local parameters array.
   */
  public function setLocalParam($key, $value) {
    $this->localParams[$key] = $value;
    return $this->localParams;
  }

  /**
   * Unsets an Apache Solr local parameter pair.
   *
   * @param string $key
   *   The key of the local parameter to unset.
   *
   * @return array
   *   The updated local parameters array.
   */
  public function unsetLocalParam($key) {
    unset($this->localParams[$key]);
    return $this->localParams;
  }

  /**
   * Add an Apache Solr input parameter.
   *
   * @param \CultuurNet\Search\Parameter\AbstractParameter $parameter
   *   The parameter to add.
   *
   * @return array
   *   The updated parameters array.
   */
  public function addParameter(Parameter\AbstractParameter $parameter) {
    $this->parameters[] = $parameter;
    return $this->parameters;
  }

  /**
   * Unsets an Apache Solr input parameter.
   *
   * @param string $key
   *   The key of the parameter to unset.
   *
   * @return array
   *   The updated parameters array.
   */
  public function unsetParameter($key) {
    unset($this->parameters[$key]);
    return $this->parameters;
  }

  /**
   * Get all the parameters for current search.
   */
  public function getParameters() {
    return $this->parameters;
  }

  /**
   * Get the group value.
   * @return mixed boolean|string
   */
  public function getGroup() {
    return $this->group;
  }

  /**
   * Group value to set.
   * @param boolean|string $group
   */
  public function setGroup($group) {
    $this->group = $group;
  }

  /**
   * Initializes the search with data from the URL query parameters.
   */
  public function initialize() {

    // Only initialize once.
    if (empty($this->facetComponent)) {
      $this->facetComponent = new FacetComponent();

      // Retrieve search parameters and add some defaults.
      $params = drupal_get_query_parameters();
      $params += array(
        'sort' => 'relevancy',
        'search' => '',
        'facet' => array(),
      );

      $this->pageNumber = empty($params['page']) ? 1 : $params['page'] + 1;

      if (!empty($params['search'])) {
        $this->addQueryTerm($params['search']);
      }

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
        $startDate->setTime(0, 0, 0);

        // Set end date time to end of the day day, to it searches on full day.
        $endDate->setTime(23, 59, 59);

        $this->parameters[] = new Parameter\DateRangeFilterQuery('startdate', $startDate->getTimestamp(), $endDate->getTimestamp());
      }
    }

    // Add search on coordinates.
    if (isset($params['coordinates'])) {

      $coordinates = explode(',', $params['coordinates']);

      if (isset($params['distance'])) {
        $this->parameters[] = new Parameter\Spatial\Distance($params['distance']);
      }
      else {
        $this->parameters[] = new Parameter\Spatial\Distance(CULTUREFEED_SEARCH_DEFAULT_PROXIMITY_RANGE);
      }

      if (isset($params['sort']) && $params['sort'] == 'geodist') {
        $this->parameters[] = new Parameter\Sort('geodist()', 'asc');
        $this->parameters[] = new Parameter\FilterQuery('{!geofilt}');
      }

      $this->parameters[] = new Parameter\Spatial\Point($coordinates[0], $coordinates[1]);
      $this->parameters[] = new Parameter\Spatial\SpatialField('physical_gis');
    }

    if (empty($params['facet']['category_flandersregion_id'][0]) && !empty($params['location'])) {
      $flanders_region = culturefeed_search_get_category_by_slug($params['location'], 'flandersregion');
      if (!empty($flanders_region)) {
        $flanders_region = $flanders_region->tid;
      }
      // Backwards compatiblity for sites without clean urls: make sure location is mapped to flandersregion.
      if (empty($flanders_region)) {
        $flanders_region = key(culturefeed_search_get_categories_by_domain('flandersregion', array('name_like' => $params['location'])));
      }

      // Check if a valid flanders region was found. If not, show a message and force empty result.
      if (empty($flanders_region)) {
        drupal_set_message(t('You are searching on an invalid location.'), 'warning', FALSE);
        $params['facet']['category_flandersregion_id'][0] = $params['location'];
      }
      else {
        $params['facet']['category_flandersregion_id'][0] = $flanders_region;
      }
    }

    // Add the location facet. Only use the location if a distance is set.
    // all other cases will search for a category Id of the type flandersregion
    // or workingregion.
    if (!empty($params['facet']['category_flandersregion_id'][0])) {

      if (!isset($params['distance'])) {

        $regFilter = array();
        $params['regId'] = $params['facet']['category_flandersregion_id'][0];

        if (!empty($params['wregIds'])) {
          $regFilter[] = array_shift($params['wregIds']);

          $wregFilters = array();
          foreach ($params['wregIds'] as $wregId) {
            $wregFilters[] = $wregId;
          }
        }

        if (empty($_GET['only-wregs'])) {
          $regFilter[] = $params['regId'];
        }

        $regFilterQuery = '(';
        $regFilterQuery .= 'category_id:(' . implode(' OR ', $regFilter) .')';
        if (!empty($wregFilters)) {
          $regFilterQuery .= ' OR exact_category_id:(' . implode(' OR ', $wregFilters) . ')';
        }
        $regFilterQuery .= ')';
        $this->parameters[] = new Parameter\FilterQuery($regFilterQuery);

      }
      else {

        $location = culturefeed_search_get_term_by_id($params['facet']['category_flandersregion_id'][0]);

        // Check if postal was present.
        if ($location) {
          $city_parts = explode(' ', $location->name);
          if (is_numeric($city_parts[0]) && empty($params['wregIds'])) {
            $distance = isset($params['distance']) ? $params['distance'] : FALSE;

            // If category_actortype_id we assume that we search on pages (on day we have to fix)
            if (isset($params['facet']['category_actortype_id'])) {
              $this->parameters[] = new Parameter\FilterQuery('zipcode' . ':' . $city_parts[0]);
            }
            else {
              $this->parameters[] = new Parameter\Spatial\Zipcode($city_parts[0], $distance);
            }

          }
        }

      }

      unset($params['facet']['category_flandersregion_id']);

    }

    // Calculate actor if available.
    if (isset($params['actor']) && !empty($params['actor'])) {
      $this->parameters[] = new Parameter\FilterQuery('"' . $params['actor'] . '"');
    }

    foreach ($params['facet'] as $facetFieldName => $facetFilter) {
      // Filter out empty facets.
      $facetFilter = array_filter($facetFilter);

      if (!empty($facetFilter)) {
        // Datetype is not a real facet, but a search field.
        if ($facetFieldName == 'datetype') {
          $facetFilterQuery = new Parameter\DateTypeQuery(implode(' OR ', $facetFilter));
        }
        elseif ($facetFieldName == 'location_category_facility_id') {
          $facetFilterQuery = new Parameter\FilterQuery('location_category_facility_id:(' . implode(' OR ', $facetFilter) . ')');
        }
        else {

          // If wreg we need to add reg (same id's, example 3000 Leuven wreg.638 reg.638)
          foreach ($facetFilter as $facetFilterItem) {
            if (substr($facetFilterItem, 0, 4) == 'wreg') {
              $facetFilter[] = substr($facetFilterItem, 1);
            }
          }

          array_walk($facetFilter, function (&$item) {
            $item = '"' . str_replace('"', '\"', $item) . '"';
          });
          $facetFilterQuery = new Parameter\FilterQuery('category_id:(' . implode(' OR ', $facetFilter) . ')');
        }

        $this->parameters[] = $facetFilterQuery;
      }
    }

  }

  /**
   * Prepare search query for inclusion as a search parameter.
   *
   * @return \CultuurNet\Search\Parameter\Query
   *   The search parameter to use.
   */
  protected function prepareQuery() {
    $query = $this->query;

    // If no search terms have been given, match on everything.
    if (empty($query)) {
      $query[] = '*:*';
    }

    // String required search terms together with 'AND'.
    $keywords = implode(' AND ', $query);

    // Prepend local parameters to the query string if they were specified.
    if (!empty($this->localParams)) {
      $local_parameters = array();
      foreach ($this->localParams as $key => $value) {
        $local_parameters[] = "$key=$value";
      }
      $keywords = '{!' . implode(' ', $local_parameters) . '}' . $keywords;
    }

    return new Parameter\Query($keywords);
  }

  /**
   * Execute the search for current page.
   */
  protected function execute($params) {

    // Add start index (page number we want)
    $this->parameters[] = new Parameter\Start(($this->pageNumber - 1) * $this->resultsPerPage);

    // Add items / page.
    $this->parameters[] = new Parameter\Rows($this->resultsPerPage);

    // Add grouping so returned data is not duplicate.
    $this->parameters[] = new Parameter\Group($this->group);

    // Add spellcheck if needed
    if (!empty($this->query[0])) {
      $this->parameters[] = new Parameter\Parameter('spellcheck', 'true');
      $this->parameters[] = new Parameter\Parameter('spellcheckQuery', $this->query[0]);
    }
    else {
      $this->parameters[] = new Parameter\Parameter('spellcheck', 'false');
    }

    drupal_alter('culturefeed_search_page_query', $this);

    // Prepare the search query and add to the search parameters.
    $this->parameters[] = $this->prepareQuery();

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
          '#start' => ($this->pageNumber - 1) * $this->resultsPerPage,
        );

        $build['pager-container']['pager'] = array(
          '#theme' => 'pager',
          '#quantity' => 5
        );

      }
      elseif ($this->pagerType == self::PAGER_INFINITE_SCROLL) {

        $params = drupal_get_query_parameters();
        $rest = $this->result->getTotalCount() - (($this->pageNumber - 1) * $this->resultsPerPage);
        $params['page'] = $this->pageNumber;

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
            '#href' => strpos($_GET['q'], 'nojs') === FALSE ? $_GET['q'] . '/nojs' : $_GET['q'],
            '#options' => array('query' => $params),
            '#ajax' => array(),
            '#attributes' => array(
              'class' => array('btn btn-primary btn-block btn-large'),
              'rel' => array('nofollow')
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
   * Returns the title to show on the page.
   *
   * @see culturefeed_search_ui_search_page()
   *
   * @return string
   *   Returns the page title according to the following logic:
   *   1. If a title has been set with $this->setTitle(), this will be returned.
   *   2. If no title was set, and one or more search facets are active, a
   *      comma-separated list of active search facets are returned.
   *   3. If no title was set, and no search facets are active, the default
   *      title is returned. This is usually defined in the 'page_title' key in
   *      hook_culturefeed_search_page_info().
   *   If the query parameter 'page' is present and no title has been set with
   *   $this->setTitle(), the returned title will be appended with a comma
   *   and the page number with the page parameter increased by one,
   *   surrounded by parentheses.
   */
  public function getPageTitle($ignore_datetype = FALSE) {

    // Return the title that has been explicitly set with $this->setTitle().
    if (!empty($this->title)) {
      return $this->title;
    }

    // Otherwise the framework version will be used.
    $active_filters = module_invoke_all('culturefeed_search_ui_active_filters', $this->facetComponent);

    if ($ignore_datetype) {
      foreach($active_filters as $key => $active_filter) {
        if ($key == 'item_datetype') {
          unset ($active_filters[$key]);
        }
      }
    }

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

    if ($this->pageNumber > 1) {
      $labels[] = (t('page') . ' ' . $this->pageNumber);
    }

    if (!empty($labels)) {
      return implode(', ', $labels);
    }

  }

  /**
   * Gets a page description for all pages.
   *
   * Only type aanbod UiT domein, theme and location need to be prepared for search engines.
   *
   * @see culturefeed_search_ui_search_page()
   *
   * @return string
   *   Description for this type of page.
   */
  public function getPageDescription() {
    return "";
  }

  /**
   * Get the active trail to show.
   */
  public function getActiveTrail() {
    return;
  }

}
