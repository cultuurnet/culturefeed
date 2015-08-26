<?php

/**
 * @file
 * Contains culturefeed_entry_ui.tag_objects.inc.
 */

/**
 * Form builder for the tags form for cdb 3.3 and above.
 */
function culturefeed_entry_ui_edit_tag_objects_form($form, &$form_state, $event) {

  /* @var \CultuurNet\Search\ActivityStatsExtendedEntity $event */
  $event = $event->getEntity();
  /* @var \CultureFeed_Cdb_Item_Event $event */
  /* @var \CultureFeed_Cdb_Data_Keyword[] $event_tags */
  $event_tags = $event->getKeywords(TRUE);
  $custom_tags = variable_get('culturefeed_entry_ui_custom_tags', array());
  $form['#old_tags'] = $event_tags;
  $form['#object_id'] = $event->getCdbId();

  // Tags.
  $tags = array();
  foreach ($event_tags as $tag) {

    $value = $tag->getValue();

    if (!in_array($value, array_keys($custom_tags))) {

      $tags[] = array(
        'value' => $tag->getValue(),
        'visible' => $tag->isVisible(),
      );

    }

  }

  $form['tags'] = array(
    '#default_value' => $tags,
    '#title' => t('Tags'),
    '#type' => 'culturefeed_tags_element',
  );

  if (user_access('manage custom tags') && !empty($custom_tags)) {

    $form['custom_tags'] = array(
      '#default_value' => array_intersect(array_keys($event_tags), array_keys($custom_tags)),
      '#options' => array_combine(array_keys($custom_tags), array_keys($custom_tags)),
      '#title' => t('Custom tags'),
      '#type' => 'checkboxes',
    );

  }

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#validate' => array('culturefeed_entry_ui_edit_tag_objects_form_submit_validate'),
      '#value' => t('Save'),
    ),
  );

  return $form;

}

/**
 * Submit handler for culturefeed_entry_ui_edit_tag_objects_form().
 */
function culturefeed_entry_ui_edit_tag_objects_form_submit($form, &$form_state) {

  // Merge custom tags into all tags.
  $keywords = array();
  foreach ($form_state['values']['tags'] as $tag) {
    $keywords[$tag['value']] = new CultureFeed_Cdb_Data_Keyword($tag['value'], isset($tag['visible']) ? $tag['visible'] : TRUE);
  }

  if (isset($form_state['values']['custom_tags'])) {

    $custom_tags = variable_get('culturefeed_entry_ui_custom_tags', array());
    foreach ($form_state['values']['custom_tags'] as $tag) {
      if (isset($custom_tags[$tag])) {
        $keywords[$custom_tags[$tag]['value']] = new CultureFeed_Cdb_Data_Keyword($custom_tags[$tag]['value'], isset($custom_tags[$tag]['visible']) ? $custom_tags[$tag]['visible'] : TRUE);
      }
    }

  }

  // Check what tags are removed.
  $removed_tags = array();
  if (!empty($form['#old_tags'])) {
    foreach (array_keys($form['#old_tags']) as $tag) {
      if (!in_array($tag, array_keys($keywords))) {
        $removed_tags[] = $tag;
      }
    }
  }

  $event = new CultureFeed_Cdb_Item_Event();
  $event->setCdbId($form['#object_id']);

  try {

    // Add new tags.
    if ($keywords) {
      Drupalculturefeed_EntryApi::addTagToEvent($event, $keywords);
    }

    // Remove old tags.
    if ($removed_tags) {
      foreach ($removed_tags as $removed_tag) {
        Drupalculturefeed_EntryApi::removeTagFromEvent($event, $removed_tag);
      }
    }

    drupal_set_message(t('The tags were adjusted.'));
    cache_clear_all('culturefeed:results:detail:event:' . $event->getCdbId(), 'cache_culturefeed_search');
    drupal_goto(culturefeed_search_detail_path('event', $event->getCdbId(), ''));

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

  $tags = $form_state['values']['tags'];
  $custom_tags = variable_get('culturefeed_entry_ui_custom_tags', array());

  foreach (array_keys($tags) as $key => $tag) {
    if (in_array($tag, array_keys($custom_tags))) {
      form_set_error('tags][keywords][' . $key, t("Tag @tag is not permitted because it's already a custom tag.", array('@tag' => $tag)));
    }
  }

}
