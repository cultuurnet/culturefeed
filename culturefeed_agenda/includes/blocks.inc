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
    );

    $form_state['#domain_id'] = $domain_id;

  }

  $form['search'] = array(
    '#type' => 'textfield',
    '#size' => 28,
    '#title' => t('Search string'),
    '#default_value' => isset($_GET['search']) ? $_GET['search'] : '',
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

  $block['subject'] = t('Program schedule');
  $block['content'] = array(
    '#theme' => 'culturefeed_agenda_production_program',
    '#items' => $items,
    '#provinces' => $provinces,
  );

  return $block;

}

function culturefeed_agenda_block_actor_search_facet() {

  // Only go if we're on an agenda search page.
  if (current_path() != 'agenda/search') {
    return;
  }

  // Only show this block if allowed in the block settings.
  $current_event = isset($_GET['facet']['category_eventtype_id'][0]) ? $_GET['facet']['category_eventtype_id'][0] : '';
  $saved_events = variable_get('agenda_actor_search_visibility');
  $allowed_events = array();

  foreach ($saved_events as $saved_event) {
    if ($saved_event != 0) {
      $allowed_events[] = $saved_event;
    }
  }

  if (!in_array($current_event, $allowed_events)) {
    return;
  }

  // Create build array with actor search form and nearby actors.
  $build = array();
  $build['form'] = drupal_get_form('culturefeed_agenda_block_actor_search_form');
  $build['actors_nearby'] = culturefeed_agenda_get_actors_nearby_for_event($current_event);

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
    array('data' => CULTUREFEED_GOOGLE_MAPS_JS_API, 'type' => 'external'),
    drupal_get_path('module', 'culturefeed_agenda') . '/js/autocomplete_autosubmit.js',
  );
  $form['#attached']['library'] = array(
    array('system', 'jquery.cookie'),
  );

  $form['search'] = array(
    '#type' => 'textfield',
    '#size' => 28,
    '#title' => t('Search string'),
    '#title_display' => 'invisible',
    '#attributes' => array('class' => array('auto-submit-field'), 'placeholder' => t('Plaats of naam')),
    '#autocomplete_path' => 'autocomplete/culturefeed/agenda/actor-search'
  );

  return $form;
}





