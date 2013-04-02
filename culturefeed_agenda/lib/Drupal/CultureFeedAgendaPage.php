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
        // Ellen: tegelijkertijd boosten op recommend_count op bvb factor 500 en comment_count op pakweg 999
        // Erwin: met deze query zoek ik ALLE data (ik gebruik text:*) EN (waarvan de like_count 0 is OF
        // waarvan recomment_count groter dan 0 is, en deze laatste clausule geboost met 999)
        // http://searchv2.cultuurnet.lodgon.com/search-poc/rest/search?q=text:* AND (recommend_count:0 OR
        // recommend_count:[1 TO *]^999)&group=true&rows=10

        //$this->query[] = '(recommend_count:0 OR recommend_count:[1 TO *]^500)';
        //$this->query[] = '(comment_count:0 OR comment_count:[1 TO *]^999)';

    }

  }

}