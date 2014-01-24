<?php

/**
 * @file
 * Theming functions for culturefeed.
 */

/**
 * Theme the authentication required page.
 */
function theme_culturefeed_authenticated_page() {

  $login_facebook = l(t('Login with Facebook'), 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect connect-facebook')), 'query' => array('via' => 'facebook')));
  $login_twitter = l('Twitter', 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect connect-twitter')), 'query' => array('via' => 'twitter')));
  $login_google = l('Google', 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect connect-google')), 'query' => array('via' => 'google')));
  $login_email = l(t('e-mail'), 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect connect-email'))));
  $register = l(t('new account'), 'culturefeed/oauth/connect/register', array('attributes' => array('class' => array('culturefeedconnect'))));

  $output = '';

  $output .= '<p>' . t('To continue you have to be logged on with your UiTiD.') . '</p>';
  $output .= $login_facebook;
  $output .= '<p>' . t('Or via') . $login_twitter . ', ' . $login_google . ' ' . t('or') . ' ' . $login_email . '</p>';
  $output .= '<p class="small quiet">' . t("Don't have an UiTiD? Create a !new_account.", array('!new_account' => $register)) . '</p>';

  return $output;
}
