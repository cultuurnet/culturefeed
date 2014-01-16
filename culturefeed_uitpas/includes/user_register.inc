<?php

/**
 * @file
 * Helper functions for UiTPAS user register.
 */

/**
 * Returns user register form.
 */
function culturefeed_uitpas_user_register_get() {

  $uitpas_user = culturefeed_uitpas_get_uitpas_user();
  if (!user_is_logged_in() || !$uitpas_user->passholder) {

    return drupal_get_form('culturefeed_uitpas_user_register_form', $uitpas_user);

  }

}

/**
 * Form constructor for the UiTPAS user register form.
 */
function culturefeed_uitpas_user_register_form($form, $form_state, $uitpas_user) {

  form_load_include($form_state, 'inc', 'culturefeed_uitpas', 'includes/user_register');

  $form['prefix']['#markup'] = variable_get('culturefeed_uitpas_user_register_intro_text', t('Register here, so you can follow your UiTPAS advantages and points balance online.'));
  $form['link'] = array(
    '#type' => 'value',
  );
  $form['uitpasnumber'] = array(
    '#type' => 'textfield',
    '#title' => variable_get('culturefeed_uitpas_user_register_number_label', t('UiTPAS number')),
    '#required' => TRUE,
  );
  $form['dob'] = array(
    '#type' => 'date',
    '#title' => t('Date of birth'),
    '#after_build' => array('culturefeed_uitpas_user_register_form_process_year'),
    '#required' => TRUE,
  );
  $form['actions'] = array(
    'submit' => array(
      '#type' => 'submit',
      '#value' => variable_get('culturefeed_uitpas_user_register_submit_value', t('Continue')),
    ),
  );

  if ($_GET['q'] != 'register_uitpas' && !$uitpas_user->user) {

    $args = array('@url' => url('culturefeed/oauth/connect'));
    $form['suffix']['#markup'] = format_string(variable_get('culturefeed_uitpas_user_register_apply_text', t('Already registered ?  <a href="@url">Apply here</a>.')), $args);

  }

  return $form;

}

/**
 * Form validation handler for culturefeed_uitpas_user_register_form().
 */
function culturefeed_uitpas_user_register_form_validate(&$form, &$form_state) {

  $number = str_replace(' ', '', $form_state['values']['uitpasnumber']);
  $dob = $form_state['values']['dob']['year'] . '-' . $form_state['values']['dob']['month'] . '-' . $form_state['values']['dob']['day'];
  $dob = new DateTime($dob);

  $cf = DrupalCultureFeed::getConsumerInstance();
  if ($number) {

    try {

      $callback_options = array(
        'query' => array('isnewpassholder' => 'true'),
        'absolute' => TRUE,
      );
      $callback_url = url('culturefeed/oauth/authorize', $callback_options);

      $activation_data = new CultureFeed_Uitpas_Passholder_Query_ActivationData();
      $activation_data->uitpasNumber = $number;
      $activation_data->dob = $dob;

      $link = $cf->uitpas()->getPassholderActivationLink($activation_data, function () use ($cf, $callback_url) {

        $token = $cf->getRequestToken($callback_url);

        $_SESSION['oauth_token'] = $token['oauth_token'];
        $_SESSION['oauth_token_secret'] = $token['oauth_token_secret'];

        $auth_url = $cf->getUrlAuthorize($token, $callback_url, CultureFeed::AUTHORIZE_TYPE_REGULAR, TRUE);

        return $auth_url;

      });

      $form_state['values']['link'] = $link;

    }
    catch (CultureFeed_Exception $e) {

      switch ($e->error_code) {
        case 'INVALID_DOB':
          $error = variable_get('culturefeed_uitpas_user_register_validation_invalid_dob', t('Invalid date of birth.'));
          form_set_error('dob', $error);
          break;

        case 'PARSE_INVALID_UITPASNUMBER':
          $error = variable_get('culturefeed_uitpas_user_register_validation_parse_invalid_uitpasnumber', t('Invalid UiTPAS number.'));
          form_set_error('number', $error);
          break;

        case 'USER_ALREADY_ACTIVATED':
          $error = variable_get('culturefeed_uitpas_user_register_validation_user_already_activated', t('User already activated.'));
          form_set_error('number', $error);
          break;

        case 'MISSING_REQUIRED_FIELDS':
          $error = variable_get('culturefeed_uitpas_user_register_validation_user_missing_required_fields', t('Missing required fields.'));
          form_error($form, $error);
          break;

        case 'PARSE_INVALID_DATE':
          $error = variable_get('culturefeed_uitpas_user_register_validation_parse_invalid_date', t('Invalid date.'));
          form_set_error('dob', $error);
          break;

        case 'UNKNOWN_UITPASNUMBER':
          $error = variable_get('culturefeed_uitpas_user_register_validation_unknown_uitpasnumber', t('Unknown UiTPAS number.'));
          form_set_error('number', $error);
          break;

        case 'ACTION_FAILED':
          $error = variable_get('culturefeed_uitpas_user_register_validation_action_failed', t('Action failed.'));
          form_error($form, $error);
          break;

        default:
          form_error($form, t('We are unable to process your request at this moment. Please try again later.'));
          watchdog(__FUNCTION__, 'Unexpected error code when validating UiTPAS: @code', array('@code' => $e->error_code), WATCHDOG_WARNING);
      }
    }
    catch (Exception $e) {
      form_error($form, t('We are unable to process your request at this moment. Please try again later.'));
      watchdog_exception('culturefeed_uitpas_user_register_form_validate', $e);
    }
  }

}

