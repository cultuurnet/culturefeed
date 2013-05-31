<?php
/**
 * @file
 * Defines page callbacks which access the Search api.
 */

/**
 * Page callback to show the detail from an agenda item.
 * @param \CultuurNet\Search\ActivityStatsExtendedEntity $item
 *   Extended entity to view.
 */
function culturefeed_agenda_detail_page(\CultuurNet\Search\ActivityStatsExtendedEntity $item) {

  // Hardcoded breadcrumb requested. Use active trail, so breadcrumb can be altered.
  $active_trail = array();

  $active_trail[] = array(
    'title' => t('Home'),
    'href' => '<front>',
    'link_path' => '',
    'localized_options' => array(),
    'type' => 0,
  );

  $active_trail[] = array(
    'title' => 'Activiteiten',
    'href' => 'agenda/search',
    'link_path' => '',
    'localized_options' => array(),
    'type' => 0,
  );

  // First theme.
  $themes = $item->getEntity()->getCategories()->getCategoriesByType(CultureFeed_Cdb_Data_Category::CATEGORY_TYPE_THEME);
  if ($themes) {
    $active_trail[] = array(
      'title' => $themes[0]->getName(),
      'href' => 'agenda/search',
      'link_path' => '',
      'localized_options' => array(
        'query' => array(
          'facet' => array('category_theme_name' => array($themes[0]->getName())),
        ),
      ),
      'type' => 0,
    );

  }

  global $language;
  $active_trail[] = array(
    'title' => $item->getTitle($language->language),
    'href' => $_GET['q'],
    'link_path' => '',
    'localized_options' => array(),
    'type' => 0,
  );

  menu_set_active_trail($active_trail);

  return array(
    '#theme' => 'culturefeed_' . $item->getType(),
    '#item' => $item,
  );

}

/**
 * Page callback to do an autocomplete search on activity titles.
 */
function culturefeed_agenda_activity_suggestion_autocomplete_page($search_string) {

  $searchService = culturefeed_get_search_service();
  $matches = array();

  if ($search_string) {

    try {

      // Get the list of suggestions from service.
      $result = $searchService->searchSuggestions($search_string, array('event', 'production'));
      if ($result->hasSuggestions()) {
        $suggestions = $result->getSuggestions();
        foreach ($suggestions as $suggestion) {
          $matches[$suggestion] = check_plain($suggestion);
        }
      }

    }
    catch (ClientErrorResponseException $e) {
      watchdog_exception('culturefeed_search', $e);
    }

  }

  drupal_json_output($matches);

}
