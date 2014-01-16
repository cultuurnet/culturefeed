<?php
/**
 * @file
 * Block callbacks for culturefeed agenda.
 */

/**
 * Block to show the related events from an item.
 */
function culturefeed_agenda_block_related_activities() {

  // Try to load the agenda detail.
  $item = culturefeed_agenda_get_active_object();
  if (!$item) {
    return;
  }

  $total_items = variable_get('agenda_related_activities_total_items', 5);
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
        $point = new \CultuurNet\Search\Parameter\Spatial\Point($coordinates->getXCoordinate(), $coordinates->getYCoordinate());
        $field = new \CultuurNet\Search\Parameter\Spatial\SpatialField('physical_gis');
        $parameters[] = new \CultuurNet\Search\Parameter\Spatial\GeoFilterQuery($point, $distance, $field);
      }
    }

  }

  // Execute search.
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

  global $language;
  $search_results = $result->getItems();
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
      $items[] = theme('culturefeed_' . $search_result->getType() . '_short_summary', array('item' => $search_result));
    }

    $count++;

  }

  $block = array();
  $block['subject'] = t('Related activities');
  $block['content'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
    '#class' => 'related-activities'
  );

  return $block;

}

/**
 * Form callback to show the agenda search block.
 */
function culturefeed_agenda_search_block_form($form, &$form_state) {

  $form = array();

  // Always submit to front, so that searches on a 404 would work.
  // $form['#action'] = url('<front>');

  $domain_id = variable_get('agenda_search_block_domain_id');
  if (!empty($domain_id)) {

    $options = array('' => t('All categories'));
    $categories = culturefeed_search_get_categories_by_domain($domain_id);
    foreach ($categories as $id => $category) {
      $options[$id] = $category;
    }

    $form['category'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => isset($_GET['facet']['category_' . $domain_id . '_id'][0]) ? $_GET['facet']['category_' . $domain_id . '_id'][0] : NULL,
    );

    $form_state['#domain_id'] = $domain_id;

  }

  $form['search'] = array(
    '#type' => 'textfield',
    '#title' => t('Search string'),
    '#default_value' => isset($_GET['search']) ? $_GET['search'] : '',
    '#autocomplete_path' => 'autocomplete/culturefeed/suggestions',
  );

  $location = isset($_GET['nearby']) ? $_GET['nearby'] : '';
  $form['nearby'] = array(
    '#type' => 'checkbox',
    '#title' => t('Activities near my current location:') . ' <span id="current-location">' . check_plain($location) . '</span>',
    '#default_value' => !empty($location),
  );

  $form['coordinates'] = array(
    '#type' => 'hidden',
    '#default_value' => isset($_GET['coordinates']) ? $_GET['coordinates'] : '',
    '#attributes' => array('id' => 'geolocate-coordinates'),
  );

  $form['city'] = array(
    '#type' => 'hidden',
    '#default_value' => $location,
    '#attributes' => array('id' => 'geolocate-city'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  global $language;
  $query = array(
    'sensor' => 'true',
    'v' => '3.5',
    'region' => 'BE',
    'hl' => $language->language,
  );
  $form['#attached']['js'] = array(
    array('data' => CULTUREFEED_GOOGLE_MAPS_JS_API, 'type' => 'external'),
    drupal_get_path('module', 'culturefeed_agenda') . '/js/search_block.js',
    drupal_get_path('module', 'culturefeed') . '/js/common.js',
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
  if (!empty($form_state['values']['search'])) {
    $query['search'] = $form_state['values']['search'];
  }

  if (!empty($form_state['#domain_id'])) {
    unset($query['facet']['category_' . $form_state['#domain_id'] . '_name']);
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

  // Add search on the different agenda connections for the actor.
  $id_search = array(
    'performer_cdbid' => $item->getId(),
    'location_cdbid' => $item->getId(),
    'organiser_cdbid' => $item->getId(),
  );
  $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery(implode(' OR ', $id_search));

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

  $item = culturefeed_agenda_get_active_object();
  if (!$item || $item->getType() != 'production') {
    return;
  }

  $production = $item->getEntity();
  $relations = $production->getRelations();
  if (empty($relations)) {
    return;
  }

  // Currently we do an extra query to load the event details.
  // Sven checks if this could be added to the event loading.
  $cdb_ids = array();
  foreach ($relations as $relation) {
    $cdb_ids[] = 'cdbid:' . $relation->getCdbid();
  }

  $parameters = array();
  $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery('type:event');
  $parameters[] = new \CultuurNet\Search\Parameter\Query(implode(' OR ', $cdb_ids));
  $parameters[] = new \CultuurNet\Search\Parameter\Group();

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

  $region_ids = array();

  $items = $result->getItems();
  // Get all flanders regions, so we can query the p1's of them.
  foreach ($items as $item) {
    $regions = $item->getEntity()->getCategories()->getCategoriesByType(CultureFeed_Cdb_Data_Category::CATEGORY_TYPE_FLANDERS_REGION);
    foreach ($regions as $region) {
      $region_ids[] = $region->getId();
    }
  }

  $preferred_language = culturefeed_search_get_preferred_language();
  $query = db_select('culturefeed_search_terms', 't');
  $query->innerJoin('culturefeed_search_terms', 'province', 'province.tid = t.p1');
  $query->condition('t.tid', $region_ids);
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
  
  $block['subject'] = t('Agenda');
  $block['content'] = array(
    '#theme' => 'culturefeed_agenda_production_program',
    '#items' => $result->getItems(),
    '#provinces' => $provinces,
  );

  return $block;

}
