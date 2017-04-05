<?php
/**
 * @file
 * Block callbacks for culturefeed agenda.
 */

use \CultuurNet\Search\Parameter;

/**
 * Block to show the related events from an item.
 */
function culturefeed_agenda_block_related_activities() {

  // Try to load the agenda detail.
  $item = culturefeed_agenda_get_active_object();
  if (!$item || arg(4) == 'edit' || arg(4) == 'tags') {
    return;
  }

  $config = array(
    'title' => t('Related activities'),
    'id' => 'related-activities',
    'path' => 'ajax/culturefeed/related-activities/' . $item->getId(),
  );

  return culturefeed_ui_block_ajaxload($config);

}

/**
 * Show a block with nearby activities for current user.
 */
function culturefeed_agenda_block_nearby_activities() {

  drupal_add_js(culturefeed_agenda_google_maps_js_api_url(), array('type' => 'external'));

  // If there is a location stored in cookie, add to the block title.
  $user_location = culturefeed_search_ui_get_user_location_cookie();

  // Add link to show all events for the location.
  $location_string = '';
  $url = '';
  $classes = 'all-activities-link';
  if (isset($user_location->postal) && isset($user_location->city)) {
    $location_string = check_plain($user_location->postal . ' ' . $user_location->city);
  }
  else {
    $classes .= ' hidden';
  }

  $all_activities_for_location_link = l(t('Everything for <span class="location-string">!location</span>', array('!location' => $location_string)), 'agenda/search',
    array(
      'query' => array('location' => $location_string),
      'html' => TRUE,
      'attributes' => array('id' => 'all-activities-link', 'class' => array($classes)))
  );

  $all_activities_for_location_raw_link = url('agenda/search',
    array(
      'query' => array('location' => $location_string),
    )
  );

  // Construct the toggle for the location form visbility.
  $change_location_link = l(t('Change location'), '',
    array(
      'attributes' => array('id' => 'change-location-link'),
    )
  );

  // Start with an empty list. On page load, the suggestions will be loaded through ajax.
  return array(
    'content' => array(
      '#theme' => 'culturefeed_agenda_block_nearby_activities',
      '#filter_form' => drupal_get_form('culturefeed_agenda_nearby_activities_filter_form'),
      '#change_location_link' => $change_location_link,
      '#all_activities_for_location_link' => $all_activities_for_location_link,
      '#all_activities_for_location_raw_link' => $all_activities_for_location_raw_link,
      '#location_string' => $location_string,
      '#attached' => array(
        'js' => array(
          drupal_get_path('module', 'culturefeed_agenda') . '/js/nearby_activities.js',
          drupal_get_path('module', 'culturefeed') . '/js/common.js',
        )
      ),
    ),
  );

}


/**
 * Form callback to show the agenda search block.
 */
