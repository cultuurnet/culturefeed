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

  if ($item->getType() == 'actor') {
    culturefeed_agenda_set_actor_breadcrumb($item);
  }
  else {
    culturefeed_agenda_set_activity_breadcrumb($item);
  }

  return array(
    '#theme' => 'culturefeed_' . $item->getType(),
    '#item' => $item,
  );

}

/**
 * Page callback to do an autocomplete search on activity titles.
 */
function culturefeed_agenda_activity_suggestion_autocomplete_page($search_string, $past = FALSE) {

  $matches = array();

  if ($search_string) {

    try {

      // Get the list of suggestions from service.
      $suggestions = culturefeed_get_search_service()->searchSuggestions($search_string, array('event', 'production'), $past);
      if ($suggestions->hasSuggestions()) {
        foreach ($suggestions as $suggestion) {
          $matches[$suggestion->getTitle()] = check_plain($suggestion->getTitle());
        }
      }

    }
    catch (ClientErrorResponseException $e) {
      watchdog_exception('culturefeed_search', $e);
    }

  }

  drupal_json_output($matches);

  // Cache if possible per request.
  drupal_page_footer();
}

/**
 * Menu callback return 'de lijn' widget.
 *
 * @param \CultuurNet\Search\ActivityStatsExtendedEntity $item
 *   Item to show 'lijn' info for.
 */
function culturefeed_agenda_page_de_lijn_widget(\CultuurNet\Search\ActivityStatsExtendedEntity $item) {

  $element = array(
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'robots',
      'content' => 'noindex, follow',
    ),
  );
  drupal_add_html_head($element, 'nofollow');

  return array(
    '#theme' => 'culturefeed_agenda_de_lijn_widget',
    '#item' => $item,
    '#attached' => array(
      'css' => array(
        array('data' => 'http://pluggable.reisinfo.delijn.be/routeplannerPluggable/css/pluggableRouteplanner.css', 'type' => 'external'),
      ),
      'js' => array(
        array('data' => 'http://pluggable.reisinfo.delijn.be/routeplannerPluggable/js/calendar.js', 'type' => 'external'),
        array('data' => 'http://pluggable.reisinfo.delijn.be/routeplannerPluggable/dwr/interface/ReisAdvies.js', 'type' => 'external'),
        array('data' => 'http://pluggable.reisinfo.delijn.be/routeplannerPluggable/dwr/engine.js', 'type' => 'external'),
        array('data' => 'http://pluggable.reisinfo.delijn.be/routeplannerPluggable/js/dojo/dojo.xd.js', 'type' => 'external'),
        array('data' => 'http://pluggable.reisinfo.delijn.be/routeplannerPluggable/js/dojo/pluggableRP.xd.js', 'type' => 'external'),
      ),
    ),
  );
}

/**
 * Menu callback to show current activity on a map.
 * @param \CultuurNet\Search\ActivityStatsExtendedEntity $item
 *   Item to show the map for.
 */
function culturefeed_agenda_page_map(\CultuurNet\Search\ActivityStatsExtendedEntity $item) {

  $element = array(
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'robots',
      'content' => 'noindex, follow',
    ),
  );
  drupal_add_html_head($element, 'nofollow');

  return culturefeed_agenda_get_map_render_array($item);
}

/**
 * Ajax callback: load the related activities for an item.
 */
