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
   * Initializes the search with data from the URL query parameters.
   */
  public function initialize() {

    // Only initialize once.
    if (empty($this->facetComponent)) {
      $this->facetComponent = new Facet\FacetComponent();

      // Retrieve search parameters and add some defaults.
      $params = drupal_get_query_parameters();
      $params += array(
        'sort' => $this->getDefaultSort(),
        'search' => '',
        'facet' => array(),
      );

      $this->pageNumber = empty($params['page']) ? 1 : $params['page'] + 1;

      if (!empty($params['search'])) {
        // Remove / from the start and : from the end of keywords.
        $this->addQueryTerm(preg_replace("/\/\b|\b:/x", "", $params['search']));
      }

      $add_type_filter = TRUE;
      if (isset($params['facet']['type'])) {
        $active_types = $params['facet']['type'];
        unset($params['facet']['type']);
      }
      else {
        $active_types = variable_get('culturefeed_agenda_active_entity_types', array('event', 'production'));
        // If all active types are selected, don't add filter.
        if (count($active_types) == count(culturefeed_agenda_known_entity_types())) {
          $add_type_filter = FALSE;
        }
      }

      if ($add_type_filter) {
        array_walk($active_types, function(&$active_type) {
          $active_type = 'type:' . $active_type;
        });
        $this->parameters[] = new Parameter\FilterQuery(implode(' OR ', $active_types));
      }

      $this->addFacetFilters($params);

      $this->parameters[] = $this->facetComponent->facetField('category');
      $this->parameters[] = $this->facetComponent->facetField('datetype');
      $this->parameters[] = $this->facetComponent->facetField('city');
      $this->parameters[] = $this->facetComponent->facetField('location_category_facility_id');

      try {
        $this->execute($params);

        // Warm up cache.
        $this->warmupCache();
      }
      // Store the exception for later use. The loadPage will throw it.
      catch (Exception $e) {
        $this->exception = $e;
      }

    }
  }

  /**
   * {@inheritdoc}
   */
  protected function addFacetFilters($params) {
    parent::addFacetFilters($params);

    if (isset($params['organiser'])) {
      $this->parameters[] = new Parameter\Query('"' . $params['organiser'] . '"');
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

    $active_trail[] = array(
      'title' => t('Agenda'),
      'href' => 'agenda/search',
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

        $title = array();

        foreach ($facet['category_eventtype_id'] as $eventtype_id) {
          if (strpos($eventtype_id,'!') === 0) {
            $title[] = t('NOT ') . culturefeed_search_get_term_translation(substr($eventtype_id,1));
          }
          else {
            $title[] = culturefeed_search_get_term_translation($eventtype_id);
          }
        }

        $operator = drupal_strtoupper(variable_get('culturefeed_multiple_categories_operator','and'));

        $active_trail[] = array(
          'title' => implode(' ' . $operator . ' ', $title),
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
          'title' => culturefeed_search_get_term_translation(reset($query['facet']['category_theme_id'])),
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

    // Show entity type in breadcrumb for actors.
    if (isset($query['facet']['type'])) {

      if ($query['facet']['type'][0] == 'actor') {
        $facet['type'] = $query['facet']['type'];
        $active_trail[] = array(
          'title' => t('Places & organisers'),
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

    // Show actor type in breadcrumb.
    if (isset($query['facet']['category_actortype_id'])) {

      $facet['category_actortype_id'] = $query['facet']['category_actortype_id'];
      $active_trail[] = array(
        'title' => culturefeed_search_get_term_translation($query['facet']['category_actortype_id'][0]),
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

    // Show flandersregion in breadcrumb.
    if (isset($query['facet']['category_flandersregion_id'])) {

      $facet['category_flandersregion_id'] = $query['facet']['category_flandersregion_id'];

      $active_trail[] = array(
        'title' => culturefeed_search_get_term_translation($query['facet']['category_flandersregion_id'][0]),
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
      'title' => $this->getPageTitle(),
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
  protected function warmupCache() {

    $this->translateFacets();
    $this->prepareSlugs();

    // This part only needs to be done in case culturefeed_social is enabled.
    if (culturefeed_agenda_social_links_preprocessing_enabled() && culturefeed_is_culturefeed_user()) {
      culturefeed_social_warmup_activities_cache($this->result->getItems());
    }
  }

  /**
   * Warm up cache for facets to translate the items.
   */
  private function translateFacets() {
    $found_ids = array();
    $found_results = array();
    $translated_terms = array();
    $facets = $this->facetComponent->getFacets();
    foreach ($facets as $key => $facet) {
      // The key should start with 'category_' or 'location_'
      $start = substr($key, 0, 9);
      if (in_array($start, array('category_', 'location_'))) {
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
        // The key should start with 'category_' or 'location_'
        $start = substr($key, 0, 9);
        if (in_array($start, array('category_', 'location_'))) {
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
   * Prepare slugs for culturefeed_agenda_url_outbound_alter().
   */
  private function prepareSlugs() {
    $term_slugs = &drupal_static('culturefeed_search_term_slugs', array());
    $facets = $this->facetComponent->getFacets();
    $items = array();

    // At the moment we only need slugs for event type and themes.
    if (isset($facets['category_eventtype_id'])) {
      $items = $facets['category_eventtype_id']->getResult()->getItems();
    }
    if (isset($facets['category_theme_id'])) {
      $items = array_merge($items, $facets['category_theme_id']->getResult()->getItems());
    }

    // Search the slug for all facet items.
    if ($items) {

      $preferred_language = culturefeed_search_get_preferred_language();

      // Construct an array with tids to do the query.
      $tids = array();
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

    // Set prefix of the meta description based on entity type
    if (empty($query)) {
      $message = t("A summary of all events and productions");
    }
    elseif (!empty($query['facet']['type'][0])) {

      if ($query['facet']['type'][0] == 'actor') {
        $message = t("A summary of all actors");
      }
      elseif ($query['facet']['type'][0] == 'event') {
        $message = t("A summary of all events");
      }
      elseif ($query['facet']['type'][0] == 'production') {
        $message = t("A summary of all productions");
      }

    }
    elseif (!empty($query['facet']['category_actortype_id'][0])) {
      $message = t("A summary of all actors");
    }
    else {
      $message = t("A summary of all events and productions");
    }

    // Add additional facet information to the meta description
    // Only needed for indexable paths, see culturefeed_search_ui_set_noindex_metatag()
    if (!empty($query['voor-kinderen'])) {
      $message .= t(" for kids");
    }

    if (!empty($query['facet']['category_actortype_id'][0])) {
      $term = culturefeed_search_get_term_translation($query['facet']['category_actortype_id'][0]);
      $message .= t(" of the type @type", array('@type' => $term));
    }

    if (!empty($query['regId'])) {
      $term = culturefeed_search_get_term_translation($query['regId']);
      $message .= t(" in @region", array('@region' => $term));
    }
    elseif (!empty($query['location'])) {
      $message .= t(" in @region", array('@region' => $query['location']));
    }
    elseif (!empty($query['facet']['category_flandersregion_id'][0])) {
      $term = culturefeed_search_get_term_translation($query['facet']['category_flandersregion_id'][0]);
      $message .= t(" in @region", array('@region' => $term));
    }

    if (!empty($query['facet']['category_eventtype_id'][0])) {
      $term = culturefeed_search_get_term_translation($query['facet']['category_eventtype_id'][0]);
      $message .= t(" of the type @type", array('@type' => $term));
    }
    elseif (!empty($query['facet']['category_umv_id'][0])) {
      $term = culturefeed_search_get_term_translation($query['facet']['category_umv_id'][0]);
      $message .= t(" of the type @type", array('@type' => $term));
    }

    if (!empty($query['facet']['category_theme_id'][0])) {
      $term = culturefeed_search_get_term_translation($query['facet']['category_theme_id'][0]);
      $message .= t(" with theme @theme", array('@theme' => $term));
    }

    if (!empty($query['keyword'])) {
      $keyword = $query['keyword'];
      $message .= t(" with keyword @keyword", array('@keyword' => $keyword));
    }

    $message .= ". ";
    $message .= t("Discover what to do today, tomorrow, this weekend or later on.");

    return $message;
  }

}
