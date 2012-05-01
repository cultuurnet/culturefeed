<?php

/**
 * @file
 * Plugin to provide a context for uitpas location.
 */

$plugin = array(
  'title' => t('Uitpas: location'),
  'description' => t('Uitpas: location.'),
  'context' => 'culturefeed_context_uitpas_location',
  'context name' => 'uitpas_location',
  'convert list' => array(
    'location' => t('Location'),
    'locations' => t('Locations (with lid)'),
    'views argument' => t('Views argument (lid)'),
  ),
  'convert' => 'culturefeed_context_uitpas_location_convert',
  'keyword' => 'uitpas_location',
);

function culturefeed_context_uitpas_location($empty, $data = NULL, $conf = FALSE) {
  //setting our locations
  $location_options = variable_get_value('culturefeed_uitpas_locations');
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
  /*if ($user) {
    $location = $user->location;
  }*/
  if (isset($_COOKIE['uitpas_location'])) {
    $location = $_COOKIE['uitpas_location'];
  }
  if ($data) {
    $location = $data;
  }
  if (!isset($locations[$location])) {
    $location = '';
  }
  //creating our context
  $context = new ctools_context('uitpas_location');
  $context->plugin = 'uitpas_location';
  $context->data = new stdClass();
  $context->data->locations = $locations;
  $context->data->location = $location;
  return $context;
}

function culturefeed_context_uitpas_location_convert($context, $type) {
  switch ($type) {
    case 'locations':
      return $context->data->locations;
    case 'location':
      return $context->data->location;
    case 'views argument':
      //@TODO we should probably define the condition on 'elders' in another way
      if ($context->data->location == 'elders') {
        $args = array();
        foreach ($context->data->locations as $location) {
          if ($location['lid']) {
            $args[] = $location['lid'];
          }
        }
        return implode('|', $args);
      }
      else if ($context->data->location) {
        return $context->data->locations[$context->data->location]['lid'];
      }
  }
}