function culturefeed_agenda_page_ajax_related_activities($cdb_id) {

  $data = '';
  $item = culturefeed_search_item_load($cdb_id);
  if ($item) {

    $total_items = variable_get('agenda_related_activities_total_items', 3);
    $parameters = array();
    $parameters[] = new \CultuurNet\Search\Parameter\Query('*:*');
    // 1 extra then requested, because it's possible that current event is in the result.
    $parameters[] = new \CultuurNet\Search\Parameter\Rows($total_items + 1);
    $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery('type:event OR type:production');
    $parameters[] = new \CultuurNet\Search\Parameter\Group();

    // Add the context filter.
    $context = variable_get('agenda_related_activities_context');
    if ($context) {

      $categories = $item->getEntity()->getCategories()->getCategoriesByType($context);
      if ($categories) {
        $context_filter = array();
        foreach ($categories as $category) {
          $context_filter[] = $category->getId();
        }
        $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery('category_id' . ':(' . implode(' OR ', $context_filter) . ')');
      }
    }

    // Add the sort.
    if ($sort_field = variable_get('agenda_related_activities_sort', '')) {
      $parameters[] = new \CultuurNet\Search\Parameter\Sort($sort_field, variable_get('agenda_related_activities_sort_direction', \CultuurNet\Search\Parameter\Sort::DIRECTION_ASC));
    }

    // Add extra query.
    if ($filter_query = variable_get('agenda_related_activities_extra_query', '')) {
      $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery($filter_query);
    }

    // Add spatial search.
    if ($item->getType() == 'event' && variable_get('agenda_related_activities_range', 0)) {

      $address = $item->getEntity()->getLocation()->getAddress();
      if ($address && $address->getPhysicalAddress()) {
        $coordinates = $address->getPhysicalAddress()->getGeoInformation();
        if ($coordinates) {
          $distance = new \CultuurNet\Search\Parameter\Spatial\Distance(variable_get('agenda_related_activities_range', 0));
          $point = new \CultuurNet\Search\Parameter\Spatial\Point($coordinates->getYCoordinate(), $coordinates->getXCoordinate());
          $field = new \CultuurNet\Search\Parameter\Spatial\SpatialField('physical_gis');
          $parameters[] = new \CultuurNet\Search\Parameter\Spatial\GeoFilterQuery($point, $distance, $field);
        }
      }
    }

    // Execute search.
    try {

      $result = culturefeed_get_search_service()->search($parameters);
      if ($result->getTotalCount() >= 0) {

        $search_results = $result->getItems();

        if(module_exists('culturefeed_social')) {
          culturefeed_social_warmup_activities_cache($search_results);
        }

        // Render the results.
        $items = array();
        $count = 0;
        foreach ($search_results as $search_result) {

          // Don't add current activity to the list.
          if ($search_result->getEntity()->getCdbId() == $item->getEntity()->getCdbid()) {
            continue;
          }

          // If current activity was not in results. Make sure we show only the request amount.
          if ($count < $total_items) {
            $items[] = $search_result;
          }

          $count++;

          $output = array(
            '#theme' => 'culturefeed_agenda_related_activities',
            '#results' => $items,
          );

          $data = drupal_render($output);
        }
      }
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_agenda', $e);
    }

  }

  $commands = array();
  $commands[] = ajax_command_html('#related-activities', $data);

  ajax_deliver(array('#type' => 'ajax', '#commands' => $commands));

}

/**
 * Ajax callback: load the nearby activities.
 */
function culturefeed_agenda_page_ajax_nearby_activities($lat, $lon) {
  // Compose the search query.
  $parameters[] = new \CultuurNet\Search\Parameter\Query('*:*');
  $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery('type:event OR type:production');
  $parameters[] = new \CultuurNet\Search\Parameter\Group();

  // Get location form cookie and add parameters to search query.
  if (isset($_COOKIE['Drupal_visitor_uitid_userLocation'])) {
    $user_location = json_decode($_COOKIE['Drupal_visitor_uitid_userLocation']);
    $distance = new \CultuurNet\Search\Parameter\Spatial\Distance(variable_get('agenda_nearby_activities_distance', '100'));
    $point = new \CultuurNet\Search\Parameter\Spatial\Point($user_location->latitude, $user_location->longitude);
    $field = new \CultuurNet\Search\Parameter\Spatial\SpatialField('physical_gis');
    $parameters[] = new \CultuurNet\Search\Parameter\Spatial\GeoFilterQuery($point, $distance, $field);
  }

  // Add other configurable parameters.
  $amount = variable_get('agenda_nearby_activities_amount', 8);
  $parameters[] = new \CultuurNet\Search\Parameter\Rows($amount);
  if ($extra_query = variable_get('agenda_nearby_activities_extra_query', '')) {
    $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery($extra_query);
  }
  $sort = variable_get('agenda_nearby_activities_sort', 'permanent asc, startdateday asc,weight desc');
  $parameters[] = new \CultuurNet\Search\Parameter\Sort($sort_field, $sort);

  // Execute search.
  try {
    $result = culturefeed_get_search_service()->search($parameters);
    if ($result->getTotalCount() >= 0) {
      $da = '';
      $items = $result->getItems();
      foreach($items as $item) {
        $da .= $item->getId() . '<br />';
      }
      /* Render the results.
      $output = array(
        '#theme' => 'culturefeed_agenda__activities',
        '#results' => $items,
      );*/

      $data = '<div>'. $da . '</div>';
//      $data = drupal_render($output);

    }
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_agenda', $e);
  }

  $data .= '<div><a href="/culturefeed/agenda/set-user-location/nojs">Set you location</a></div>';

  $commands = array();
  $commands[] = ajax_command_html('#nearby-activities', $data);

  ajax_deliver(array('#type' => 'ajax', '#commands' => $commands));

}

