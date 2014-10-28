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
        'sort' => \CultuurNet\Search\ActivityStatsExtendedEntity::ACTIVITY_COUNT_PAGE_FOLLOW,
        'page' => 0,
        'search' => '',
        'facet' => array(),
      );

      $this->pageNumber = empty($params['page']) ? 1 : $params['page'] + 1;

      if (!empty($params['search'])) {
        $this->addQueryTerm($params['search']);
      }

      $this->addFacetFilters($params);

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

    $this->result = culturefeed_get_search_service()->searchPages($this->parameters);
    $this->facetComponent->obtainResults($this->result);

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

    $message = "";

    $query = drupal_get_query_parameters(NULL, array('q'));

    if (empty($query)) {
      $message = t("A summary of all pages on @site", array('@site' => variable_get('site_name', '')));
    }
    else {
      $message = t("A summary of all pages on @site", array('@site' => variable_get('site_name', '')));

      if (!empty($query['regId'])) {
        $term = culturefeed_search_get_term_translation($query['regId']);
        $message .= t(" in @region", array('@region' => $term));
      }
      elseif (!empty($query['location'])) {
        $message .= t(" in @region", array('@region' => $query['location']));
      }

      if (!empty($query['facet']['category_actortype_id'][0])) {
        $term = culturefeed_search_get_term_translation($query['facet']['category_actortype_id'][0]);
        $message .= t(" of the type @type", array('@type' => $term));
      }

    }

    return $message;
  }

}