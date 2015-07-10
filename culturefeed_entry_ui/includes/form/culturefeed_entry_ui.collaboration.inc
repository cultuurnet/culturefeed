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
function culturefeed_entry_ui_attach_collaboration_element(array &$form, array $form_state, array $info) {

  $keyword = $info['keyword'];

  $form['collaboration'][$keyword] = array(
    '#group' => 'vertical_tabs',
    '#title' => $keyword,
    '#type' => 'fieldset',
  );

  // Title.
  $form['collaboration'][$keyword]['title'] = array(
    '#default_value' => isset($info['title']) ? $info['title'] : '',
    '#required' => TRUE,
    '#title' => t('Title'),
    '#type' => 'textfield',
  );

  // Image.
  $default_image = '';
  if (isset($info['description']['image']) && $info['description']['image']) {

    $default_image = culturefeed_entry_ui_collaboration_get_image_from_url($info['description']['image']);

    // @codingStandardsIgnoreStart
    // $default_image = system_retrieve_file($info['description']['image'], 'public://uploads/', TRUE);
    // @codingStandardsIgnoreEnd

  }

  $form['collaboration'][$keyword]['image'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Image'),
  );

  $form['collaboration'][$keyword]['image']['upload'] = array(
    '#type' => 'managed_file',
    '#name' => 'image_upload',
    '#title' => t('Image upload'),
    '#default_image' => isset($default_image->fid) ? $default_image : FALSE,
    '#default_value' => isset($default_image->fid) ? $default_image->fid : FALSE,
    '#description' => t('Allowed extensions: gif png jpg jpeg'),
    '#upload_location' => 'public://uploads/',
    '#upload_validators' => array(
      'file_validate_extensions' => array('gif png jpg jpeg'),
      // Pass the maximum file size in bytes.
      'file_validate_size' => array(1024 * 1024 * 1024),
    ),
    '#after_build' => array('culturefeed_entry_ui_photo_after_build'),
  );

  if (isset($default_image->fid)) {
    $form['collaboration'][$keyword]['image']['original_file'] = array(
      '#type' => 'hidden',
      '#value' => $default_image->fid,
    );
  }

  $form['collaboration'][$keyword]['image']['copyright_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Copyright'),
    '#description' => t('Name of the copyright owner of this image.'),
    '#default_value' => isset($info['copyright']) ? $info['copyright'] : '',
    '#size' => 30,
  );

  $form['collaboration'][$keyword]['image']['copyright'] = array(
    '#type' => 'checkbox',
    '#default_value' => isset($default_image->fid) ? 1 : 0,
    '#title' => t('I agree to the <a href="http://www.uitdatabank.be/uitportal/legal.aspx">terms & conditions</a> and I declare that I have the necessary rights or permissions to distribute the image.'),
  );

  // Article.
  $form['collaboration'][$keyword]['article'] = array(
    '#default_value' => isset($info['description']['article']) ? $info['description']['article'] : '',
    '#title' => t('Article'),
    '#type' => 'textfield',
  );

  // Text.
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
function culturefeed_entry_ui_edit_collaboration_data_form(array $form, array &$form_state, \CultuurNet\Search\ActivityStatsExtendedEntity $event) {

  /* @var \CultuurNet\Search\ActivityStatsExtendedEntity $event */
  $event = $event->getEntity();
  /* @var \CultureFeed_Cdb_Item_Event $event */
  $form['#event'] = $event;
  $form['#attributes'] = array(
    'class' => array('edit-collaboration-data'),
  );

  $custom_tags = variable_get('culturefeed_entry_ui_custom_collaboration_tags', array());
  if (!count($custom_tags)) {
    $tag = variable_get('site_name') . ' selectie';
    $custom_tags[$tag] = array('value' => $tag, 'visible' => FALSE);
    variable_set('culturefeed_entry_ui_custom_collaboration_tags', $custom_tags);
  }

  // Start vertical tabs.
  $form['vertical_tabs'] = array(
    '#type' => 'vertical_tabs',
  );

  $form['collaboration'] = array(
    '#tree' => TRUE,
    '#type' => 'container',
  );

  form_load_include($form_state, 'inc', 'culturefeed_entry_ui', 'includes/helpers/culturefeed_entry_ui.collaboration');
  form_load_include($form_state, 'inc', 'culturefeed_entry_ui', 'includes/culturefeed_entry_ui.pages');

  foreach ($custom_tags as $tag) {

    $info = culturefeed_entry_ui_collaboration_get_data_by_keyword($event, $tag['value']);
    culturefeed_entry_ui_attach_collaboration_element($form, $form_state, $info);
  }

  $form['#attached'] = array(
    'css' => array(
      drupal_get_path('module', 'culturefeed_entry_ui') . '/css/culturefeed_entry_ui.css',
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
function culturefeed_entry_ui_edit_collaboration_data_form_submit(array &$form, array &$form_state) {

  $consumer_key = variable_get('culturefeed_api_application_key', '');
  /* @var \CultureFeed_Cdb_Item_Event $event */
  $event = $form['#event'];

  if ($consumer_key && $event && isset($form_state['values']['collaboration'])) {

    foreach ($form_state['values']['collaboration'] as $keyword => $collaboration) {

      $title = $collaboration['title'];
      $copyright = isset($collaboration['image']['copyright_text']) ? $collaboration['image']['copyright_text'] : '';
      $plain_text = $collaboration['text'];
      $image_file = '';
      $image = '';

      if ($collaboration['image']['upload']) {

        $image_file = file_load($collaboration['image']['upload']);
        $image_file->status = FILE_STATUS_PERMANENT;
        file_save($image_file);
        file_usage_add($image_file, 'culturefeed_entry_ui', 'event', $image_file->fid);
        $image = file_create_url($image_file->uri);

      }

      $description = drupal_json_encode(array(
        'keyword' => $keyword,
        'text' => $plain_text,
        'image' => $image,
        'article' => $collaboration['article'],
      ));

      /** @var CultureFeed_EntryApi $entryApi */
      $entryApi = Drupalculturefeed_EntryApi::getLoggedInUserInstance();

      $entryApi->addCollaborationLinkToEvent(
        $event,
        culturefeed_entry_ui_get_preferred_language(),
        $plain_text,
        $title,
        $copyright,
        $consumer_key,
        $description
      );

      // @codingStandardsIgnoreStart
      // Remove the file(s).
      /*if ($image_file) {
        file_delete($image_file);
      }
      if (isset($collaboration['image']['original_file'])) {
        $file = file_load($collaboration['image']['original_file']);
        file_delete($file);
      }*/
      // @codingStandardsIgnoreEnd

      // Also save the keyword directly as keyword.
      $keywords = array(
        $keyword => new CultureFeed_Cdb_Data_Keyword($keyword, FALSE),
      );
      $entryApi->addTagToEvent($event, $keywords);

    }

  }

}

/**
 * Validation handler for culturefeed_entry_ui_edit_collaboration_data_form().
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 */
function culturefeed_entry_ui_edit_collaboration_data_form_validate(array $form, array &$form_state) {

  if (isset($form_state['values']['collaboration'])) {

    $collaboration = $form_state['values']['collaboration'];
    foreach ($collaboration as $tag => $info) {

      $field = "collaboration][" . $tag . "][image][";
      if (!empty($info['image']['upload']) && $info['image']['copyright'] != 1) {
        form_set_error($field . "copyright", t('Please agree to the general conditions of UiTdatabank and declare that you have the necessary rights or permissions to distribute the image through UiTdatabank.'));
      }
      if (!empty($info['image']['upload']) && empty($info['image']['copyright_text'])) {
        form_set_error($field . "copyright_text", t('Copyright field is required.'));
      }

    }

  }

}
