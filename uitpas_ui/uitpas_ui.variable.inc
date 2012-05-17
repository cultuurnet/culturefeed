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
    'uitpas_ui_advantages_promotions_info' => array(
      'title' => t('Uitpas UI: advantages promotions info'),
      'type' => 'text_format',
      'localize' => TRUE,
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_locations' => array(
      'title' => t('Uitpas UI: locations'),
      'description' => t('Put 1 location per line'),
      'type' => 'text',
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_register_uitpas_intro' => array(
      'title' => t('Uitpas UI: register uitpas intro'),
      'type' => 'text_format',
      'localize' => TRUE,
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_register_where_intro' => array(
      'title' => t('Uitpas UI: register where intro'),
      'type' => 'text_format',
      'localize' => TRUE,
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_register_where_info' => array(
      'title' => t('Uitpas UI: register where info'),
      'type' => 'text_format',
      'localize' => TRUE,
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_registered_intro' => array(
      'title' => t('Uitpas UI: registered intro'),
      'type' => 'text_format',
      'localize' => TRUE,
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_sidebar_bottom' => array(
      'title' => t('Uitpas UI: sidebar bottom'),
      'type' => 'text_format',
      'localize' => TRUE,
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_user_default_image' => array(
      'title' => t('Uitpas UI: user default image'),
      'type' => 'string',
      'default' => 'http://media.uitid.be/fis/rest/download/ce126667652776f0e9e55160f12f5478/uiv/default.png',
      'localize' => TRUE,
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_user_profile_details_intro' => array(
      'title' => t('Uitpas UI: user profile details intro'),
      'type' => 'text_format',
      'localize' => TRUE,
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_user_profile_navigation_account_edit_title' => array(
      'title' => t('Uitpas UI: user profile navigation account edit title'),
      'type' => 'string',
      'default' => t('Edit account'),
      'localize' => TRUE,
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_user_profile_navigation_account_edit_path' => array(
      'title' => t('Uitpas UI: user profile navigation account edit path'),
      'type' => 'string',
      'default' => 'myprofile/account_edit',
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_user_profile_navigation_account_edit_body' => array(
      'title' => t('Uitpas UI: user profile navigation account edit body'),
      'type' => 'text_format',
      'localize' => TRUE,
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_user_profile_navigation_activities_title' => array(
      'title' => t('Uitpas UI: user profile navigation activities title'),
      'type' => 'string',
      'default' => t('User history'),
      'localize' => TRUE,
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_user_profile_navigation_activities_path' => array(
      'title' => t('Uitpas UI: user profile navigation activities path'),
      'type' => 'string',
      'default' => 'myprofile/activities',
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_user_profile_navigation_activities_body' => array(
      'title' => t('Uitpas UI: user profile navigation activities body'),
      'type' => 'text_format',
      'localize' => TRUE,
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_user_profile_navigation_advantages_title' => array(
      'title' => t('Uitpas UI: user profile navigation advantages title'),
      'type' => 'string',
      'default' => t('My advantages'),
      'localize' => TRUE,
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_user_profile_navigation_advantages_path' => array(
      'title' => t('Uitpas UI: user profile navigation advantages path'),
      'type' => 'string',
      'default' => 'myprofile/advantages',
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_user_profile_navigation_advantages_body' => array(
      'title' => t('Uitpas UI: user profile navigation advantages body'),
      'type' => 'text_format',
      'localize' => TRUE,
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_user_profile_navigation_details_title' => array(
      'title' => t('Uitpas UI: user profile navigation details title'),
      'type' => 'string',
      'default' => t('My details'),
      'localize' => TRUE,
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_user_profile_navigation_details_path' => array(
      'title' => t('Uitpas UI: user profile navigation details path'),
      'type' => 'string',
      'default' => 'myprofile/details',
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_user_profile_navigation_details_body' => array(
      'title' => t('Uitpas UI: user profile navigation details body'),
      'type' => 'text_format',
      'localize' => TRUE,
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_user_profile_navigation_notifications_title' => array(
      'title' => t('Uitpas UI: user profile navigation notifications title'),
      'type' => 'string',
      'default' => t('My notifications'),
      'localize' => TRUE,
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_user_profile_navigation_notifications_path' => array(
      'title' => t('Uitpas UI: user profile navigation notifications path'),
      'type' => 'string',
      'default' => 'myprofile/notifications',
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_user_profile_navigation_notifications_body' => array(
      'title' => t('Uitpas UI: user profile navigation notifications body'),
      'type' => 'text_format',
      'localize' => TRUE,
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_user_profile_navigation_uitid_title' => array(
      'title' => t('Uitpas UI: user profile navigation uitid title'),
      'type' => 'string',
      'default' => t('My UITID Profile'),
      'localize' => TRUE,
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_user_profile_navigation_uitid_path' => array(
      'title' => t('Uitpas UI: user profile navigation uitid path'),
      'type' => 'string',
      'default' => 'myprofile/uitid',
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_user_profile_navigation_uitid_body' => array(
      'title' => t('Uitpas UI: user profile navigation uitid body'),
      'type' => 'text_format',
      'localize' => TRUE,
      'group' => 'uitpas_ui',
    ),
   'uitpas_ui_user_profile_notifications_intro' => array(
      'title' => t('Uitpas UI: user profile notifications intro'),
      'type' => 'text_format',
      'localize' => TRUE,
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_user_profile_uitid_info' => array(
      'title' => t('Uitpas UI: user profile uitid info'),
      'type' => 'text_format',
      'localize' => TRUE,
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_validation_intro' => array(
      'title' => t('Uitpas UI: validation intro'),
      'type' => 'text_format',
      'localize' => TRUE,
      'group' => 'uitpas_ui',
    ),
    'uitpas_ui_validation_invalid_dob_error_message' => array(
      'title' => t('Uitpas UI: validation invalid dob error message'),
      'type' => 'string',
      'group' => 'uitpas_ui',
      'localize' => TRUE,
      'default' => t('Invalid date of birth'),
    ),
    'uitpas_ui_validation_parse_invalid_uitpasnumber_error_message' => array(
      'title' => t('Uitpas UI: validation invalid uitpasnumber error message'),
      'type' => 'string',
      'group' => 'uitpas_ui',
      'localize' => TRUE,
      'default' => t('Invalid uitpas number'),
    ),
    'uitpas_ui_validation_user_already_activated_error_message' => array(
      'title' => t('Uitpas UI: validation user already activated error message'),
      'type' => 'string',
      'group' => 'uitpas_ui',
      'localize' => TRUE,
      'default' => t('User already activated'),
    ),
  );
}