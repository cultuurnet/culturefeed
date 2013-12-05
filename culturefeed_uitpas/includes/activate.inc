<?php
/**
 * @file
 * Helper functions for UiTPAS activation.
 */

/**
 * Page callback for activation of a UiTPAS.
 *
 * The activation link is sent to the pass holder when he/she provides
 * an e-mail address when getting a UiTPAS at a counter.
 *
 * @param string $uid
 *   User id
 * @param string $code
 *   Activation code
 */
function culturefeed_uitpas_activate($uid, $code) {
  $cf = DrupalCultureFeed::getConsumerInstance();

  $callback_options = array(
    'query' => array('destination' => 'registration_confirmation'),
    'absolute' => TRUE,
  );
  $callback_url = url('culturefeed/oauth/authorize', $callback_options);

  $token = $cf->getRequestToken($callback_url);

  $_SESSION['oauth_token'] = $token['oauth_token'];
  $_SESSION['oauth_token_secret'] = $token['oauth_token_secret'];

  $auth_url = $cf->getUrlAuthorize($token, $callback_url, CultureFeed::AUTHORIZE_TYPE_REGULAR, TRUE);

  $link = $cf->uitpas()->constructPassHolderActivationLink($uid, $code, $auth_url);

  drupal_goto($link);
}
