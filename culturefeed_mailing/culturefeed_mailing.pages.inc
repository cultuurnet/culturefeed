<?php

/**
 * @file
 * Page callback functions for culturefeed_mailing module.
 */

/**
 * Page callback to toggle newsletter optin.
 */
function culturefeed_mailing_toggle_optin($mailing_id, $optin, $type = 'ajax') {

  $success = TRUE;

  try {

    $account = DrupalCultureFeed::getLoggedInUser();
    if (!$account) {
      return NULL;
    }

    if ($optin == '1') {
      culturefeed_mailing_subscribe_user($account, $mailing_id);
    }
    else {
      DrupalCultureFeed::unsubscribeFromMailing($account->id, $mailing_id);
    }

  }
  catch (CultureFeed_Exception $e) {

    // If user was already subscribed, don't show a message.
    if ($e->error_code != CultureFeed::CODE_MAILING_ALREADY_SUBSCRIBED) {
      watchdog_exception('culturefeed_mailing', $e);
      $success = FALSE;
    }

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_mailing', $e);
    $success = FALSE;
  }

  if ($type == 'ajax') {
    $commands = array();

    if ($success) {
      $data = _culturefeed_mailing_optin_toggle($mailing_id);
      $commands[] = ajax_command_html('#newsletter-optin-' . $mailing_id, $data);
    }

    ajax_deliver(array('#type' => 'ajax', '#commands' => $commands));
  }
  else {
    if (!$success) {
      drupal_set_message(t('Error occurred'), 'error');
    }
    drupal_goto();
  }
}

/**
 * Form callback for the mailing subscribe form.
 */
function culturefeed_mailing_subscribe_form() {

  $account = culturefeed_load_logged_in_user();

  // Redirect to loginpage if not logged in and light id permission is off.
  if (!$account && !CULTUREFEED_API_LIGHT_ID_ALLOWED) {
    drupal_goto('authenticated', array('query' => array('destination' => 'subscribe')));
  }

  // Show nothing if no mailing list available.
  if (!(variable_get('culturefeed_mailing_list', ''))) {
    return;
  }

  $form = array();

  $user_already_subscribed = $account ? _culturefeed_mailing_check_user_subscription($account->id, variable_get('culturefeed_mailing_list')) : FALSE;
  if ($user_already_subscribed) {

    $form['already_subscribed'] = array(
      '#theme' => 'culturefeed_mailing_content_block_already_subscribed',
      '#title' => NULL,
      '#message' => NULL,
    );
  }
  else {

    // Set defaults for the email field.
    if ($account) {
      $email = (!empty($_GET['email'])) ? $_GET['email'] : ($account ? $account->mbox : '');
      $disabled = TRUE;
      $description = '<span>' . t('Change your email address <a href="!url">here</a>.', array('!url' => url('culturefeed/account/edit'))) . '</span>';
    }
    else {
      $email = (!empty($_GET['email'])) ? $_GET['email'] : '';
      $disabled = FALSE;
      $description = '';
    }
    $form['mail'] = array(
      '#type' => 'textfield',
      '#title' => t('Email address'),
      '#required' => TRUE,
      '#size' => 40,
      '#maxlength' => 255,
      '#default_value' => $email,
      '#weight' => -10,
      '#disabled' => $disabled,
      '#description' => $description,
    );

    $show_zip = variable_get('culturefeed_mailing_block_show_zip', 0);

    $postal = '';
    if (!empty($_GET['zip'])) {
      $postal = $_GET['zip'];
    }
    elseif ($account) {
      $postal = $account->zip;
    }

    $form['zip'] = array(
      '#type' => 'textfield',
      '#title' => t('Zipcode'),
      '#required' => !CULTUREFEED_API_LIGHT_ID_ALLOWED,
      '#size' => 40,
      '#maxlength' => 255,
      '#default_value' => $postal,
      '#weight' => -9,
      '#access' => !CULTUREFEED_API_LIGHT_ID_ALLOWED || (CULTUREFEED_API_LIGHT_ID_ALLOWED && $show_zip),
      '#attributes' => array(
        'class' => array('zip-field'),
      ),
    );

    $form['firstname'] = array(
      '#type' => 'textfield',
      '#title' => t('First name'),
      '#required' => TRUE,
      '#size' => 40,
      '#maxlength' => 255,
      '#default_value' => ($account ? $account->givenName : ''),
      '#weight' => -8,
      '#access' => !CULTUREFEED_API_LIGHT_ID_ALLOWED,
    );

    $form['lastname'] = array(
      '#type' => 'textfield',
      '#title' => t('Family name'),
      '#required' => TRUE,
      '#size' => 40,
      '#maxlength' => 255,
      '#default_value' => ($account && isset($account->familyName) ? $account->familyName : ''),
      '#weight' => -7,
      '#access' => !CULTUREFEED_API_LIGHT_ID_ALLOWED,
    );

    $form['privacy'] = array(
      '#type' => 'markup',
      '#value' => '<div id="privacy">' . l(t('Privacy statement'), 'privacyverklaring') . '</div>',
      '#access' => !CULTUREFEED_API_LIGHT_ID_ALLOWED,
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Subscribe'),
      '#weight' => 10,
    );

    if (CULTUREFEED_API_LIGHT_ID_ALLOWED && $show_zip && empty($postal)) {
      // Attach jquery.cookie library.
      $form['#attached']['library'][] = array('system', 'jquery.cookie');

      // Attach scripts.
      $form['#attached']['js'][] = array(
        'data' => drupal_get_path('module', 'culturefeed_mailing') . '/js/culturefeed_mailing.js',
        'type' => 'file',
        'weight' => 0,
      );
    }
  }

  return $form;

}