/**
 * Autocomplete callback for actor search.
 */
function culturefeed_agenda_actor_search_autocomplete($search) {

  $matches = array();

  if ($search) {

    try {

      $parameters = array();
      $allowed_categories = variable_get('agenda_actor_autocomplete_categories', NULL);
      if (!empty($allowed_categories)) {
        array_walk($allowed_categories, function(&$item) {
          $item = '"' . $item . '"';
        });
        $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery('category_actortype_id:(' . implode(' OR ', $allowed_categories) . ')');
      }

      // Get the list of suggestions from service.
      $suggestions = culturefeed_get_search_service()->searchSuggestions($search, array('actor'), TRUE, $parameters);
      if ($suggestions->hasSuggestions()) {
        foreach ($suggestions as $suggestion) {
          $path = culturefeed_search_detail_url('actor', $suggestion->getCdbid(), $suggestion->getTitle());
          $matches[$path] = check_plain($suggestion->getTitle());
        }
      }

    }
    catch (ClientErrorResponseException $e) {
      watchdog_exception('culturefeed_agenda', $e);
    }

  }
  drupal_json_output($matches);
}

/**
 * Search for nearby actors.
 */
function culturefeed_agenda_page_ajax_actor_suggest() {

  $parameters = array();

  if (isset($_COOKIE['Drupal_visitor_uitid_userLocation'])) {
    $coordinates = json_decode($_COOKIE['Drupal_visitor_uitid.userLocation']);
    $point = new \CultuurNet\Search\Parameter\Spatial\Point($coordinates->latitude, $coordinates->longitude);
    $distance = new \CultuurNet\Search\Parameter\Spatial\Distance(CULTUREFEED_SEARCH_DEFAULT_PROXIMITY_RANGE);
    $field = new \CultuurNet\Search\Parameter\Spatial\SpatialField('physical_gis');
    $parameters[] = new \CultuurNet\Search\Parameter\Spatial\GeoFilterQuery($point, $distance, $field);
  }

  $parameters[] = new \CultuurNet\Search\Parameter\Rows(5);
  $parameters[] = new \CultuurNet\Search\Parameter\Query('*:*');

  $allowed_categories = variable_get('agenda_actor_autocomplete_categories', NULL);
  if (!empty($allowed_categories)) {
    array_walk($allowed_categories, function(&$item) {
      $item = '"' . $item . '"';
    });
    $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery('category_actortype_id:(' . implode(' OR ', $allowed_categories) . ')');
  }

  $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery('type:actor');

  $output = new stdClass();
  $output->success = TRUE;
  $output->data = '';

  try {

    $result = culturefeed_get_search_service()->search($parameters);

    if ($result->getTotalCount() > 0) {
      $items = $result->getItems();
      $output->data = theme('culturefeed_agenda_actors_nearby', array('items' => $items));
    }

  }
  catch (Exception $e) {
    $output->success = FALSE;
    watchdog_exception('culturefeed_agenda', $e);
  }

  drupal_json_output($output);
  exit();

}

