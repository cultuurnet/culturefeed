<?php
/**
 * @file
 * Defines a Page callback for Agenda search results.
 */

use \CultuurNet\Search\Parameter;
use \CultuurNet\Search\Component\Facet;

/**
 * Class CultureFeedAgendaPage
 */
class CultureFeedAgendaPage extends CultureFeedSearchPage
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

    $this->parameters[] = new Parameter\FilterQuery('type:event OR type:production');
    $this->parameters[] = $facetingComponent->facetField('category');
    $this->parameters[] = $facetingComponent->facetField('datetype');

    $this->execute($params, $facetingComponent);

    return $this->build();

  }

  /**
   * Get the title to show.
   */
  public function getDrupalTitle() {
    return $this->result->getTotalCount() . ' activiteiten gevonden';
  }

  /**
   * Add the sorting parameters for the agenda searches.
   */
  private function addSort($params) {

    switch ($params['sort']) {

      case 'date':
        $this->parameters[] = new Parameter\Sort('startdate', Parameter\Sort::DIRECTION_ASC);
      break;

      case 'recommends':
        $this->parameters[] = new Parameter\Sort('recommend_count', Parameter\Sort::DIRECTION_DESC);
      break;

      case 'comments':
        $this->parameters[] = new Parameter\Sort('comment_count', Parameter\Sort::DIRECTION_DESC);
      break;

      default:

    }

  }

}