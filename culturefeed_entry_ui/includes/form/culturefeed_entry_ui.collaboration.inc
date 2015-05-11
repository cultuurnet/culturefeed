<?php

/**
 * @file
 * Contains culturefeed_entry_ui.collaboration.inc.
 */

/**
 * Attaches the collaboration element to the form.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 * @param array $info
 *   The collaborative info.
 */
function culturefeed_entry_ui_attach_collaboration_element(&$form, $form_state, $info) {

  $keyword = $info['keyword'];

  $form['collaboration'][$keyword] = array(
    '#group' => 'vertical_tabs',
    '#title' => $keyword,
    '#type' => 'fieldset',
  );

  $form['collaboration'][$keyword]['title'] = array(
    '#default_value' => isset($info['title']) ? $info['title'] : '',
    '#required' => TRUE,
    '#title' => t('Title'),
    '#type' => 'textfield',
  );

  $form['collaboration'][$keyword]['image'] = array(
    '#default_value' => isset($info['description']['image']) ? $info['description']['image'] : '',
    '#title' => t('Image'),
    '#type' => 'textfield',
  );

  $form['collaboration'][$keyword]['article'] = array(
    '#default_value' => isset($info['description']['article']) ? $info['description']['article'] : '',
    '#title' => t('Article'),
    '#type' => 'textfield',
  );

  $form['collaboration'][$keyword]['text'] = array(
    '#default_value' => isset($info['description']['text']) ? $info['description']['text'] : '',
    '#required' => TRUE,
    '#title' => t('Short description'),
    '#type' => 'textarea',
  );

}

/**
 * Form builder for the collaboration tab.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 * @param \CultuurNet\Search\ActivityStatsExtendedEntity $event
 *   The event.
 *
 * @return array
 *   The form.
 */
function culturefeed_entry_ui_edit_collaboration_data_form($form, &$form_state, \CultuurNet\Search\ActivityStatsExtendedEntity $event) {

  /* @var \CultuurNet\Search\ActivityStatsExtendedEntity $event */
  $event = $event->getEntity();
  /* @var \CultureFeed_Cdb_Item_Event $event */
  $form['#event'] = $event;

  $custom_tags = variable_get('culturefeed_entry_ui_custom_collaboration_tags', array());
  if (!count($custom_tags)) {
    $tag = variable_get('site_name') . ' selectie';
    $custom_tags[] = array($tag => array('value' => $tag, 'visible' => FALSE));
  }

  // Start vertical tabs
  $form['vertical_tabs'] = array(
    '#type' => 'vertical_tabs',
  );

  $form['collaboration'] = array(
    '#tree' => TRUE,
  );

  form_load_include($form_state, 'inc', 'culturefeed_entry_ui', 'includes/helpers/culturefeed_entry_ui.collaboration');

  foreach ($custom_tags as $tag) {

    $info = culturefeed_entry_ui_collaboration_get_data_by_keyword($event, $tag['value']);
    culturefeed_entry_ui_attach_collaboration_element($form, $form_state, $info);
  }

  $form['#attached'] = array(
    'css' => array(
      drupal_get_path('module', 'culturefeed_entry_ui') . '/css/culturefeed_entry_ui.collaboration.css',
    ),
  );

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    ),
  );

  return $form;

}

/**
 * Submit handler for culturefeed_entry_ui_edit_collaboration_data_form().
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 */
function culturefeed_entry_ui_edit_collaboration_data_form_submit(&$form, &$form_state) {

  $consumer_key = variable_get('culturefeed_api_application_key', '');
  /* @var \CultureFeed_Cdb_Item_Event $event */
  $event = $form['#event'];

  if ($consumer_key && $event && isset($form_state['values']['collaboration'])) {

    foreach ($form_state['values']['collaboration'] as $keyword => $collaboration) {

      $title = $collaboration['title'];
      $copyright = '';
      $plain_text = $collaboration['text'];

      $description = drupal_json_encode(array(
        'keyword' => $keyword,
        'text' => $plain_text,
        'image' => $collaboration['image'],
        'article' => $collaboration['article'],
      ));


      Drupalculturefeed_EntryApi::addLinkToEvent(
        $event,
        '',
        CultureFeed_Cdb_Data_File::MEDIA_TYPE_ROADMAP,
        culturefeed_get_preferred_language(),
        $title,
        $copyright,
        $plain_text,
        $consumer_key,
        $description
      );

      // Also save the keyword directly as keyword.
      $keywords = array(
        $keyword => new CultureFeed_Cdb_Data_Keyword($keyword, FALSE),
      );
      Drupalculturefeed_EntryApi::addTagToEvent($event, $keywords);

    }

  }

}
