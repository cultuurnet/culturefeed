<?php

/**
 * @file
 * Preprocess functions for culturefeed_mailing module.
 */

/**
 * Theme the mailing optin for 1 item.
 */
function theme_culturefeed_mailing_optin($variables) {
  $variables['newsletter_optin_toggle'] = NULL;
  $optin = _culturefeed_mailing_optin_toggle($variables['mailing_id']);
  if ($optin) {
    $item_list = array(
      '#theme' => 'item_list',
      '#items' => array(
        array(
          'data' => $optin,
          'id' => 'newsletter-optin-' . $variables['mailing_id'],
        ),
      ),
    );
    return drupal_render($item_list);
  }
}

/**
 * Preprocess variables for the overview of possible mailings.
 *
 * @see culturefeed-mailing-user-profile-admin.tpl.php
 */
function culturefeed_mailing_preprocess_culturefeed_mailing_user_profile_admin(&$variables) {

  $items = array();
  foreach ($variables['mailings'] as $mailing) {
    $items[] = theme('culturefeed_mailing_user_profile_admin_item', array('item' => $mailing));
  }

  $variables['items'] = $items;

}

/**
 * Preprocess variables for the overview of 1 possible mailing item.
 *
 * @see culturefeed-mailing-user-profile-admin-item.tpl.php
 */
function culturefeed_mailing_preprocess_culturefeed_mailing_user_profile_admin_item(&$variables) {

  $item = $variables['item'];

  if (!empty($item->template)) {
    $variables['description'] = $item->template->subject;
    $variables['frequency'] = t(strtolower($item->template->frequency));
  }

  $variables['title'] = $item->name;
  $variables['optin_toggle'] = theme('culturefeed_mailing_optin', array('mailing_id' => $item->id));
  $variables['description'] = $item->description;
}

/**
 * Preprocess variables for subscription thank you message.
 *
 * @see culturefeed-mailing-content-block-subscribed.tpl.php
 */
function culturefeed_mailing_preprocess_culturefeed_mailing_content_block_subscribed(&$variables) {

}

/**
 * Preprocess variables for already subscribed message.
 *
 * @see culturefeed-mailing-content-block-already-subscribed.tpl.php
 */
function culturefeed_mailing_preprocess_culturefeed_mailing_content_block_already_subscribed(&$variables) {

  $variables['message'] = variable_get('culturefeed_mailing_already_subscribed_message', t('You are already subscribed to our newsletter'));
  $variables['update_link'] = l(t('Update subscription'), 'subscribe/update');
  $variables['unsubscribe_link'] = l(t('Manage subscriptions'), 'culturefeed/mailing');

}
