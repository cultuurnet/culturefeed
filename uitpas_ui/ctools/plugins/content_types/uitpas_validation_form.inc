<?php

/**
 * @file
 * Plugin to provide a content type for uitpas validation form.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Uitpas: validation form'),
  'description' => t('Uitpas: validation form.'),
  'category' => t('Uitpas'),
  'required context' => new ctools_context_required(t('Uitpas user'), 'uitpas_user'),
);

function uitpas_ui_uitpas_validation_form_content_type_edit_form($form, &$form_state) {
  return $form;
}

function uitpas_ui_uitpas_validation_form_content_type_render($subtype, $conf, $args, $context) {
  if (!user_is_logged_in() || ($context->data && !$context->data->passholder)) {
    $block = new stdClass();
    $block->title = t('Allready own an Uitpas ?');
    $block->content = drupal_get_form('uitpas_ui_uitpas_validation_form_content_type_render_form');
    return $block;
  }
}

function uitpas_ui_uitpas_validation_form_content_type_render_form($form, $form_state) {
  $form['#prefix'] = variable_format_value('uitpas_ui_validation_intro');
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
    '#after_build' => array('uitpas_ui_uitpas_validation_form_process_year'),
    '#required' => TRUE,
  );
  $form['actions'] = array(
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Continue'),
    ),
  );
  if ($_GET['q'] != 'register_uitpas' && !user_is_logged_in()) {
    $args = array('@url' => url('culturefeed/oauth/connect'));
    $form['#suffix'] = t('Allready registered ?  <a href="@url">Apply here</a>.', $args);
  }
  return $form;
}

function uitpas_ui_uitpas_validation_form_content_type_render_form_validate(&$form, &$form_state) {
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
        $error = variable_get_value('uitpas_ui_validation_invalid_dob_error_message');
        form_set_error('dob', $error);
      }
      if ($e->error_code == 'PARSE_INVALID_UITPASNUMBER') {
        $error = variable_get_value('uitpas_ui_validation_parse_invalid_uitpasnumber_error_message');
        form_set_error('number', $error);
      }
      if ($e->error_code == 'USER_ALREADY_ACTIVATED') {
        $error = variable_get_value('uitpas_ui_validation_user_already_activated_error_message');
        form_set_error('number', $error);
      }
    }
  }
}

function uitpas_ui_uitpas_validation_form_content_type_render_form_submit(&$form, &$form_state) {
  $form_state['redirect'] = $form_state['values']['link'];
}

function uitpas_ui_uitpas_validation_form_process_year($element) {
  $year = date('Y');
  //1900 is hardcoded in inludes/form.inc
  $element['year']['#options'] = array_slice($element['year']['#options'], 0 , $year - 1900 + 1, TRUE);
  return $element;  
}