function culturefeed_agenda_search_block_form($form, &$form_state) {

  $active_fields = variable_get('culturefeed_agenda_search_block_active_fields', array());

  $form = array();

  // Always submit to front, so that searches on a 404 would work.
  // $form['#action'] = url('<front>');

  // Category search.
  if (isset($active_fields['types'])) {

    $domain_id = variable_get('agenda_search_block_domain_id');
    if (!empty($domain_id)) {

      $options = array('' => t('All categories'));
      $categories = culturefeed_search_get_categories_by_domain($domain_id);

      if ($domain_id == 'eventtype') {
        $categories_remove = array(
          '0.100.0.0.0' => 'Kijken en luisteren',
          '0.100.1.0.0' => 'Doen',
          '0.100.2.0.0' => 'Bezoeken',
          '0.51.0.0.0' => 'Type onbepaald',
        );
        $categories = array_diff_key($categories, $categories_remove);
      }
      foreach ($categories as $id => $category) {
        $options[$id] = $category;
      }
      asort($options);

      $form['category'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => isset($_GET['facet']['category_' . $domain_id . '_id'][0]) ? $_GET['facet']['category_' . $domain_id . '_id'][0] : NULL,
        '#weight' => $active_fields['types'],
      );

      $form_state['#domain_id'] = $domain_id;

    }

    if (isset($_GET['facet']['category_' . $domain_id . '_id'][0])) {
      $default = $_GET['facet']['category_' . $domain_id . '_id'][0];
    }
    elseif (isset($_GET['facet']['category_id'][0])) {
      $default = $_GET['facet']['category_id'][0];
    }
    else {
      $default = NULL;
    }

    $form['category'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $default,
    );

  }

  // Free text search.
  if (isset($active_fields['what'])) {
    $form['search'] = array(
      '#type' => 'textfield',
      '#size' => 28,
      '#title' => t('What'),
      '#default_value' => isset($_GET['search']) ? $_GET['search'] : '',
      '#weight' => $active_fields['what'],
    );
  }

  // Datetype search.
  if (isset($active_fields['when'])) {
    $form['when'] = array(
      '#type' => 'select',
      '#title' => t('When'),
      '#options' => array('' => t('All dates')) + culturefeed_search_ui_get_date_facet_options(),
      '#default_value' => isset($_GET['facet']['datetype'][0]) ? $_GET['facet']['datetype'][0] : NULL,
      '#weight' => $active_fields['when'],
    );
  }

  // Location search.
  if (isset($active_fields['where'])) {

    $form['location'] = array(
      '#type' => 'placeholder',
      '#weight' => $active_fields['where'],
    );

    $default_where = isset($_GET['facet']['category_flandersregion_id'], $_GET['facet']['category_flandersregion_id'][0]) ? culturefeed_search_get_term_translation($_GET['facet']['category_flandersregion_id'][0]) : '';
    $form['location']['where'] = array(
      '#type' => 'textfield',
      '#size' => 28,
      '#title' => t('Where'),
      '#attributes' => array('class' => array('location-search', 'jquery-ui-autocomplete'), 'placeholder' => t('Zipcode or city')),
      '#default_value' => $default_where,
    );

    $form['location']['radius'] = array(
      '#type' => 'hidden',
      '#default_value' => isset($_GET['distance']) && !isset($_GET['facet']['category_flandersregion_id']) ? $_GET['distance'] : NULL,
    );

    $form['location']['nearby'] = array(
      '#markup' => '<a href="#" class="search-find-location"><span>' . check_plain(t('Use my current location')) . '</span></a>',
    );

    // Add js for geolocate.
    global $language;
    $form['#attached']['js'] = array(
      array('data' => culturefeed_agenda_google_maps_js_api_url(), 'type' => 'external'),
      drupal_get_path('module', 'culturefeed_agenda') . '/js/search_block.js',
      drupal_get_path('module', 'culturefeed') . '/js/common.js',
    );

    $form['#attached']['library'][] = array('system', 'jquery.cookie');
    $form['#attached']['library'][] = array('system', 'ui.autocomplete');
    $form['#attached']['js'][] = drupal_get_path('module', 'culturefeed_search_ui') . '/js/location_autocomplete.js';
    $form['#attached']['css'][] = drupal_get_path('module', 'culturefeed_search_ui') . '/css/location_search.css';

  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => variable_get('agenda_search_block_submit_value', t('Search')),
    '#weight' => 200,
  );

  global $user;
  if ($user->uid) {
    $form['#token'] = FALSE;
  }

  return $form;

}

/**
 * Submit the agenda search block form.
 */
function culturefeed_agenda_search_block_form_submit($form, &$form_state) {

  $query = drupal_get_query_parameters(NULL, array('q', 'page', 'search'));
  $destination = drupal_get_destination();

  // UIV-1406 reset $query if http headers return 404.
  $status = drupal_get_http_header();

  if (isset($status['status']) && $status['status'] == '404 Not Found') {
    $_GET['q'] = 'agenda/search';
    unset($_GET['destination']);
    $query = array();
  }


  if (!empty($form_state['values']['search'])) {
    $query['search'] = $form_state['values']['search'];
  }

  $active_filters = variable_get('culturefeed_agenda_search_block_active_fields', array());

  if (isset($active_filters['when'])) {
    unset($query['facet']['datetype']);
    if (!empty($form_state['values']['when'])) {
      $query['facet']['datetype'][0] = $form_state['values']['when'];
    }
  }

  if (isset($active_filters['where'])) {
    unset($query['where']);

    // Remove the region filter.
    if (isset($query['facet']['category_flandersregion_id'])) {
      unset($query['facet']['category_flandersregion_id']);
    }

    // Remove the distance filter.
    if (isset($query['distance'])) {
      unset($query['distance']);
    }

    if (!empty($form_state['values']['where'])) {
      $query['location'] = $form_state['values']['where'];
    }

    if (!empty($form_state['values']['radius'])) {
      $query['distance'] = $form_state['values']['radius'];
    }
  }

  if (!empty($form_state['#domain_id'])) {
    unset($query['facet']['category_' . $form_state['#domain_id'] . '_id']);
    // Activate the category facet.
    if (!empty($form_state['values']['category'])) {
      $query['facet']['category_' . $form_state['#domain_id'] . '_id'][0] = $form_state['values']['category'];
    }
  }

  if (!empty($form_state['values']['nearby']) && !empty($form_state['values']['coordinates'])) {
    $query['coordinates'] = $form_state['values']['coordinates'];
    $query['nearby'] = $form_state['values']['city'];
  }

  $form_state['redirect'] = array(
    'agenda/search',
    array('query' => $query),
  );

}

