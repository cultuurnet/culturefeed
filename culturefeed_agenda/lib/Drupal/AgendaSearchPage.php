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

  /**
   * Warm up static caches that will be needed for this request.
   * We do this before rendering, because data for by example the recommend link would generate 20 requests.
   * This way we can lower this to only 1 request.
   */
  private function warmupCache() {

    $this->translateFacets();
    $this->prepareSlugs();

    // This part only needs to be done in case culturefeed_social is enabled.
    if (module_exists('culturefeed_social') && culturefeed_is_culturefeed_user()) {
      $this->prepareSocialStats();
    }
  }

  /**
   * Warm up cache for facets to translate the items.
   */
  private function translateFacets() {

    global $culturefeedFacetingComponent;

    $found_ids = array();
    $found_results = array();
    $translated_terms = array();
    $facets = $culturefeedFacetingComponent->getFacets();
    foreach ($facets as $key => $facet) {
      // The key should start with 'category_'
      if (substr($key, 0, 9) == 'category_') {
        $items = $facet->getResult()->getItems();
        foreach ($items as $item) {
          $found_ids[$item->getValue()] = $item->getValue();
        }
      }
    }

    // Translate the facets.
    if ($translations = culturefeed_search_term_translations($found_ids, TRUE)) {

      // Preferred language.
      $preferred_language = culturefeed_search_get_preferred_language();

      // Translate the facets labels.
      foreach ($facets as $key => $facet) {
        // The key should start with 'category_'
        if (substr($key, 0, 9) == 'category_') {
          $items = $facet->getResult()->getItems();
          foreach ($items as $item) {
            // Translate if found.
            if (!empty($translations[$item->getValue()][$preferred_language])) {
              $item->setLabel($translations[$item->getValue()][$preferred_language]);
            }
          }
        }
      }

    }

  }

  /**
   * Prepare all the social activity stats for this user.
   */
  private function prepareSocialStats() {

    global $culturefeedFacetingComponent;

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
   * Prepare slugs for culturefeed_agenda_url_outbound_alter().
   */
  private function prepareSlugs() {

    global $culturefeedFacetingComponent;
    $term_slugs = &drupal_static('culturefeed_search_term_slugs', array());
    $facets = $culturefeedFacetingComponent->getFacets();
    $items = array();

    // At the moment we only need slugs for event type and themes.
    if (isset($facets['category_eventtype_id'])) {
      $items = $facets['category_eventtype_id']->getResult()->getItems();
    }
    if (isset($facets['category_theme_id'])) {
      $items = array_merge($items, $facets['category_theme_id']->getResult()->getItems());
    }

    $preferred_language = culturefeed_search_get_preferred_language();
    $tids = array();
    $items = $facets['category_eventtype_id']->getResult()->getItems();
    $items += $facets['category_eventtype_id']->getResult()->getItems();
    foreach ($items as $item) {
      $subitems = $item->getSubItems();
      if ($subitems) {
        foreach ($subitems as $subitem) {
          $tids[] = $subitem->getValue();
        }
      }
      $tids[] = $item->getValue();
    }

    $result = db_query('SELECT tid, slug FROM {culturefeed_search_terms} WHERE tid IN(:tids) AND language = :language', array(':tids' => $tids, ':language' => $preferred_language));
    foreach ($result as $row) {
      $term_slugs[$row->tid] = $row->slug;
    }

  }


}