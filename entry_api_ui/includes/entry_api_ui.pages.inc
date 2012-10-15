<?php
/**
 * @file
 * Page callbacks for the entry api ui.
 */

/**
 * Form callback: Show the manage tags form for a cnapi object.
 */
function entry_api_ui_edit_tags_form($form, $form_state, $node) {

  $form = array();
  try {
    $event = DrupalCultureFeed_EntryApi::getEvent('cc83f911-2ad7-47c5-8bf3-14de3397099c');
  }
  catch (Excpeption $e) {
    watchdog_exception(WATCHDOG_ERROR, $e);
    drupal_set_message('Fout bij het laden van dit event', 'error');
    return $form;
  }

  $event_tags = $event->getKeywords();

  $lifestyle_tags = array();

  $lifestyle_options = array(
    CultureFeed_User::LIFESTYLE_ONTDEKKER => 'Ontdekker',
    CultureFeed_User::LIFESTYLE_FIJNPROEVER => 'Fijnproever',
    CultureFeed_User::LIFESTYLE_ACTIE_ZOEKER => 'Actiezoeker',
    CultureFeed_User::LIFESTYLE_ACTIEVE_ONTSPANNER => 'Actieve ontspanner',
  );

  // Filter out lifestyle tags
  foreach ($event_tags as $tag) {
    if (array_key_exists($tag, $lifestyle_options)) {
      unset($event_tags[$tag]);
      $lifestyle_tags[$tag] = $tag;
    }
  }

  $form['#object_id'] = 'cc83f911-2ad7-47c5-8bf3-14de3397099c';

  if (user_access('manage lifestyle profile tags')) {
    $form['lifestyle_profile'] = array(
      '#type' => 'checkboxes',
      '#options' => $lifestyle_options,
      '#title' => 'Leefstijlprofiel',
      '#default_value' => $lifestyle_tags,
    );
  }

  $form['tags'] = array(
    '#type' => 'textfield',
    '#title' => 'Tags',
    '#default_value' => implode(';', $event_tags),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Opslaan',
  );

  return $form;

}

/**
 * Validate the tag form. No lifestyle profiles are allowed as a tag.
 */
function entry_api_ui_edit_tags_form_validate($form, &$form_state) {

  $tags = explode(';', $form_state['values']['tags']);
  foreach ($tags as $tag) {
    if (array_key_exists($tag, $form['lifestyle_profile']['#options'])) {
      form_set_error('tags', $tag . ' is niet toegelaten als tag.');
    }
  }

}

/**
 * Submit the edit tags form.
 */
function entry_api_ui_edit_tags_form_submit($form, &$form_state) {

  $tags = explode(';', $form_state['values']['tags']);
  foreach ($form_state['values']['lifestyle_profile'] as $profile) {
    if ($profile) {
      $tags[] = $profile;
    }
  }

  $event = new CultureFeed_Cdb_Event();
  $event->setExternalId($form['#object_id']);

  try {
    DrupalCultureFeed_EntryApi::addTagToEvent($event, $tags);

    drupal_set_message("De tags werden aangepast. Het duurt echter een half uur eer deze
      aanpassingen op al onze kanalen (inclusief UitinVlaanderen onder de tab 'Weergeven'
      beschikbaar zullen zijn.");
  }
  catch (Exception $e) {
    watchdog_exception(WATCHDOG_ERROR, $e);
    drupal_set_message('Er ging iets fout tijdens het bewaren.');
  }

}