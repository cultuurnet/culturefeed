<?php
/**
 * @file
 * Defines a Page callback for Pages search results.
 */

use \CultuurNet\Search\Parameter;
use \CultuurNet\Search\Component\Facet;

/**
 * Class CultureFeedPagesSearchPage
 */
class CultureFeedPagesSearchPage extends CultureFeedSearchPage
    implements CultureFeedSearchPageInterface {

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

      if (!empty($params['search'])) {
        $this->addQueryTerm($params['search']);
      }

      $this->addFacetFilters($params);
      $this->addSort($params);

      $this->parameters[] = $this->facetComponent->facetField('category');
      $this->parameters[] = $this->facetComponent->facetField('city');

      $this->execute($params);
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

    // Add grouping so returned data is not duplicate.
    $this->parameters[] = new Parameter\Group($this->group);

    // Always add spellcheck.
    $this->parameters[] = new Parameter\Parameter('spellcheck', 'true');
    if (isset($this->query[0])) {
      $this->parameters[] = new Parameter\Parameter('spellcheckQuery', $this->query[0]);
    }
    else {
      $this->parameters[] = new Parameter\Parameter('spellcheckQuery', '');
    }

    drupal_alter('culturefeed_search_page_query', $this);

    // Prepare the search query and add to the search parameters.
    $this->parameters[] = $this->prepareQuery();

    $this->result = culturefeed_get_search_service()->searchPages($this->parameters);
    $this->facetComponent->obtainResults($this->result);

  }

  /**
   * Add the sorting parameters for the agenda searches.
   */
  private function addSort($params) {

    switch ($params['sort']) {

      case 'title':
        $this->parameters[] = new Parameter\Sort('title_sort', Parameter\Sort::DIRECTION_ASC);
      break;

      case \CultuurNet\Search\ActivityStatsExtendedEntity::ACTIVITY_COUNT_PAGE_MEMBER:
        $this->parameters[] = new Parameter\Sort('pagemember_count', Parameter\Sort::DIRECTION_DESC);
      break;

      case \CultuurNet\Search\ActivityStatsExtendedEntity::ACTIVITY_COUNT_PAGE_FOLLOW:
        $this->parameters[] = new Parameter\Sort('pagefollow_count', Parameter\Sort::DIRECTION_DESC);
      break;

      case 'activity_count':
        $this->parameters[] = new Parameter\Sort('pagefollow_count', Parameter\Sort::DIRECTION_DESC);
      break;

      case 'agefrom':
        $this->parameters[] = new Parameter\Sort('agefrom', Parameter\Sort::DIRECTION_ASC);
      break;

      default:

    }

  }

}