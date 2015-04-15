<?php

/**
 * @file
 * Contains culturefeed_entry_ui.collaboration.inc.
 */

/**
 * Attaches the collaboration elements to the form.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 * @param Culturefeed_Cdb_Data_EventDetail $event_details
 *   The event details.
 */
function culturefeed_entry_ui_attach_collaboration_elements(&$form, $form_state, Culturefeed_Cdb_Data_EventDetail $event_details = NULL) {

  if ($event_details) {

    $media_links = $event_details->getMedia()->byMediaTypes(array(CultureFeed_Cdb_Data_File::MEDIA_TYPE_WEBRESOURCE));
    foreach ($media_links as $link) {

      if ($link->getSubBrand()) {

        $sub_brand = $link->getSubBrand();
        $description = drupal_json_decode($link->getDescription());

        if ($description['keyword']) {
          culturefeed_entry_ui_attach_collaboration_element($form, $form_state, $sub_brand, $description);
        }

      }

    }

  }

  // Add an empty element for new data.
  culturefeed_entry_ui_attach_collaboration_element($form, $form_state);

}

/**
 * Attaches the collaboration element to the form.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 * @param string $sub_brand
 *   The collaborative sub brand.
 * @param array $description
 *   The collaborative description.
 */
function culturefeed_entry_ui_attach_collaboration_element(&$form, $form_state, $sub_brand = NULL, $description = NULL) {

  $sub_brand = ($sub_brand) ? $sub_brand : 'new_collaboration';

  $form['extra'][$sub_brand] = array(
    '#group' => 'vertical_tabs',
    '#title' => isset($description['keyword']) ? $description['keyword'] : t('New collaboration data'),
    '#type' => 'fieldset',
  );

  $form['extra'][$sub_brand]['sub_brand'] = array(
    '#type' => 'value',
    '#value' => $sub_brand,
  );

  $form['extra'][$sub_brand]['keyword'] = array(
    '#default_value' => isset($description['keyword']) ? $description['keyword'] : '',
    '#title' => t('Keyword'),
    '#type' => 'textfield',
  );

  $form['extra'][$sub_brand]['title'] = array(
    '#default_value' => isset($description['title']) ? $description['title'] : '',
    '#title' => t('Title'),
    '#type' => 'textfield',
  );

  $form['extra'][$sub_brand]['text'] = array(
    '#default_value' => isset($description['text']) ? $description['text'] : '',
    '#title' => t('Short description'),
    '#type' => 'textarea',
  );

}
