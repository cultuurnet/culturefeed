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

    // Warm up cache.
    $this->warmupCache();

    return $this->build();

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

  /**
   * Get the active trail to show.
   */
  public function getActiveTrail() {

    $active_trail = array();

    $active_trail[] = array(
      'title' => t('Home'),
      'href' => '<front>',
      'link_path' => '',
      'localized_options' => array(),
      'type' => 0,
    );

    // Show event type and theme in breadcrumb.
    $query = drupal_get_query_parameters(NULL, array('page', 'q'));
    $facet = array();
    if (isset($query['facet']['category_eventtype_id']) || isset($query['facet']['category_theme_id'])) {

      if (isset($query['facet']['category_eventtype_id'])) {

        $facet['category_eventtype_id'] = $query['facet']['category_eventtype_id'];

        $active_trail[] = array(
          'title' => culturefeed_search_get_term_translation($query['facet']['category_eventtype_id'][0]),
          'href' => 'agenda/search',
          'link_path' => '',
          'localized_options' => array(
            'query' => array(
              'facet' => $facet,
            ),
          ),
          'type' => 0,
        );
      }

      if (isset($query['facet']['category_theme_id'])) {

        $facet['category_theme_id'] = $query['facet']['category_theme_id'];

        $active_trail[] = array(
          'title' => culturefeed_search_get_term_translation($query['facet']['category_theme_id'][0]),
          'href' => 'agenda/search',
          'link_path' => '',
          'localized_options' => array(
            'query' => array(
              'facet' => $facet,
            ),
          ),
          'type' => 0,
        );
      }

    }
    // If a filter was active, but none of the above 2. Show all activities.
    elseif (!empty($query)) {

      $active_trail[] = array(
        'title' => 'Alle activiteiten',
        'href' => 'agenda/search',
        'link_path' => '',
        'localized_options' => array(),
        'type' => 0,
      );

    }

    if (isset($query['location'])) {

      $active_trail[] = array(
        'title' => $query['location'],
        'href' => 'agenda/search',
        'link_path' => '',
        'localized_options' => array(
          'query' => array(
            'location' => $query['location'],
            'facet' => $facet,
          ),
        ),
        'type' => 0,
      );

    }

    $active_trail[] = array(
      'title' => $this->getDrupalTitle(),
      'href' => $_GET['q'],
      'link_path' => '',
      'localized_options' => array(),
      'type' => 0,
    );

    return $active_trail;

  }

}