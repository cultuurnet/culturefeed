<?php

/**
 * @file
 * Helper functions for culturefeed mailing.
 */

/**
 * Helper function to build mailing optin toggle.
 */
function _culturefeed_mailing_optin_toggle($mailing_id) {

  try {

    $account = DrupalCultureFeed::getLoggedInUser();
    if (!$account) {
      return NULL;
    }

    $subscribed = _culturefeed_mailing_check_user_subscription($account->id, $mailing_id);

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_mailing', $e);
    return;
  }

  $subscribed_class = $subscribed ? t('subscribed') : t('not subscribed');
  $publish_link = array(
    '#type' => 'link',
    '#title' => $subscribed ? t('Unsubscribe') : t('Subscribe'),
    '#href' => 'culturefeed/toggle/newsletter-optin/' . $mailing_id . '/' . ($subscribed ? '0' : '1') . '/nojs',
    '#attributes' => array(
      'id' => 'newsletter-optin-link-' . $mailing_id,
      'class' => array('btn btn-primary newsletter-optin-link', $subscribed_class),
    ),
    '#options' => array('query' => drupal_get_destination()),
    '#ajax' => array(),
  );

  return drupal_render($publish_link);

}


/**
 * Helper function to check if user is subscribed to mailing list.
 */
function _culturefeed_mailing_check_user_subscription($user_id, $mailing_id, $use_auth = TRUE) {
  try {
    $subscriptions = DrupalCultureFeed::getMailingSubscriptions($user_id, $use_auth)->objects;
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

/**
 * Helper function to retrieve the postal code from cookie.
 */
function _culturefeed_mailing_get_postal_from_cookie() {
  if (!empty($_COOKIE['Drupal_visitor_uitid_userLocation'])) {
    $user_location = json_decode($_COOKIE['Drupal_visitor_uitid_userLocation']);
    if ($user_location instanceof stdClass) {
      $postal = $user_location->postal;
      return $postal;
    }
  }
  return '';
}

/**
 * Helper function to retrieve the postal code from cookie.
 */
function _culturefeed_mailing_set_postal_to_cookie($postal) {

  if (!empty($_COOKIE['Drupal_visitor_uitid_userLocation'])) {
    $user_location = json_decode($_COOKIE['Drupal_visitor_uitid_userLocation']);
    if (!$user_location instanceof stdClass) {
      $user_location = new stdClass();
    }
  }
  else {
    $user_location = new stdClass();
  }

  $user_location->postal = $postal;
  user_cookie_save(array('uitid.userLocation' => json_encode($user_location)));
}
