<?php

/**
 * @file
 * Theming functions for culturefeed.
 */

/**
 * Implements hook_preprocess_culturefeed_authenticated_page().
 * @see culturefeed-authentication-page.tpl.php
 */
function culturefeed_preprocess_culturefeed_authenticated_page(&$variables) {

  $cf_query = drupal_get_query_parameters($_GET, array('q'));

  $cf_query['via'] = 'facebook';
  $variables['login_facebook'] = l(t('Login with Facebook'), 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect connect-facebook')), 'query' => $cf_query));

  $cf_query['via'] = 'twitter';
  $variables['login_twitter'] = l('Twitter', 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect connect-twitter'), 'rel' => 'nofollow'), 'query' => $cf_query));

  $cf_query['via'] = 'google';
  $variables['login_google'] = l('Google', 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect connect-google'), 'rel' => 'nofollow'), 'query' => $cf_query));

  unset($cf_query['via']);
  unset($cf_query['via']);
  $variables['login_email'] = l(t('e-mail'), 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect connect-email'), 'rel' => 'nofollow'), 'query' => $cf_query));
  $variables['register'] = l(t('new account'), 'culturefeed/oauth/connect/register', array('attributes' => array('class' => array('culturefeedconnect'), 'rel' => 'nofollow'), 'query' => $cf_query));

  $variables['login_message'] = t('To continue you have to be logged on with your UiTiD.');

}