function culturefeed_agenda_block_organiser_events() {
	$item = culturefeed_agenda_get_active_object();
	if (!$item || $item->getType() == 'actor') {
		return;
	}

  if (is_null($item->getEntity()->getOrganiser())) {
    return;
  }
  else {
    $organiser_id = $item->getEntity()->getOrganiser()->getCdbid();
  }

	$amount = variable_get('agenda_organiser_event_amount', 10);

	$parameters = array();
	$parameters[] = new \CultuurNet\Search\Parameter\Query('organiser_cdbid:' . $organiser_id);
	$parameters[] = new \CultuurNet\Search\Parameter\FilterQuery('type:event OR type:production');
	$parameters[] = new \CultuurNet\Search\Parameter\Group();
	$parameters[] = new \CultuurNet\Search\Parameter\Rows($amount);

	try {

		$result = culturefeed_get_search_service()->search($parameters);

		if ($result->getTotalCount() == 0) {
			return;
		}

	}
	catch (Exception $e) {
		watchdog_exception('culturefeed_agenda', $e);
		return;
	}

	$content = array(
		'#theme' => 'culturefeed_agenda_organiser_events',
		'#organiser' => $item->getEntity()->getOrganiser(),
		'#items' => $result->getItems(),
	);

	$block = array(
		'subject' => '',
		'content' => $content,
	);

	return $block;
}

/**
 * Show the upcoming program of current actor.
 */
function culturefeed_agenda_block_actor_program() {

  $item = culturefeed_agenda_get_active_object();
  if (!$item || $item->getType() != 'actor') {
    return;
  }

  $parameters = array();
  $parameters[] = new \CultuurNet\Search\Parameter\Query('*:*');
  $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery('type:event OR type:production');
  $parameters[] = new \CultuurNet\Search\Parameter\Group();
  $parameters[] = new \CultuurNet\Search\Parameter\Rows(CULTUREFEED_AGENDA_ACTOR_PROGRAM_NUM_ITEMS);
  $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery('"' . $item->getId() . '"');
  $parameters[] = new \CultuurNet\Search\Parameter\Sort('startdate', 'asc');

  try {

    $result = culturefeed_get_search_service()->search($parameters);

    if ($result->getTotalCount() == 0) {
      return;
    }

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_agenda', $e);
    return;
  }

  $content = array(
    '#theme' => 'culturefeed_agenda_actor_program',
    '#actor' => $item,
    '#items' => $result->getItems(),
  );

  $block = array(
    'subject' => '',
    'content' => $content,
  );

  return $block;

}

/**
 * Show the program of current production.
 */
function culturefeed_agenda_block_production_program() {

  $cdb_item = culturefeed_agenda_get_active_object();
  if (!$cdb_item || $cdb_item->getType() != 'production') {
    return;
  }

  $region_ids = array();
  $items = culturefeed_agenda_load_events_of_production($cdb_item->getEntity());

  if (empty($items)) {
    return;
  }

  // Get all flanders regions, so we can query the p1's of them.
  foreach ($items as $event) {
    $regions = $event->getEntity()->getCategories()->getCategoriesByType(CultureFeed_Cdb_Data_Category::CATEGORY_TYPE_FLANDERS_REGION);
    foreach ($regions as $region) {
      $region_ids[] = $region->getId();
    }
  }

  $preferred_language = culturefeed_search_get_preferred_language();
  $query = db_select('culturefeed_search_terms', 't');
  $query->innerJoin('culturefeed_search_terms', 'province', 'province.tid = t.p1');
  if ($region_ids) {
    $query->condition('t.tid', $region_ids);
  }
  $query->condition('t.language', $preferred_language);
  $query->condition('province.language', $preferred_language);
  $query->addField('province', 'name', 'province_name');
  $query->addField('province', 'tid', 'province_id');
  $query->addField('t', 'tid', 'tid');

  $query_result = $query->execute();
  $provinces = array();
  foreach ($query_result as $row) {
    $provinces[$row->tid] = $row;
  }

  $block['subject'] = t('Program schedule');
  $block['content'] = array(
    '#theme' => 'culturefeed_agenda_production_program',
    '#items' => $items,
    '#provinces' => $provinces,
  );

  return $block;

}

/**
 * Show the actor search block.
 */
