<?php

/**
 * @file
 * Callback and handler implementations for administration pages.
 */

/**
 * Configuration form for general settings.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function culturefeed_ui_admin_settings() {

  $sort_options = array(
    CultureFeed_SearchUsersQuery::SORT_NUMBEROFACTIVITIES       => t('Number of activities'),
    CultureFeed_SearchUsersQuery::SORT_NUMBEROFLIKES            => t('Number of likes'),
    CultureFeed_SearchUsersQuery::SORT_NUMBEROFSHARESFACEBOOK   => t('Number of Facebook shares'),
    CultureFeed_SearchUsersQuery::SORT_NUMBEROFSHARESTWITTER    => t('Number of Twitter shares'),
    CultureFeed_SearchUsersQuery::SORT_NUMBEROFATTENDS          => t('Number of attends'),
    CultureFeed_SearchUsersQuery::SORT_NUMBEROFACTIVEACTIVITIES => t('Number of activities (aggregate)'),
  );

  $form['culturefeed_ui_block_most_active_user_sort'] = array(
    '#type' => 'select',
    '#title' => t('Most active user block : Sort by'),
    '#options' => $sort_options,
    '#default_value' => variable_get('culturefeed_ui_block_most_active_user_sort', CultureFeed_SearchUsersQuery::SORT_NUMBEROFACTIVEACTIVITIES),
  );

  $form['cookies'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings for displaying cookies notification'),
  );

  $form['cookies']['culturefeed_ui_cookie_bool'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable cookie notification'),
    '#default_value' => variable_get('culturefeed_ui_cookie_bool', 0),
  );

  $form['cookies']['culturefeed_ui_cookie_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Internal path to the page with cookie terms'),
    '#default_value' => variable_get('culturefeed_ui_cookie_path', ''),
  );

  return system_settings_form($form);
}