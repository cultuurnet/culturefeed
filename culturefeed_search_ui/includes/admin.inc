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
    '#default_value' => variable_get('culturefeed_search_redirect_old_search', FALSE),
  );

  $form['culturefeed_search_ui_facet_compatibility_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Compatibility markup'),
    '#description' => t('If enabled, show the old facet block markup. This limits the visible levels to 2.'),
    '#default_value' => variable_get('culturefeed_search_ui_facet_compatibility_mode', 0),
  );

  $form['culturefeed_search_group'] = array(
    '#type' => 'textfield',
    '#title' => t('Group'),
    '#description' => t('The data is now fully denormalized so that 1 document = 1 time-based event. So you get the same event multiple times per opening day or hour. To group the results per event, the group = event parameter can be used. To group multiple events per production you can use group=event_production.'),
    '#default_value' => variable_get('culturefeed_search_group', 'event_production'),
  );

  $form['culturefeed_search_query_operator'] = array(
    '#type' => 'select',
    '#title' => t('Search query operator'),
    '#description' => t('Determine which operator to use in the search query.'),
    '#options' => array(
      'and' => t('Use \'AND\' operator'),
      'or' => t('Use \'OR\' operator'),
    ),
    '#default_value' => variable_get('culturefeed_search_query_operator', 'and'),
  );

  return system_settings_form($form);

}
