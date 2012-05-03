<?php
/**
 * @file
* Variable module hook implementations
*/

/**
 * Implements hook_variable_group_info().
 */ 
function uitpas_ui_variable_group_info() {
  return array(
    'uitpas_ui' => array(
      'title' => t('Uitpas UI'),
      'description' => t('Uitpas UI'),
	),
  );
}

/**
 * Implements hook_variable_info().
 */
function uitpas_ui_variable_info() {
  return array(
    'uitpas_ui_locations' => array(
      'title' => t('Uitpas UI: locations'),
      'description' => t('Put 1 location per line'),
      'type' => 'text',
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_invalid_dob_error_message' => array(
      'title' => t('Uitpas UI: invalid dob error message'),
      'type' => 'string',
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_parse_invalid_uitpasnumber_error_message' => array(
      'title' => t('Uitpas UI: invalid uitpasnumber error message'),
      'type' => 'string',
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_parse_user_already_activated_error_meesage' => array(
      'title' => t('Uitpas UI: user already activated error message'),
      'type' => 'string',
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_user_profile_details_intro' => array(
      'title' => t('Uitpas UI: user aprofile details intro'),
      'type' => 'text_format',
      'group' => 'uitpas_ui',
    ),
  );
}