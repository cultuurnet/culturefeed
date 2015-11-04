<?php

/**
 * Contains culturefeed_entry_ui.location.inc.
 */

/**
 * Adds the tags form element.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 * @param \CultureFeed_Cdb_Item_Event $event
 *   The event.
 */
function culturefeed_entry_ui_tags_form(array &$form, array &$form_state, \CultureFeed_Cdb_Item_Event $event = NULL) {
  if (culturefeed_entry_ui_is_capable_of_cdbxml_version_3_3()) {

    // Default values.
    $keywords = array();
    if ($event && $event->getKeywords(TRUE)) {

      /* @var \CultureFeed_Cdb_Data_Keyword $keyword */
      foreach ($event->getKeywords(TRUE) as $keyword) {
        $keywords[$keyword->getValue()] = array(
          'value' => $keyword->getValue(),
          'visible' => $keyword->isVisible(),
        );
      }

    }

    // Form element.
    $form['extra']['tags'] = array(
      '#type' => 'culturefeed_tags_element',
      '#title' => t('Tags'),
      '#description' => t('Add Add tags'),
      '#default_value' => $keywords,
    );

    $form['extra']['old_tags'] = array(
      '#type' => 'value',
      '#value' => $keywords,
    );

  }

  else {

    // Default values.
    if ($event && $event->getKeywords()) {
      $keywords = $event->getKeywords();
      $keywords = implode(';', $keywords);
    }
    else {
      $keywords = '';
    }

    // Form element.
    $form['extra']['tags'] = array(
      '#type' => 'textfield',
      '#title' => t('Tags'),
      '#description' => t('Add multiple tags separated by a ";"'),
      '#default_value' => $keywords,
    );

  }

}

/**
 * Save handler for the tags.
 *
 * @param \CultureFeed_Cdb_Item_Event $event
 *   The event.
 * @param array $form_state
 *   The form state.
 */
function culturefeed_entry_ui_tags_form_save(\CultureFeed_Cdb_Item_Event &$event, array $form_state) {

  $version = variable_get('culturefeed_cdb_version', CULTUREFEED_CDB_DEFAULT_VERSION);
  if (version_compare($version, '3.3', '>=')) {

    $tags = $form_state['values']['tags'];
    $old_tags = $form_state['values']['old_tags'];
    $custom_entry_api_tags = variable_get('culturefeed_entry_ui_custom_entry_api_tags', array());
    $keywords = array_unique(array_merge($tags, $custom_entry_api_tags), SORT_REGULAR);

    foreach ($keywords as $keyword) {
      $event->addKeyword(new CultureFeed_Cdb_Data_Keyword($keyword['value'], $keyword['visible']));
    }

    foreach ($old_tags as $tag => $keyword) {
      if (!in_array($tag, array_keys($keywords))) {
        $event->deleteKeyword(new CultureFeed_Cdb_Data_Keyword($keyword['value'], $keyword['visible']));
      }
    }

  }

  else {

    $keywords_form = array();
    $custom_entry_api_tags = array();
    if (!empty($form_state['values']['keywords'])) {
      $keywords_form = explode(';', $form_state['values']['tags']);
    }
    if (variable_get('culturefeed_entry_ui_custom_entry_api_tags')) {
      $custom_entry_api_tags = explode("\n", variable_get('culturefeed_entry_ui_custom_entry_api_tags'));
    }
    $keywords = array_map('trim',array_unique(array_merge($keywords_form,$custom_entry_api_tags)));
    foreach ($keywords as $keyword) {
      $event->addKeyword($keyword);
    }

  }

}
