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

  $tags = array();
  foreach ($event_tags as $tag) {
    $tags[] = array(
      'value' => $tag->getValue(),
      'visible' => $tag->isVisible(),
    );
  }

  $form['#object_id'] = $event->getCdbId();

  $form['tags'] = array(
    '#type' => 'culturefeed_tags_element',
    '#default_value' => $tags,
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
function culturefeed_entry_ui_edit_tag_objects_form_submit($form, &$form_state) {

  // Merge custom tags into all tags.
  $tags = $form_state['values']['tags'];
  $keywords = array();
  $visibles = array();
  foreach ($tags as $tag) {
    $keywords[] = $tag['value'];
    $visibles[] = (isset($tag['visible']) && $tag['visible']) ? TRUE : FALSE;
  }

  $event = new CultureFeed_Cdb_Item_Event();
  $event->setCdbId($form['#object_id']);

  try {

    // Add new tags.
    if ($keywords) {
      Drupalculturefeed_EntryApi::addTagToEvent($event, $keywords, $visibles);
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
      form_set_error('tags][keywords][' . $key . '][value', t('The tag should have a value.'));
    }
  }

}
