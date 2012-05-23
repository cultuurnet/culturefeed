<?php

/**
 * @file
 * Plugin to provide a content type for uitpas user profile account edit.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Uitpas: user profile account edit'),
  'description' => t('Uitpas: user profile account edit.'),
  'category' => t('Uitpas'),
  'required context' => new ctools_context_required(t('Uitpas user'), 'uitpas_user'),
);

function uitpas_ui_uitpas_user_profile_account_edit_content_type_edit_form($form, &$form_state) {
  return $form;
}

function uitpas_ui_uitpas_user_profile_account_edit_content_type_render($subtype, $conf, $args, $context) {
  if ($context->data) {
    $uitpas_user = $context->data->user;
    $cf = $context->data->cf;
    $block = new stdClass();
    $block->title = t('Edit account');
    $block->content = drupal_get_form('uitpas_ui_uitpas_user_profile_account_edit_content_type_render_form', $uitpas_user, $cf);
    return $block;
  }
}

function uitpas_ui_uitpas_user_profile_account_edit_content_type_render_form($form, &$form_state, $uitpas_user, $cf) {
  $form['uitpas_user'] = array(
    '#type' => 'value',
    '#value' => $uitpas_user,
  );
  $form['cf'] = array(
    '#type' => 'value',
    '#value' => $cf,
  );
  $form['username'] = array(
    '#type' => 'textfield',
    '#disabled' => TRUE,
    '#title' => t('Username'),
    '#default_value' => $uitpas_user->nick,
    '#required' => TRUE,
  );
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail address'),
    '#default_value' => $uitpas_user->mbox,
    '#required' => TRUE,
  );
  $form['actions'] = array(
    'save' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    )
  );
  //change password (taken from culturefeed_ui.pages.inc)
  $destination = url('myprofile/account_edit', array('absolute' => TRUE, 'query' => array('closepopup' => 'true')));
  $url = DrupalCultureFeed::getUrlChangePassword($uitpas_user->id, $destination);
  $options = array('attributes' => array('class' => array('culturefeedconnect')), 'query' => drupal_get_destination());
  $form['change_password'] = array(
    '#prefix' => '<div id="change-password">',
    '#markup' => l('Paswoord wijzigen', $url, $options),
    '#suffix' => '</div>',
  );
  return $form;
}

function uitpas_ui_uitpas_user_profile_account_edit_content_type_render_form_validate(&$form, &$form_state) {
  if (!valid_email_address($form_state['values']['email'])) {
    form_set_error('email', t('Invalid e-mail address'));
  }
}

function uitpas_ui_uitpas_user_profile_account_edit_content_type_render_form_submit(&$form, &$form_state) {
  $uitpas_user = new CultureFeed_User();
  $uitpas_user->id = $form_state['values']['uitpas_user']->id;
  $uitpas_user->nick = $form_state['values']['username'];
  $uitpas_user->mbox = $form_state['values']['email'];
  $cf = $form_state['values']['cf'];
  try {
    $cf->updateUser($uitpas_user);
    drupal_set_message(t('Your changes have been saved.'));
  }
  catch (Exception $e) {
    watchdog_exception('uitpas_ui_content_type_user_profile_account_edit', $e);
    drupal_set_message(t('An error occured.'), 'error');
  }
}