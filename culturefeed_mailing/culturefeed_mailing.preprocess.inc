<?php

/**
 * @file
 * Preprocess functions for culturefeed_mailing module.
 */

/**
 * Preprocess variables for culturefeed-mailing-optin.tpl.php
 *
 * @see culturefeed-mailing-optin.tpl.php
 */
function template_preprocess_culturefeed_mailing_optin(&$variables) {
  $variables['newsletter_optin_toggle'] = NULL;
  $optin = _culturefeed_mailing_optin_toggle();
  if ($optin) {
    $item_list = array(
      '#theme' => 'item_list',
      '#items' => array(
        array(
          'data' => $optin,
          'id' => 'newsletter-optin',
        ),
      )
    );
    $variables['newsletter_optin_toggle'] = drupal_render($item_list);
  }
}