/**
 * Submit the mailing subscription form.
 */
function culturefeed_mailing_subscribe_form_submit($form, &$form_state) {

  if (!empty($form_state['values']['zip'])) {
    $zip = $form_state['values']['zip'];
    _culturefeed_mailing_set_postal_to_cookie($form_state['values']['zip']);
  }
  else {
    $zip = '';
  }

  if (CULTUREFEED_API_LIGHT_ID_ALLOWED) {

    try {
      culturefeed_mailing_subscribe_user_light($form_state['values']['mail'], variable_get('culturefeed_mailing_list', ''), $zip);
    }
    catch (CultureFeed_Exception $e) {

      // If user was already subscribed, don't show a message.
      if ($e->error_code != CultureFeed::CODE_MAILING_ALREADY_SUBSCRIBED) {
        drupal_set_message(t('An error occurred while subscribing, please try again later.'), 'error');
        return;
      }

    }
    catch (Exception $e) {
      drupal_set_message(t('An error occurred while subscribing, please try again later.'), 'error');
      return;
    }

  }
  else {
    $account = DrupalCultureFeed::getLoggedInUser();

    // Subscribe to normal mailing.
    try {
      culturefeed_mailing_subscribe_user($account, variable_get('culturefeed_mailing_list', ''));
    }
    catch (CultureFeed_Exception $e) {

      // If user was already subscribed, don't show a message.
      if ($e->error_code != CultureFeed::CODE_MAILING_ALREADY_SUBSCRIBED) {
        drupal_set_message(t('An error occurred while subscribing, please try again later.'), 'error');
        return;
      }

    }
    catch (Exception $e) {
      drupal_set_message(t('An error occurred while subscribing, please try again later.'), 'error');
      return;
    }

    // Update user with entered fields.
    $fields = array('id', 'mbox', 'zip', 'firstname', 'lastname');

    $user_update = new CultureFeed_User();
    $user_update->id = $account->id;
    $user_update->mbox = $form_state['values']['mail'];
    $user_update->zip = $form_state['values']['zip'];
    $user_update->givenName = $form_state['values']['firstname'];
    $user_update->familyName = $form_state['values']['lastname'];

    try {
      DrupalCultureFeed::updateUser($user_update, $fields);
    }
    catch (Exception $e) {
      // Show no error here.
    }
  }

  $form_state['redirect'] = 'subscribe/subscribed';

}

/**
 * Form callback for the mailing subscription update form.
 */
