<?php

/**
 * @file
 * Plugin to provide a context for uitpas user.
 */

$plugin = array(
  'title' => t('Uitpas: user'),
  'description' => t('Uitpas: user.'),
  'context' => 'culturefeed_context_uitpas_user',
  'context name' => 'uitpas_user',
  'convert list' => array(
    /*'location' => t('Location'),
    'locations' => t('Locations (with lid)'),
    'views argument' => t('Views argument (lid)'),*/
  ),
  'convert' => 'culturefeed_context_uitpas_user_convert',
  'keyword' => 'uitpas_user',
);

function culturefeed_context_uitpas_user($empty, $data = NULL, $conf = FALSE) {
  //setting our locations
  //creating our context
  $context = new ctools_context('uitpas_user');
  $context->plugin = 'uitpas_user';
  $context->data = new stdClass();
  //$context->data->locations = $locations;
  //$context->data->location = $location;
  return $context;
}

function culturefeed_context_uitpas_user_convert($context, $type) {
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