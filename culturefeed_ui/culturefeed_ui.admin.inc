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

  $form['culturefeed_ui_account_edit_intro'] = array(
    '#default_value' => variable_get('culturefeed_ui_account_edit_intro', t('Update your password or email address. Your username cannot be changed.')),
    '#title' => t('Account edit page: intro'),
    '#type' => 'textarea',
  );

  $form['culturefeed_ui_account_edit_email_description'] = array(
    '#default_value' => variable_get('culturefeed_ui_account_edit_email_description', t('This email address has been connected to your UiTPAS card. Any communication regarding your UiTPAS will be sent to this email address as well.')),
    '#title' => t('Account edit page: email description'),
    '#type' => 'textarea',
  );

  $form['culturefeed_ui_privacy_settings_intro'] = array(
    '#default_value' => variable_get('culturefeed_ui_privacy_settings_intro', t('Choose what personal information is visible to others in your <a href="@profile-url">public profile</a>. When choosing <em>Anonymous</em> only your username will be visible.', array('@profile-url' => url('user')))),
    '#title' => t('Privacy settings: intro'),
    '#type' => 'textarea',
  );

  $form['culturefeed_ui_privacy_settings_anonymous_tooltip'] = array(
    '#default_value' => variable_get('culturefeed_ui_privacy_settings_anonymous_tooltip', t('All your user activities will be set to private too.')),
    '#title' => t('Privacy settings: anonymous tooltip'),
    '#type' => 'textarea',
  );

  $form['culturefeed_ui_profile_edit_intro'] = array(
    '#default_value' => variable_get('culturefeed_ui_profile_edit_intro', t('Complete your profile information for a more personal experience. You <a href="/culturefeed/profile/privacy">decide what personal information</a> is visible to others in your <a href="@profile-url">public profile</a>. All your user activities can be managed in your <a href="@history-url">personal history</a>.', array('@profile-url' => url('user'), '@history-url' => url('culturefeed/activities')))),
    '#title' => t('Profile edit page: intro'),
    '#type' => 'textarea',
  );

  return system_settings_form($form);
}