/**
 * Form submission handler for culturefeed_uitpas_user_register_form().
 */
function culturefeed_uitpas_user_register_form_submit(&$form, &$form_state) {
  $form_state['redirect'] = $form_state['values']['link'];
}

/**
 * Modifies the year element of a date form element.
 */
function culturefeed_uitpas_user_register_form_process_year($element) {

  $year = date('Y');
  // 1900 is hardcoded in inludes/form.inc
  $element['year']['#options'] = array_slice($element['year']['#options'], 0, $year - 1900 + 1, TRUE);
  return $element;

}

/**
 * Returns form elements for user register settings.
 */
function culturefeed_uitpas_user_register_settings_get(&$form, $include_title = FALSE) {

  if ($include_title) {
    $form['culturefeed_uitpas_user_register_confirmation_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title for the registration page'),
      '#default_value' => variable_get('culturefeed_uitpas_user_register_confirmation_title', t('Already own an UiTPAS?'))
    );
  }

  $form['culturefeed_uitpas_user_register_intro_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Intro text'),
    '#default_value' => variable_get('culturefeed_uitpas_user_register_intro_text', t('Register here, so you can follow your UiTPAS advantages and points balance online.')),
  );
  $form['culturefeed_uitpas_user_register_number_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label for UiTPAS number field'),
    '#default_value' => variable_get('culturefeed_uitpas_user_register_number_label', t('UiTPAS number')),
  );
  $form['culturefeed_uitpas_user_register_submit_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Value for the submit button'),
    '#default_value' => variable_get('culturefeed_uitpas_user_register_submit_value', t('Continue')),
  );
  $form['culturefeed_uitpas_user_register_apply_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Apply text'),
    '#default_value' => variable_get('culturefeed_uitpas_user_register_apply_text', t('Already registered ?  <a href="@url">Apply here</a>.')),
  );

}

/**
 * Saves settings for user register.
 */
function culturefeed_uitpas_user_register_settings_save($values) {

  variable_set('culturefeed_uitpas_user_register_intro_text', $values['culturefeed_uitpas_user_register_intro_text']);
  variable_set('culturefeed_uitpas_user_register_number_label', $values['culturefeed_uitpas_user_register_number_label']);
  variable_set('culturefeed_uitpas_user_register_submit_value', $values['culturefeed_uitpas_user_register_submit_value']);
  variable_set('culturefeed_uitpas_user_register_apply_text', $values['culturefeed_uitpas_user_register_apply_text']);

}
