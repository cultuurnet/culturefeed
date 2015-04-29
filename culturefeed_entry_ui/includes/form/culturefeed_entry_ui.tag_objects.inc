<?php

/**
 * @file
 * Contains culturefeed_entry_ui.tag_objects.inc.
 */

/**
 * Ajax callback for the edit tags form.
 */
function culturefeed_entry_ui_edit_tag_objects_ajax_callback($form, $form_state) {
  return $form['wrapper'];
}

/**
 * Form builder for the tags form for cdb 3.3 and above.
 */
function culturefeed_entry_ui_edit_tag_objects_form($form, &$form_state, $event) {

  /* @var \CultuurNet\Search\ActivityStatsExtendedEntity $event */
  $event = $event->getEntity();
  /* @var \CultureFeed_Cdb_Item_Event $event */
  /* @var \CultureFeed_Cdb_Data_Keyword[] $event_tags */
  $event_tags = $event->getKeywords(TRUE);

  // Tags.
  $tags = array();
  if (!isset($form_state['values']['tags'])) {

    foreach ($event_tags as $tag) {
      $tags[] = array(
        'value' => $tag->getValue(),
        'visible' => $tag->isVisible(),
      );
    }

  }

  else {
    $tags = $form_state['values']['tags'];
  }

  // Set the form.
  $html_id = 'tag-objects-wrapper';

  $form['wrapper'] = array(
    '#type' => 'container',
    '#attributes' => array('id' => $html_id),
  );

  $form['wrapper']['tags'] = array(
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'culturefeed_entry_ui') . '/css/edit_tag_objects_form.css',
      ),
    ),
    '#attributes' => array('class' => array('edit-tag-objects-form-table')),
    '#theme' => 'culturefeed_entry_ui_edit_tag_objects_form_table',
    '#tree' => TRUE,
  );

  foreach ($tags as $key => $tag) {

    $tag_elements = array(
      'value' => array(
        '#default_value' => $tag['value'],
        '#type' => 'textfield',
      ),
      'visible' => array(
        '#default_value' => $tag['visible'],
        '#title' => t('Hide tag'),
        '#type' => 'checkbox',
      ),
    );

    if (count($tags) > 1) {

      $tag_elements['remove'] = array(
        '#ajax' => array(
          'callback' => 'culturefeed_entry_ui_edit_tag_objects_ajax_callback',
          'wrapper' => $html_id,
        ),
        '#name' => 'remove_' . $key,
        '#submit' => array('culturefeed_entry_ui_edit_tag_objects_form_process'),
        '#type' => 'submit',
        '#value' => t('Remove tag'),
      );

    }

    $form['wrapper']['tags'][] = $tag_elements;

  }

  $form['add_tag'] = array(
    '#ajax' => array(
      'callback' => 'culturefeed_entry_ui_edit_tag_objects_ajax_callback',
      'wrapper' => $html_id,
    ),
    '#submit' => array('culturefeed_entry_ui_edit_tag_objects_form_process'),
    '#type' => 'submit',
    '#value' => t('Add tag'),
  );

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#validate' => array('culturefeed_entry_ui_edit_tag_objects_form_submit_validate'),
      '#value' => t('Submit'),
    ),
  );

  return $form;

}

/**
 * Submit handler for culturefeed_entry_ui_edit_tag_objects_form().
 */
function culturefeed_entry_ui_edit_tag_objects_form_process($form, &$form_state) {

  // Triggering data.
  $triggering_element = isset($form_state['triggering_element']) ? $form_state['triggering_element'] : '';
  $triggering_parents = ($triggering_element) ? array_reverse($triggering_element['#parents']) : array();

  // Check if a tag needs to be added.
  if (isset($triggering_parents[0]) && $triggering_parents[0] == 'add_tag') {
    $tag = array('value' => '', 'visible' => 0);
    $form_state['values']['tags'][] = $tag;
  }

  // Check if a tag needs to be removed.  This is done here as renumbering
  // needs to be ensured.
  if (isset($triggering_parents[0]) && $triggering_parents[0] == 'remove') {

    $key = $triggering_element['#parents'][1];
    unset($form_state['values']['tags'][$key]);
    $form_state['values']['tags'] = array_values($form_state['values']['tags']);
    unset($form_state['input']['tags'][$key]);
    $form_state['input']['tags'] = array_values($form_state['input']['tags']);

  }

  $form_state['rebuild'] = TRUE;

}

/**
 * Submit handler for culturefeed_entry_ui_edit_tag_objects_form().
 */
function culturefeed_entry_ui_edit_tag_objects_form_submit($form, &$form_state) {

  // Merge custom tags into all tags.
  $tags = $form_state['values']['tags'];
  $tag_objects = array();
  foreach ($tags as $tag) {
    $tag_objects[] = new CultureFeed_Cdb_Data_Keyword($tag['value'], $tag['visible']);
  }

  $event = new CultureFeed_Cdb_Item_Event();
  $event->setCdbId($form['#object_id']);

  try {

    // Add new tags.
    if ($tags) {
      Drupalculturefeed_EntryApi::addTagToEvent($event, $tag_objects);
    }

    drupal_set_message(t('The tags were adjusted. However, it takes half an hour before these changes on all channels (including Uitinvlaanderen) will be. Available under the "View" tab'));

  }

  catch (Exception $e) {

    if ($e->getCode() == CultureFeed_EntryApi::CODE_KEYWORD_PRIVATE) {
      drupal_set_message(t('You tried to add a tag that should not be added by anyone. If you still wish to add this or have other questions about this, please email to "vragen@uitdatabank.be".'));
    }
    else {
      drupal_set_message(t('Error occured while saving the tags'));
    }

    watchdog_exception(WATCHDOG_ERROR, $e);

  }

}

/**
 * Validation handler for culturefeed_entry_ui_edit_tag_objects_form().
 */
function culturefeed_entry_ui_edit_tag_objects_form_submit_validate($form, &$form_state) {

  foreach ($form_state['values']['tags'] as $key => $tag) {
    if (!$tag['value']) {
      form_set_error('tags][' . $key . '][value', t('The tag should have a value.'));
    }
  }

}
