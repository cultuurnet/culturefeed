<?php
/**
 * @file
 */

/**
 * Implements hook_drush_command().
 */
function culturefeed_roles_drush_command() {
  $items['culturefeed-pre-assign-role'] = array(
    'description' => 'Pre-assign roles to UiTID users.',
    'arguments' => array(
      'role' => 'The role RID or name',
      'uid' => 'The remote id of the UiTID user',
    ),
    'required-arguments' => 2,
  );

  return $items;
}

function drush_culturefeed_roles_culturefeed_pre_assign_role($role_id, $uid) {
  $role = NULL;
  if (ctype_digit($role_id)) {
    $role = user_role_load($role_id);
  }

  if (!$role) {
    $role = user_role_load_by_name($role_id);
  }

  if (!$role) {
    return drush_set_error('', 'Undefined role');
  }

  $pre_assigned_roles = variable_get('culturefeed_roles', array());

  if (isset($pre_assigned_roles[$role->rid])) {
    $pre_assigned_roles[$role->rid] .= "\r" . $uid;
  }
  else {
    $pre_assigned_roles[$role->rid] = $uid;
  }

  variable_set('culturefeed_roles', $pre_assigned_roles);
}
