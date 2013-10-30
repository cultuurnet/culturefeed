<?php

/**
 * @file
 * Form functions for Culturefeed uitpas.
 */

/**
 * Form constructor for the uitpas user register form.
 */
function culturefeed_uitpas_user_register_form($form, $form_state, $uitpas_user) {

  $form['#prefix'] = variable_get('culturefeed_uitpas_user_register_intro');
  $form['link'] = array(
    '#type' => 'value',
  );
  $form['uitpasnumber'] = array(
    '#type' => 'textfield',
    '#title' => t('UITPAS number'),
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
      '#value' => t('Continue'),
    ),
  );

  if ($_GET['q'] != 'register_uitpas' && !$uitpas_user->user) {

    $args = array('@url' => url('culturefeed/oauth/connect'));
    $form['#suffix'] = t('Already registered ?  <a href="@url">Apply here</a>.', $args);

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
        'query' => array('closepopup' => 'true', 'isnewpassholder' => 'true'),
        'absolute' => TRUE,
      );
      $callback_url = url('culturefeed/oauth/authorize', $callback_options);

      $link = $cf->uitpas()->getPassholderActivationLink($number, $dob, function () use ($cf, $callback_url) {

        $token = $cf->getRequestToken($callback_url);

        $_SESSION['oauth_token'] = $token['oauth_token'];
        $_SESSION['oauth_token_secret'] = $token['oauth_token_secret'];

        $auth_url = $cf->getUrlAuthorize($token, $callback_url, CultureFeed::AUTHORIZE_TYPE_REGULAR, TRUE);

        return $auth_url;

      });

      $form_state['values']['link'] = $link;

    }
    catch (Exception $e) {

      if ($e->error_code == 'INVALID_DOB') {

        $error = variable_get('culturefeed_uitpas_user_register_validation_invalid_dob', t('Invalid date of birth.'));
        form_set_error('dob', $error);

      }

      if ($e->error_code == 'PARSE_INVALID_UITPASNUMBER') {

        $error = variable_get('culturefeed_uitpas_user_register_validation_parse_invalid_uitpasnumber', t('Invalid uitpas number.'));
        form_set_error('number', $error);

      }

      if ($e->error_code == 'USER_ALREADY_ACTIVATED') {

        $error = variable_get('culturefeed_uitpas_user_register_validation_user_already_activated', t('User already activated.'));
        form_set_error('number', $error);

      }

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
