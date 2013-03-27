<?php
/**
 * @file
 * Theme / preprocess functions for culturefeed agenda.
 */

/**
 * Preprocess the variables for the event summary.
 * @see culturefeed-event-summary.tpl.php
 */
function template_preprocess_culturefeed_event_summary(&$variables) {

  _template_preprocess_culturefeed_agenda($variables);

}

/**
 * Preprocess the variables for the event detail.
 * @see culturefeed-event.tpl.php
 */
function template_preprocess_culturefeed_event(&$variables) {

  _template_preprocess_culturefeed_agenda($variables);

}

/**
 * Helper preprocess function to preprocess the general agenda variables.
 */
function _template_preprocess_culturefeed_agenda(&$variables) {

  global $language;

  $item = $variables['item'];

  $cdb_item = $item->getEntity();
  $details = $cdb_item->getDetails()->getDetailByLanguage($language->language);

  $variables['title'] = $details->getTitle();

}