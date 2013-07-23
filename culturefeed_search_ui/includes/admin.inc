<?php

/**
 * @file
 * Callback and handler implementations for administration pages for culturefeed search UI.
 */

/**
 * Configuration form for general settings.
 */
function culturefeed_search_ui_admin_settings() {

  $form['culturefeed_search_ui_pager_items_per_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of items per page'),
    '#default_value' => variable_get('culturefeed_search_ui_pager_items_per_page', CULTUREFEED_SEARCH_UI_PAGER_ITEMS_PER_PAGE),
  );

  $form['culturefeed_search_ui_pager_type'] = array(
    '#type' => 'select',
    '#title' => t('Pager type for the search page(s)'),
    '#description' => t('The type of pager to use on search pages. This will trigger different templates as well.'),
    '#options' => array(
      CULTUREFEED_SEARCH_UI_PAGER_NORMAL => t('Normal pager with page numbers'),
      CULTUREFEED_SEARCH_UI_PAGER_INFINITE_SCROLL => t('Pager with button to manually scroll to infinity'),
    ),
    '#default_value' => variable_get('culturefeed_search_ui_pager_type', CULTUREFEED_SEARCH_UI_PAGER_NORMAL),
  );

  $form['culturefeed_search_redirect_old_search'] = array(
    '#type' => 'checkbox',
    '#title' => t('Redirect old cnapi urls to the new system'),
    '#description' => t('If enabled, all urls that where used by the cnapi module, will be redirected to urls for the new search api.'),
    '#default_value' => variable_get('culturefeed_search_redirect_old_search', TRUE),
  );

  return system_settings_form($form);

}
