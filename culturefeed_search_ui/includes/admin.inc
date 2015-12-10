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

  $form['culturefeed_search_ui_fb_app_id'] = array(
    '#title' => t('facebook App Id'),
    '#type' => 'textfield',
    '#default_value' => variable_get('culturefeed_search_ui_fb_app_id'),
    '#description' => t('Enter the facebook app id. When share links are clicked, the javascript SDK will be used instead of redirecting to facebook.'),
  );

  $form['culturefeed_multiple_categories_operator'] = array(
    '#type' => 'select',
    '#title' => t('Multiple categories operator'),
    '#description' => t('Determine which operator to use when selecting
    multiple categories.'),
    '#options' => array(
      'and' => t('Use \'AND\' operator'),
      'or' => t('Use \'OR\' operator'),
    ),
    '#default_value' => variable_get
    ('culturefeed_multiple_categories_operator', 'and'),
  );

  return system_settings_form($form);

}

/**
 * Admin form for sort order settings.
 */
function culturefeed_search_ui_admin_sort_order_settings_form() {

  $form = array();

  $form['description'] = array(
    '#markup' => t('<p>
      Define the different sorting options you want.<br />
      When a sorting option is not exposed. It won\'t be shown as option, but is applicable through the url.
      Example:<ul>
      <li>Label: Date</li>
      <li>Value: date</li>
      <li>Sort query for API: startdateday asc</li>
    </p>'
  ));

  $form['sort_options'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );

  $search_pages = culturefeed_get_searchable_types();
  foreach ($search_pages as $search_page) {

    $type = $search_page['id'];
    $sort_options = culturefeed_search_ui_get_sort_options_for_page($search_page);

    $form['sort_options'][$type] = array(
      '#type' => 'fieldset',
      '#title' => check_plain($search_page['page_title']),
      '#tree' => TRUE,
      '#theme' => 'culturefeed_search_ui_admin_sort_order_options',
    );

    for ($i = 0; $i < 10; $i++) {

      $value = isset($sort_options[$i]['value']) ? $sort_options[$i]['value'] : '';

      $form['sort_options'][$type][$i]['default'] = array(
        '#title' => t('Default'),
        '#type' => 'radio',
        '#parents' => array('sort_options', $type, 'default'),
        '#default_value' => (isset($sort_options['default']) && $i == $sort_options['default']) ? $i : FALSE,
        '#return_value' => $i,
      );

      $form['sort_options'][$type][$i]['exposed'] = array(
        '#title' => t('Exposed'),
        '#type' => 'checkbox',
        '#default_value' => isset($sort_options[$i]['exposed']) ? $sort_options[$i]['exposed'] : $i
      );

      $form['sort_options'][$type][$i]['label'] = array(
        '#title' => t('Label'),
        '#type' => 'textfield',
        '#default_value' => isset($sort_options[$i]['label']) ? $sort_options[$i]['label'] : '',
      );

      $form['sort_options'][$type][$i]['value'] = array(
        '#title' => t('Value used in url'),
        '#type' => 'textfield',
        '#default_value' => $value,
      );

      $form['sort_options'][$type][$i]['query'] = array(
        '#title' => t('Sortquery for api'),
        '#type' => 'textfield',
        '#default_value' => isset($sort_options[$i]['query']) ? $sort_options[$i]['query'] : '',
      );

    }

  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
    '#submit' => array('culturefeed_search_ui_admin_sort_order_settings_form_submit'),
  );

  return $form;
}


/**
 * Submit the admin sort order settings form.
 */
function culturefeed_search_ui_admin_sort_order_settings_form_submit(&$form, &$form_state) {

  if (!empty($form_state['values']['sort_options'])) {

    // Don't save empty options.
    foreach ($form_state['values']['sort_options'] as $type => $options) {

      $default_sort = $form_state['values']['sort_options'][$type]['default'];
      foreach ($options as $key => &$option) {

        if ($key == 'default') {
          continue;
        }

        if (empty($option['value'])) {
          unset($options[$key]);
        }
      }

      variable_set('culturefeed_search_sortoptions_' . $type, $options);
    }
  }
}
