<?php

/**
 * @file
 * Helper functions for culturefeed mailing.
 */

/**
 * Helper function to build mailing optin toggle
 */
function _culturefeed_mailing_optin_toggle() {

  try {

    $account = DrupalCultureFeed::getLoggedInUser();
    if (!$account) {
      return NULL;
    }

    $mailing_id = variable_get('culturefeed_mailing_list', '');
    $subscribed = _culturefeed_mailing_check_user_subscription($account->id, $mailing_id);

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_mailing', $e);
    return;
  }

  $subscribed_class = $subscribed ? 'ingeschreven' : 'niet-ingeschreven';
  $publish_link = array(
    '#type' => 'link',
    '#title' => $subscribed ? 'Ingeschreven' : 'Niet ingeschreven',
    '#href' => 'culturefeed/toggle/newsletter-optin/' . ($subscribed ? '0' : '1') . '/nojs',
    '#attributes' => array('id' => 'newsletter-optin-link', 'class' => array('newsletter-optin-link', $subscribed_class)),
    '#options' => array('query' => drupal_get_destination()),
    '#ajax' => array(),
  );

  return drupal_render($publish_link) . ' ' . 'Ik wil een tweewekelijkse e-mail met nieuwe aanbevelingen ontvangen';

}


/**
 * Helper function to check if user is subscribed to mailing list
 */
function _culturefeed_mailing_check_user_subscription($user_id, $mailing_id) {
  try {
    $subscriptions = DrupalCultureFeed::getMailingSubscriptions($user_id)->objects;
    $newsletters = array();
    foreach ($subscriptions as $subscription) {
      $newsletters[$subscription->id] = $subscription->id;
    }
    return in_array($mailing_id, $newsletters);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_mailing', $e);
    return FALSE;
  }
}