function culturefeed_mailing_subscription_update_form() {

  $account = culturefeed_load_logged_in_user();

  // Redirect to loginpage if not logged in and light id permission is off.
  if (!$account) {
    drupal_goto('authenticated', array('query' => array('destination' => 'subscribe')));
  }

  // Show nothing if no mailing list available.
  if (!(variable_get('culturefeed_mailing_list', ''))) {
    return;
  }

  $form = array();

  $user_already_subscribed = $account ? _culturefeed_mailing_check_user_subscription($account->id, variable_get('culturefeed_mailing_list')) : FALSE;
  if (!$user_already_subscribed) {
    drupal_goto('subscribe');
  }
  else {

    $form['mail'] = [
      '#type' => 'textfield',
      '#title' => t('Email address'),
      '#required' => TRUE,
      '#size' => 40,
      '#maxlength' => 255,
      '#default_value' => ($account ? $account->mbox : ''),
      '#weight' => -10,
    ];

    $show_zip = variable_get('culturefeed_mailing_block_show_zip', 0);

    $form['zip'] = [
      '#type' => 'textfield',
      '#title' => t('Zipcode'),
      '#size' => 40,
      '#maxlength' => 255,
      '#default_value' => ($account ? $account->zip : ''),
      '#weight' => -9,
      '#access' => !CULTUREFEED_API_LIGHT_ID_ALLOWED || (CULTUREFEED_API_LIGHT_ID_ALLOWED && $show_zip),
      '#attributes' => [
        'class' => ['zip-field'],
      ],
    ];

    $form['firstname'] = array(
      '#type' => 'textfield',
      '#title' => t('First name'),
      '#required' => TRUE,
      '#size' => 40,
      '#maxlength' => 255,
      '#default_value' => ($account ? $account->givenName : ''),
      '#weight' => -8,
      '#access' => !CULTUREFEED_API_LIGHT_ID_ALLOWED,
    );

    $form['lastname'] = array(
      '#type' => 'textfield',
      '#title' => t('Family name'),
      '#required' => TRUE,
      '#size' => 40,
      '#maxlength' => 255,
      '#default_value' => ($account && isset($account->familyName) ? $account->familyName : ''),
      '#weight' => -7,
      '#access' => !CULTUREFEED_API_LIGHT_ID_ALLOWED,
    );

    $form['submit'] = [
      '#type' => 'submit',
      '#value' => t('Update'),
      '#weight' => 10,
    ];
  }

  return $form;

}

/**
 * Submit the mailing subscription uodate form.
 */
function culturefeed_mailing_subscription_update_form_submit($form, &$form_state) {

  $account = DrupalCultureFeed::getLoggedInUser();

  // Update user with entered fields.
  $fields = array('id', 'mbox', 'zip', 'firstname', 'lastname');

  $user_update = new CultureFeed_User();
  $user_update->id = $account->id;
  $user_update->mbox = $form_state['values']['mail'];
  $user_update->zip = $form_state['values']['zip'];
  $user_update->givenName = $form_state['values']['firstname'];
  $user_update->familyName = $form_state['values']['lastname'];

  try {
    DrupalCultureFeed::updateUser($user_update, $fields);
    drupal_set_message(t('You successfully updated your info.'));
  }
  catch (Exception $e) {
    form_set_error('update_error', t('Something went wrong while trying to update your info.'));
  }

}

/**
 * Page callback: Show a confirmation that the user is subscribed.
 */
function culturefeed_mailing_user_subscribed() {
  return variable_get('culturefeed_mailing_subscribed_message', t('You have successfully subscribed to our newsletter'));
}

/**
 * Show the overview of newsletters + the possiblity to toggle them.
 */
function culturefeed_mailing_page_my_newsletters() {

  $query = new CultureFeed_SearchMailingsQuery();
  $query->max = 999;
  $query->consumerKey = variable_get('culturefeed_api_application_key', '');
  $mailings = DrupalCultureFeed::searchMailings($query);

  $build = array();
  $build['intro'] = array(
    '#markup' => '',
  );

  if ($mailings->total > 0) {
    $build['mailings'] = array(
      '#theme' => 'culturefeed_mailing_user_profile_admin',
      '#mailings' => $mailings->objects,
    );
  }

  return $build;

}
