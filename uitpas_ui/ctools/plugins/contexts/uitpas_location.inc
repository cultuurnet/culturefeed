<?php

/**
 * @file
 * Plugin to provide a context for uitpas location.
 */

$plugin = array(
  'title' => t('Uitpas: location'),
  'description' => t('Uitpas: location.'),
  'context' => 'uitpas_ui_context_uitpas_location',
  'context name' => 'uitpas_location',
  'convert list' => array(
    'location' => t('Location'),
    'locations' => t('Locations (with lid)'),
    'views argument' => t('Views argument (lid)'),
  ),
  'convert' => 'uitpas_ui_context_uitpas_location_convert',
  'keyword' => 'uitpas_location',
);

function uitpas_ui_context_uitpas_location($empty, $data = NULL, $conf = FALSE) {
  //setting our locations
  $location_options = variable_get_value('uitpas_ui_locations');
  $location_options = array_map('trim', explode("\n", $location_options));
  $locations = array();
  foreach($location_options as $option) {
    $sql = "SELECT lid FROM {cnapi_location} WHERE type = 'region' && name = :name";
    $lid = db_query($sql, array(':name' => $option))->fetchField();
    $locations[strtolower($option)] = array(
      'name' => $option,
      'lid' => $lid,
    );
  }
  //getting the active location
  $location = '';
  if (DrupalCultureFeed::isCultureFeedUser()) {
    try {
      $cf_user = DrupalCultureFeed::getLoggedInUser();
      if ($cf_user->city) {
        $location = $cf_user->city;
      }
    }
    catch (Exception $e) {
      watchdog_exception('uitpas_ui_context_uitpas_location', $e);
    }
  }
  //we only want the cookie to kick in on pages with a location argument (selector)
  if (isset($_COOKIE['uitpas_location'])) {
    $location = $_COOKIE['uitpas_location'];
  }
  if (!$conf && $data) {
    $location = $data;
  }
  if (!isset($locations[$location])) {
    $location = 'all';
  }
  setcookie('uitpas_location', $location, 0, '/');
  //creating our context
  $context = new ctools_context('uitpas_location');
  $context->plugin = 'uitpas_location';
  $context->data = new stdClass();
  $context->data->locations = $locations;
  $context->data->location = $location;
  //a validation check for our respons code selection rule as optional arguments always validate
  $context->data->valid_argument = TRUE;
  if (!$empty && !$conf && $data && $data != $location) {
    $context->data->valid_argument = FALSE;
  }
  //views and culturefeed arguments
  $context->data->views_argument = '';
  $context->data->cf_query_argument = '';
  if ($context->data->location == 'all') {
    $context->data->views_argument = $context->data->location;
    $context->data->cf_query_argument = '';
  }
  //@TODO we should probably define the condition on 'elders' in another way
  else if ($context->data->location == 'elders') {
    $args = array();
    foreach ($context->data->locations as $location) {
      if ($location['lid']) {
        $args[] = "!" . $location['lid'];
      }
    }
    $context->data->views_argument = implode('|', $args);
    $context->data->cf_query_argument = implode(',', $args);
  }
  else {
    $context->data->views_argument = $context->data->locations[$context->data->location]['lid'];
    $context->data->cf_query_argument = $context->data->locations[$context->data->location]['name'];
  }
  return $context;
}

function uitpas_ui_context_uitpas_location_convert($context, $type) {
  switch ($type) {
    case 'locations':
      return $context->data->locations;
    case 'location':
      return $context->data->location;
    case 'views argument':
      return $context->data->views_argument;
  }
}