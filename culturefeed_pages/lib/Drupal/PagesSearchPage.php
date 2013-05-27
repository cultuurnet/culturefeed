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
   * Loads a search page.
   */
  public function loadPage() {

    // store faceting component in global, for use in blocks
    global $facetingComponent;
    $facetingComponent = new Facet\FacetComponent();

    $args = $_GET;
    $params = drupal_get_query_parameters();

    $params += array(
      'sort' => 'relevancy',
      'page' => 0,
      'search' => '',
      'facet' => array(),
    );

    $this->addFacetFilters($params);
    $this->addSort($params);

    $this->parameters[] = $facetingComponent->facetField('category');
    $this->parameters[] = $facetingComponent->facetField('city');

    $this->execute($params, $facetingComponent);

    return $this->build();

  }

  /**
   * Get the title to show.
   */
  public function getDrupalTitle() {
    return format_plural($this->result->getTotalCount(), '@count pagina gevonden', "@count pagina's gevonden");
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
    $this->result = $searchService->searchPages($this->parameters);
    $facetingComponent->obtainResults($this->result, '');

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

      default:

    }

  }

}