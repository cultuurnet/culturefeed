<?php
/**
 * @file
* Variable module hook implementations
*/

/**
 * Implements hook_variable_group_info().
 */ 
function culturefeed_variable_group_info() {
  return array(
    'culturefeed' => array(
      'title' => t('Culturefeed'),
      'description' => t('Culturefeed'),
	),
  );
}

/**
 * Implements hook_variable_info().
 */
function culturefeed_variable_info() {
  return array(
    'culturefeed_uitpas_locations' => array(
      'title' => t('Culturefeed: uitpas locations'),
      'description' => t('Put 1 location per line'),
      'type' => 'text',
      'children' => array('text'),
      'group' => 'culturefeed',
    ),
  );
}