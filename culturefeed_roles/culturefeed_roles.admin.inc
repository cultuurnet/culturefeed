<?php
/**
 * @file
 */

/**
 * Foo form.
 */
function culturefeed_roles_settings_form($form, &$form_state) {
  $pre_assigned_roles = variable_get('culturefeed_roles', array());

  $form['culturefeed_roles'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );

  foreach (user_roles(TRUE) as $role_id => $role_name) {
    if ($role_id == DRUPAL_AUTHENTICATED_RID) {
      continue;
    }
    $form['culturefeed_roles'][$role_id] = array(
      '#title' => check_plain($role_name),
      '#type' => 'textarea',
      '#default_value' => $pre_assigned_roles[$role_id] ?: '',
      '#description' => t('The uid of the UiTID users that should get the role %role assigned the first time they log in. Enter one uid per line.', array('%role' => $role_name)),
    );
  }

  return system_settings_form($form);
}