/**
 * Page callback to set the user's location in the cookie.
 */
function culturefeed_agenda_page_set_user_location($request_type = 'ajax') {
  $params['title'] = t('Set your location');
//  $params['use_ajax'] = $request_type == 'ajax';

  $form = drupal_get_form('culturefeed_agenda_set_user_location_form', $params);

  return $form;

  if ($request_type == 'ajax') {
    $commands = array();
    $commands[] = culturefeed_ajax_command_modal('#set-user-location-form', drupal_render($form));

    print ajax_render($commands);
    exit;
  }
  else {
    return $form;
  }
}


/**
 * Form set the user's location in a cookie.
 */
function culturefeed_agenda_set_user_location_form($form, &$form_state, $params) {

  drupal_set_title($params['title']);

  $form['errors'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="set-user-location-ajax"></div>'
  );

  $form['info'] = array(
    '#type' => 'markup',
    '#markup' => '<label>' . t('Set your location?') . '</label>',
  );

  $form['where'] = array(
    '#type' => 'textfield',
    '#title' => t('Your Location'),
    '#default_value' => isset($_GET['location']) ? $_GET['location'] : NULL,
    '#autocomplete_path' => 'autocomplete/culturefeed/city-suggestion',
    '#attributes' => array('placeholder' => t('Zipcode or city')),
  );
  drupal_add_js(drupal_get_path('module', 'culturefeed_agenda') .'/js/my_location.js');


  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save location'),
    '#name' => 'delete',
    '#submit' => array('culturefeed_agenda_set_user_location_form_submit'),
  );
/*
  if ($params['use_ajax']) {
    $form['actions']['submit']['#ajax'] = array(
      'callback' => 'culturefeed_agenda_set_user_location_form_ajax',
      'wrapper' => 'set-user-location-form',
    );
  }
*/
  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t("Cancel"),
    '#name' => 'cancel',
    '#submit' => array('culturefeed_agenda_set_user_location_form_submit'),
  );

  $form_state['params'] = $params;

  return $form;

}

/**
 * Ajax callback for the user's location form.
 */
function culturefeed_agenda_set_user_location_form_ajax($form, $form_state) {

  $url = '/agenda/search';

  $commands = array();
  if (empty($form_state['params']['validation_error'])) {
    $commands[] = culturefeed_ajax_command_goto($url);
  }
  else {
    $commands[] = ajax_command_html('#set-user-location-form', drupal_render($form));
    $commands[] = ajax_command_html('#set-user-location-ajax', theme('status_messages'));
  }

  return array('#type' => 'ajax', '#commands' => $commands);

}

/**
 * Submit handler for deleting calendar events.
 */
function culturefeed_agenda_set_user_location_form_submit($form, $form_state) {

  if ($where = $form_state['values']['where']) {
    // Split postal and city.
    $where = trim($form_state['values']['where']);
    $postal_length = 4;
    $postal = substr($where, 0, $postal_length);
    $city = substr($where, $postal_length);
    // Store in cookie.
    $values = array(
      'uitid.userLocation' => json_encode(
        array(
          'latitude' => '',
          'longitude' => '',
          'city' => $city,
          'postal' => $postal,
        )
      ),
    );
    user_cookie_save($values);
  }

drupal_goto('/agenda/search');

  if ($form_state['triggering_element']['#name'] == 'cancel') {
    drupal_set_message(t('You clicked cancel.'));
    drupal_goto('/agenda/search');
    return;
  }

  drupal_set_message(t('You clicked OK.'));
  if (!$form_state['params']['use_ajax']) {
    drupal_goto('/agenda/search');
  }

}

/**
 * Validate the location.
 */
function culturefeed_agenda_set_user_location_form_validate(&$form, &$form_state) {

  $form_state['params']['validation_error'] = FALSE;

  if (!isset($form_state['values']['where'])) {
    form_set_error('where', t('Please search your location'));
    $form_state['params']['validation_error'] = TRUE;
  }

}
