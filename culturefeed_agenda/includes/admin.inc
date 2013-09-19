<?php
/**
 * @file
 * Admin settings for culturefeed_agenda.
 */

/**
 * General settings for agenda.
 */
function culturefeed_agenda_admin_settings_form() {

  $page_info = culturefeed_agenda_culturefeed_search_page_info();
  $agenda_page = current($page_info);

  $form = array();
  $form['culturefeed-agenda-default-sort'] = array(
    '#title' => t('Default sorting for agenda'),
    '#type' => 'select',
    '#options' => $agenda_page['sort_options'],
    '#default_value' => $agenda_page['sort_default'],
  );

  return system_settings_form($form);
}

