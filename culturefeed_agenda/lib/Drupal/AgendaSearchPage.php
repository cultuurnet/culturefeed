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
    global $culturefeedFacetingComponent;
    $culturefeedFacetingComponent = new Facet\FacetComponent();

    $params = drupal_get_query_parameters();

    $params += array(
      'sort' => $this->getDefaultSort(),
      'page' => 0,
      'search' => '',
      'facet' => array(),
    );

    $this->addFacetFilters($params);
    $this->addSort($params);

    $this->parameters[] = new Parameter\FilterQuery('type:event OR type:production');
    $this->parameters[] = $culturefeedFacetingComponent->facetField('category');
    $this->parameters[] = $culturefeedFacetingComponent->facetField('datetype');
    $this->parameters[] = $culturefeedFacetingComponent->facetField('city');

    $this->execute($params, $culturefeedFacetingComponent);

    if (module_exists('culturefeed_social')) {
      $this->warmupCache();
    }

    return $this->build();

  }

  /**
   * Get the title to show.
   */
  public function getDrupalTitle() {
    return format_plural($this->result->getTotalCount(), '@count activiteit gevonden', '@count activiteiten gevonden');
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

      case 'agefrom':
        $this->parameters[] = new Parameter\Sort('agefrom', Parameter\Sort::DIRECTION_ASC);
      break;

      default:

    }

  }

}