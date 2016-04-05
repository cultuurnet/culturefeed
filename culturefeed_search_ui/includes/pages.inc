<?php
/**
 * @file
 * Defines page callback to render the search results.
 */

use \Guzzle\Http\Exception;

/**
 * Page callback to search.
 *
 * @param String $name
 *   The name of the searchable type.
 * @param String $class_name
 *   The name of class to be used to load the correct search page.
 * @param Boolean $ajax
 *   The type of request.
 */
function culturefeed_search_ui_search_page($name, $class_name = NULL, $request_type = 'nojs') {

  // The first search page callback is a dummy callback to redirect to the
  // real search page.
  if ($name == "submitted") {
    return "";
  }
  $ajax = $request_type == 'ajax';
  $rss = $request_type == 'rss';

  try {

    $conf = culturefeed_get_searchable_type($name);
    $page = culturefeed_get_search_page($conf);

    if ($ajax || $rss) {
      $page->setFullPage(FALSE);
    }
    $page->setPagerType(variable_get('culturefeed_search_ui_pager_type', CULTUREFEED_SEARCH_UI_PAGER_NORMAL));
    $page->setDefaultTitle($conf['page_title']);
    $build = $page->loadPage();

    drupal_set_title($page->getPageTitle());

    $description = $page->getPageDescription();
    if (!empty($description)) {
      $element = array(
        '#tag' => 'meta',
        '#attributes' => array(
          'name' => 'description',
          'content' => $description,
        ),
      );
      drupal_add_html_head($element, 'meta_description');
    }

    $active_trail = $page->getActiveTrail();
    if ($active_trail) {
      menu_set_active_trail($active_trail);

      // Last item of the breadcrumb should not be clickable.
      $breadcrumb = drupal_get_breadcrumb();
      $last_item = array_pop($breadcrumb);
      $breadcrumb[] = strip_tags($last_item);
      drupal_set_breadcrumb($breadcrumb);

    }
    
    // Add rss to head if we're on a valid search path
    if (current_path() == 'agenda/search') {
      drupal_add_html_head_link(array(
        'rel' => 'alternate',
        'type' => 'application/rss+xml',
        'title' => end($active_trail)['title'],
        'href' => url(current_path() . '/rss', array('absolute' => TRUE, 'query' => drupal_get_query_parameters(NULL, array('q', 'page')))),
      ));
    }
  }
  catch (Exception\ClientErrorResponseException $e) {
    watchdog_exception('culturefeed_search_ui', $e);
    $build = t('There was an error while loading the search results.');
  }
  catch (Exception\ServerErrorResponseException $e) {
    watchdog_exception('culturefeed_search_ui', $e);
    $build = t('There was an error while loading the search results.');
  }
  catch (Exception\CurlException $e) {
    watchdog_exception('culturefeed_search_ui', $e);
    $build = t('There was an error while loading the search results.');
  }
  catch (InvalidSearchPageException $e) {
    $build = $e->getMessage();
  }
  catch (CultureFeed_Cdb_ParseException $e) {
    watchdog_exception('culturefeed_search_ui', $e);
    $build = t('There was an error while loading the search results.');
  }

  if ($ajax) {
    $commands = array(
      ajax_command_invoke('.more-link:first', 'remove'),
      ajax_command_append('#culturefeed-search-results-more-wrapper', render($build)),
    );

    ajax_deliver(array('#type' => 'ajax', '#commands' => $commands));
    exit;
  }
  elseif ($rss) {  
    // Add the RSS content-type header.
    drupal_add_http_header('Content-Type', 'application/rss+xml; charset=utf-8');
    $data = array();
    foreach ($page->getSearchResult()->getItems() as $item) {
      $object['cdbid'] = $item->getEntity()->getCdbId();
      $details = $item->getEntity()->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language());
      $object['title'] = $details->getTitle();
      $object['created'] = $item->getEntity()->getCreationDate();
      if (method_exists($item->getEntity(), 'getLocation')) {
        $object['city'] = $item->getEntity()->getLocation()->getAddress()->getPhysicalAddress()->getCity();
      }
      if (method_exists($item->getEntity(), 'getCalendar')) {
        $calendar = $item->getEntity()->getCalendar();
        $calendar_class = get_class($calendar);
        $txt_formatter = culturefeed_agenda_get_plain_text_calendar_formatter();
        $object['when_md_txt'] = '';
        if ($calendar_class != 'CultureFeed_Cdb_Data_Calendar_Permanent') {
          try {
            $object['when_md_txt'] = $txt_formatter->format($calendar, 'md');
          }
          catch (Exception $e) {
          }
        }
      }  
      if (method_exists($details, 'getCalendarSummary')) {
        $object['calendarsummary'] = $details->getCalendarSummary();
      }
      $object['shortdescription'] = $details->getShortDescription();
      $object['path'] = culturefeed_search_detail_url($item->getType(), $item->getId(), $details->getTitle());
      $data[] = $object;
    }
    // Return the themed feed as a render array.
    print theme('culturefeed_agenda_page_list_rss', array('objects' => $data, 'path' => end($active_trail)));
    exit;
  }
  else {
    return $build;
  }

}