function culturefeed_agenda_block_actor_search() {

  // Only go if we're on an agenda search page.
  if (current_path() != 'agenda/search') {
    return;
  }

  // Only show this block if allowed in the block settings.
  if (isset($_GET['facet']['category_eventtype_id'][0])) {
    $current_category = $_GET['facet']['category_eventtype_id'][0];
  }
  elseif (isset($_GET['facet']['category_id'][0])) {
    $current_category = $_GET['facet']['category_id'][0];
  }
  else {
    $current_category = '';
  }

  $allowed_categories = variable_get('agenda_actor_search_visibility', NULL);
  if ($allowed_categories !== NULL) {
    if (!in_array($current_category, $allowed_categories)) {
      return;
    }
  }

  // Create build array with actor search form and nearby actors.
  $build = array();
  $build['form'] = drupal_get_form('culturefeed_agenda_block_actor_search_form');

  return array(
    'subject' => t('Actor'),
    'content' => $build,
  );
}

/**
 * Form callback to show the agenda actor search block.
 */
function culturefeed_agenda_block_actor_search_form($form, &$form_state) {
  $form = array();

  $form['#attached']['js'] = array(
    array(
      'data' => culturefeed_agenda_google_maps_js_api_url(),
      'type' => 'external'),
    drupal_get_path('module', 'culturefeed_agenda') . '/js/actor_autocomplete.js',
    drupal_get_path('module', 'culturefeed') . '/js/common.js',
    array(
      'data' => array(
        'culturefeed' => array(
          'actorsSuggestUrl' => url('ajax/culturefeed-agenda/actor-suggest'),
        ),
      ),
      'type' => 'setting',
    ),
  );
  $form['#attached']['library'] = array(
    array('system', 'jquery.cookie'),
  );

  $form['search'] = array(
    '#type' => 'textfield',
    '#size' => 28,
    '#title' => t('Search string'),
    '#title_display' => 'invisible',
    '#attributes' => array('class' => array('auto-submit-field'), 'placeholder' => t('Venue name or location')),
    '#autocomplete_path' => 'autocomplete/culturefeed/agenda/actor-search'
  );

  $form['#suffix'] = '<div id="actor-search-suggest"></div>';

  return $form;
}

/**
 * Show a block with event types.
 */
function culturefeed_agenda_block_event_types() {

  $eventtypes = variable_get('agenda_menu_event_types', array());
  if (empty($eventtypes)) {
    return;
  }

  $query = db_select('culturefeed_search_terms', 'cst');
  $query->fields('cst', array('tid', 'name', 'slug', 'p1'));
  $query->condition("cst.did", "eventtype");
  $query->condition("cst.show_term", 1);
  $query->condition("cst.language", culturefeed_search_get_preferred_language());
  $db_or = db_or();
  $db_or->condition('cst.tid', $eventtypes, 'IN');
  $db_or->condition('cst.p1', $eventtypes, 'IN');

  $query->condition($db_or);
  $result = $query->execute();

  $items = array();
  foreach ($result as $row) {
    if (empty($row->p1)) {
      $items[$row->tid]['item'] = $row;
    }
    else {
      $items[$row->p1]['children'][$row->tid]['item'] = $row;
    }
  }

  if (!empty($items)) {
    return array(
      'content' => array(
        '#theme' => 'culturefeed_agenda_event_types',
        '#items' => $items,
      ),
    );
  }

  return;

}

/**
 * Block to render GMAP for the Search results page
 * @return array|void
 */
function culturefeed_agenda_search_map() {
  global $language;
  $block = array();

  // Get search results
  $culturefeedSearchResult = culturefeed_get_search_result();
  if (!$culturefeedSearchResult instanceof \Cultuurnet\Search\SearchResult) {
    return;
  }

  if ($culturefeedSearchResult->getTotalCount() == 0) {
    return;
  }

  //if ($culturefeedSearchResult->getTotalCount() > variable_get('culturefeed_search_ui_pager_items_per_page', 10)) {
  if (variable_get('agenda_search_map_results_mode', 0) == 0) {
    $locations = culturefeed_agenda_get_locations_by_search_result();
  }
  else {
    $locations = culturefeed_agenda_get_locations_by_list_search();
  }

  $markers = array();
  foreach ($locations as $location) {
    $markers[] = array(
      'title' => $location['title'],
      'latitude' => $location['latitude'],
      'longitude' => $location['longitude'],
      'html' => implode('<br />', $location['events']),
    );
  }

  culturefeed_agenda_gmap_add_js();

  // Set JS setting
  drupal_add_js(
    array(
      'culturefeed_agenda_map' => array(
        'markers' => $markers,
      ),
    ),
    'setting'
  );

  // HTML for the map
  $block['content'] = array(
    '#theme' => 'culturefeed_agenda_search_map',
  );

  return $block;
}