/**
 * Page callback for the city suggestions autocomplete.
 *
 * (using jquery ui autocomplete format).
 */
function culturefeed_search_ui_city_suggestion_autocomplete_page($search_string, $include_regions = TRUE) {

  $matches = array();

  if ($search_string) {

    $parents = (isset($_GET['parents']) && !empty($_GET['parents'])) ? $_GET['parents'] : array();
    // Get the list of suggestions from service.
    $suggestions = culturefeed_search_get_city_suggestions($search_string, $include_regions, $parents);
    foreach ($suggestions as $key => $suggestion) {
      $matches[] = (object) array('value' => $key, 'label' => check_plain($suggestion));
    }

  }

  drupal_json_output($matches);

}

/**
 * Page callback for the city and actor suggestions autocomplete.
 *
 * (using jquery ui autocomplete format).
 */
function culturefeed_search_ui_city_actor_suggestion_autocomplete($search_string) {

  $matches = array();

  if ($search_string) {
    try {

      // Get the list of city suggestions from service.
      $city_suggestions = culturefeed_search_get_city_suggestions($search_string, TRUE);
      // We only need the 5 first suggestions.
      if (empty($city_suggestions)) {
        $matches[] = array(
          'label' => '',
          'category' => t('No postal or city found.'),
          'suggestion' => '',
          'type' => 'empty',
        );
      }
      // UIV-1352 We want all results instead of the first 5.
      //$city_suggestions = array_slice($city_suggestions, 0, 5);

      // Fix for UIV-1403 order by match of $search_string.
      // Change the structure of the $city_suggestions array.
      // Add stripos element to it.
      $old_city_suggestions = array();
      foreach ($city_suggestions as $key => $city_order_suggestion) {
        unset($city_suggestions[$key]);
        $old_city_suggestions[$key] = $city_order_suggestion;
        $city_suggestions[$key]['name'] = $city_order_suggestion;
        $city_suggestions[$key]['stripos'] = stripos($city_order_suggestion, $search_string);
      }

      // Order the $city_suggestions array by its element stripos.
      foreach ($city_suggestions as $key => $row) {
        $stripos[$key]  = $row['stripos'];
      }
      if (isset($stripos)) {
        array_multisort($stripos, SORT_ASC, $city_suggestions);
      }

      foreach ($city_suggestions as $city_suggestion) {
        $matches[] = array(
          'label' => str_replace("&#039;","'",check_plain($city_suggestion['name'])),
          'category' => '',
          'suggestion' => $city_suggestion['name'],
          'type' => 'city',
        );
      }

      // Get the allowed actor types.
      $parameters = array();
      $allowed_categories = variable_get('culturefeed_location_search_actortypes', NULL);
      if (!empty($allowed_categories)) {
        array_walk($allowed_categories, function(&$item) {
          $item = '"' . $item . '"';
        });
        $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery('category_actortype_id:(' . implode(' OR ', $allowed_categories) . ')');
      }

      // Get the list of actor suggestions from service.
      $actor_suggestions = culturefeed_get_search_service()->searchSuggestions($search_string, array('actor'), FALSE, $parameters);
      if ($actor_suggestions->hasSuggestions()) {
        foreach ($actor_suggestions as $actor_key => $actor_suggestion) {
          // We only need the 5 first suggestions.
          if  ($actor_key < 5) {
            $actor_path = culturefeed_search_detail_url('actor', $actor_suggestion->getCdbid(), $actor_suggestion->getTitle());
            $matches[] = array(
              'label' => $actor_suggestion->getTitle(),
              'category' => t('Directly to'),
              'suggestion' => $actor_path,
              'type' => 'actor',
            );
          }
        }
      }

    }
    catch (ClientErrorResponseException $e) {
      watchdog_exception('culturefeed_search_ui', $e);
    }

    // If only one city was found. Add a link to search on all actors in this city.
    if (count($city_suggestions) == 1) {

      $city = reset($old_city_suggestions);

      $query = array(
        'location' => $city,
        'facet' => array(
          'type' => array('actor'),
        ),
      );

      $matches[] = array(
        'label' => t('Show all locations in !city', array('!city' => $city)),
        'category' => t('Directly to'),
        'suggestion' => url('agenda/search', array('query' => $query)),
        'type' => 'actor',
      );
    }

  }

  drupal_json_output($matches);

}
