<?php
/**
 * @file
 * Page callbacks for member management.
 */

/**
 * Show the manage members page for 1 culturefeed page.
 */
function culturefeed_pages_page_manage_members(CultureFeed_Cdb_Item_Page $page, $request_type = 'nojs', $cf_user = NULL) {

  try {

    $cf_pages = DrupalCultureFeed::getLoggedInUserInstance()->pages();
    $user_list = $cf_pages->getUserList($page->getId(),  array(CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN, CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_MEMBER));

    $list = culturefeed_pages_manage_members_list($page, $user_list, $cf_user);

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
  }

  if ($request_type == 'ajax') {

    $build = array('page' => $list);
    $commands = array(
      ajax_command_html('#manage-members', render($build)),
    );

    ajax_deliver(array('#type' => 'ajax', '#commands' => $commands));
    return;

  }

  // Non-ajax pages should have more data.
  $build['view_page'] = array(
    '#markup' => '<div id="view-page">' . culturefeed_search_detail_l('page', $page->getId(), $page->getName(), t('View page')) . '</div>'
  );
  $build['page'] = $list;
  $build['search_form'] = drupal_get_form('culturefeed_pages_search_user_form');

  if (isset($_GET['search'])) {
    if (strlen($_GET['search']) >= 3) {
      $build['search_result'] = culturefeed_pages_user_search_result($_GET['search'], $page, $user_list);
    }
    else {
      drupal_set_message(t('Please enter at least 3 characters'), 'error');
    }
  }

  culturefeed_pages_set_page_breadcrumb($page);

  return $build;

}

/**
 * Show the list of members from a page.
 */
function culturefeed_pages_manage_members_list(CultureFeed_Cdb_Item_Page $page, CultureFeed_Pages_UserList $user_list, $cf_user = NULL) {

  // Get all the uid's in 1 time. Otherwise the theming layer will search it 1 by 1.
  culturefeed_get_uids_for_memberships($user_list->memberships);

  $header = array(
    t('Name'),
    t('Function'),
    t('Role'),
    t('Member since'),
    '',
    '',
  );

  // Count how many admins.
  $admins = array();
  foreach ($user_list->memberships as $member) {
    if ($member->role == CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN) {
      $admins[] = $member;
    }
  }
  $total_admins = count($admins);

  $rows = array();
  // Create row for every member.
  foreach ($user_list->memberships as $member) {

    $row = array();

    $name = '';
    $depiction = !empty($member->user->depiction) ? $member->user->depiction : 'http://media.uitid.be/fis/rest/download/ce126667652776f0e9e55160f12f5478/uiv/default.png';
    $name = '<span class="depiction">' . theme('image', array('path' => $depiction . '?width=30&height=30&crop=auto')) . '</span>';
    $drupal_uid = culturefeed_get_uid_for_cf_uid($member->user->id, $member->user->nick);
    $name .= l($member->user->nick, 'user/' . $drupal_uid);

    $row['name'] = $name;

    // Show the user data.
    if (empty($cf_user) || $cf_user->id != $member->user->id) {

      $row['function'] = $member->relation;
      switch ($member->role) {
        case CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN:
          $role = t('administrator');
        break;
        case CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_MEMBER:
        default:
          $role = t('member');
        break;
      }
      $row['role'] = $role;
      $row['member_since'] = date('d/m/Y H:i', $member->creationDate);
      $row['edit'] = l(t('Edit'), 'pages/' . $page->getId() . '/membership/' . $member->user->id . '/edit/nojs', array('attributes' => array('class' => 'use-ajax')));

      if ($total_admins == 1 && $member->role == CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN) {
        $row['delete'] = theme('culturefeed_pages_membership_delete_not_possible', array('page' => $page));
      }
      else {
        $delete_options = array(
          'attributes' => array(
            'role' => 'button',
            'data-toggle' => 'modal',
            'data-target' => '#page_confirm',
            'data-remote' => url('pages/' . $page->getId() . '/membership/' . $member->user->id . '/delete/ajax'),
          ),
        );
        $row['delete'] = l(t('Remove as member'), 'pages/' . $page->getId() . '/membership/' . $member->user->id . '/delete/nojs', $delete_options);
      }
    }
    // Show the edit form.
    else {

      $form = drupal_get_form('culturefeed_pages_edit_membership_form', $page, $cf_user, $user_list);
      $row['function'] = array(
        'data' => render($form),
        'colspan' => 2,
      );
      $row['member_since'] = date('d/m/Y H:i', $member->creationDate);
      $row['cancel'] = array(
        'data' => l(t('Cancel changes'), 'pages/' . $page->getId() . '/members/nojs', array('attributes' => array('class' => 'use-ajax'))),
        'colspan' => 2,
      );

    }

    $rows[] = array('data' => $row, 'id' => 'member-' . $member->user->id);

  }

  return array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No content available.'),
    '#attached' => array('library' => array(array('system', 'drupal.ajax'))),
    '#prefix' => '<div id="manage-members">',
    '#suffix' => '</div>',
  );

}

/**
 * Show the edit form for a membership.
 */
function culturefeed_pages_edit_membership_form($form, &$form_state, CultureFeed_Cdb_Item_Page $page, CultureFeed_User $cf_user, CultureFeed_Pages_UserList $user_list) {

  $membership = NULL;
  $total_admins = 0;

  foreach ($user_list->memberships as $member) {

    if ($member->user->id == $cf_user->id) {
      // If the form was submit, and request was succesfull. The new admin state will be set in the form_state.
      if (!empty($form_state['new_role'])) {
        $member->role = $form_state['new_role'];
      }
      $membership = $member;
    }

    if ($member->role == CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN) {
      $total_admins++;
    }

  }

  if (empty($membership)) {
    return;
  }

  $membership->page = $page;
  $form = array();

  $form['function'] = array(
    '#type' => 'textfield',
    '#default_value' => $membership->relation,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#ajax' => array(
      'callback' => 'culturefeed_pages_edit_membership_ajax_form_submit',
      'wrapper' => 'membership-ajax-wrapper',
    ),
  );

  // Only show toggle when user is not an admin, or he is not the only admin.
  if ($membership->role != CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN || $total_admins != 1) {
    $form['role'] = array(
      '#type' => 'button',
      '#name' => 'toggle_role',
      '#value' => $membership->role == CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN ? t('Remove administrator') : t('Make administrator'),
      '#ajax' => array(
        'callback' => 'culturefeed_pages_edit_membership_ajax_form_submit',
        'wrapper' => 'membership-ajax-wrapper',
      ),
    );
  }

  $form['#membership'] = $membership;
  $form['#prefix'] = '<div id="membership-ajax-wrapper">';
  $form['#suffix'] = '</div>';

  return $form;

}

/**
 * Submit the edit membership form.
 */
function culturefeed_pages_edit_membership_form_validate($form, &$form_state) {

  try {

    // Toggle admin role was clicked.
    if ($form_state['input']['_triggering_element_name'] == 'toggle_role') {

      $cf_pages = DrupalCultureFeed::getLoggedInUserInstance()->pages();
      if ($form['#membership']->role == CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN) {

        $cf_pages->removeAdmin($form['#membership']->page->getId(), $form['#membership']->user->id);

        // If he was removed as admin, he is also removed as member. Re-add him as member.
        $params = array(
          'relation' => $form['#membership']->relation,
          'activityPrivate' => FALSE,
        );

        $cf_pages->addMember($form['#membership']->page->getId(), $form['#membership']->user->id, $params);
        $form_state['new_role'] = CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_MEMBER;

      }
      else {
        $cf_pages->addAdmin($form['#membership']->page->getId(), $form['#membership']->user->id, array('activityPrivate' => FALSE));
        $form_state['new_role'] = CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN;
      }

    }
    // Normal submit.
    else {

      $params = array('relation' => $form_state['values']['function']);

      $page = $form['#membership']->page;
      $cf_pages = DrupalCultureFeed::getLoggedInUserInstance()->pages();
      if ($form['#membership']->role == CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN) {
        $cf_pages->updateAdmin($page->getId(), $form['#membership']->user->id, $params);
      }
      else {
        $cf_pages->updateMember($page->getId(), $form['#membership']->user->id, $params);
      }

    }

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    form_set_error('', t('Something went wrong while saving. Please try again or contact an administrator.'));
  }

}

/**
 * Submit the edit membership form.
 */
function culturefeed_pages_edit_membership_form_submit(&$form, &$form_state) {

  $form_state['#success'] = TRUE; // This boolean will be checked at in the ajax callback.
  drupal_set_message(t('Membership changed'));

}

/**
 * Form callback for the search user form.
 */
function culturefeed_pages_search_user_form() {

  $form = array();

  $form['title'] = array(
    '#markup' => t('Add new members'),
    '#weight' => -999,
  );

  $form['search'] = array(
    '#title' => t('Keyword'),
    '#type' => 'textfield',
    '#default_value' => isset($_GET['search']) ? $_GET['search'] : '',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search User'),
  );

  return $form;

}

/**
 * Submit the search users form.
 */
function culturefeed_pages_search_user_form_submit($form, &$form_state) {

  $form_state['redirect'] = array(
    $_GET['q'],
    array('query' => array('search' => $form_state['values']['search'])),
  );

}

/**
 * Show the user search result.
 */
function culturefeed_pages_user_search_result($search_key, CultureFeed_Cdb_Item_Page $page, CultureFeed_Pages_UserList $user_list) {

  $query = new CultureFeed_SearchUsersQuery();
  $query->name = $search_key;

  $result = DrupalCultureFeed::searchUsers($query);

  return array(
    '#theme' => 'culturefeed_pages_user_search_result',
    '#result' => $result,
    '#page' => $page,
    '#user_list' => $user_list
  );

}

/**
 * Page callback to add a new member to the page.
 */
function culturefeed_pages_page_add_member(CultureFeed_Cdb_Item_Page $page, CultureFeed_User $user, $request_type = 'nojs') {

  $form = drupal_get_form('culturefeed_pages_add_member_form', $page, $user, $request_type);

  if ($request_type == 'ajax') {
    $output = drupal_render($form);
    print $output;
  }
  else {
    return $form;
  }

}

/**
 * Show the add member confirmation form.
 */
function culturefeed_pages_add_member_form($form, &$form_state, $page, $user, $request_type) {

  if ($request_type == 'ajax') {
    $form['#prefix'] = '<h3>' . t('Add member') . '</h3>';
  }

  $form_state['page'] = $page;
  $form_state['user'] = $user;
  $form_state['query'] = drupal_get_query_parameters();

  $form['add_confirm'] = array(
    '#type' => 'markup',
    '#markup' => format_string('<p><strong>' . t('Sure you want to add @nick as a member?') . '</strong></p><hr />', array('@nick' => $user->nick)),
  );

  $form['remove'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
    '#attributes' => array('class' => array('button-add')),
  );

  $form['decline'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'pages/' . $page->getId() . '/members/nojs',
    '#attributes' => array('class' => array('button-decline')),
  );

  return $form;

}

/**
 * Validate the add member form. Try to send the request to culturefeed.
 */
function culturefeed_pages_add_member_form_submit($form, &$form_state) {

  $page = $form_state['page'];

  $form_state['redirect'] = array(
    'pages/' . $page->getId() . '/members/nojs',
    array('query' => $form_state['query']),
  );

  try {
    $params = array('activityPrivate' => FALSE);
    DrupalCultureFeed::getLoggedInUserInstance()->pages()->addMember($form_state['page']->getId(), $form_state['user']->id, $params);
    $message = t('@nick is member of @page now.', array('@nick' => $form_state['user']->nick, '@page' => $page->getName()));
    drupal_set_message($message);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    drupal_set_message(t('There was an error while saving. Please try again, or contact an administrator.'), 'error');
  }

}

/**
 * Page callback to confirm deletion of a membership.
 * @param CultureFeed_Cdb_Item_Page $page
 */
function culturefeed_pages_page_delete_member(CultureFeed_Cdb_Item_Page $page, $user = NULL, $request_type = 'nojs') {

  $my_pages = FALSE;
  if (empty($user)) {
    try {
      $user = DrupalCultureFeed::getLoggedInUser();
      $my_pages = TRUE;
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_pages', $e);
      return;
    }
  }

  $form = drupal_get_form('culturefeed_pages_delete_member_form', $page, $user, $request_type, $my_pages);

  if ($request_type == 'ajax') {
    $output = drupal_render($form);
    print $output;
  }
  else {
    return $form;
  }

}

/**
 * Form confirmation callback to show a form to confirm the removal of a page.
 * @param CultureFeed_Cdb_Item_Page $page
 *  Page to remove membership.
 * @param CultureFeed_User $user
 *   User to delete membership
 * @param string $request_type
 *   Output beïng requested, ajax or nojs
 * @param $my_pages
 *   Is this page beïng called from the my pages screen, or the manage members screen.
 */
function culturefeed_pages_delete_member_form($form, &$form_state, CultureFeed_Cdb_Item_Page $page, CultureFeed_User $user, $request_type, $my_pages) {

  if ($request_type == 'ajax') {
    $form['#prefix'] = '<h3>' . t('Remove member') . '</h3>';
  }

  $form_state['page'] = $page;
  $form_state['user'] = $user;
  $form_state['my_pages'] = $my_pages;

  $form['remove_confirm'] = array(
    '#type' => 'markup',
    '#markup' => '
      <p>' . t('This action cannot be undone.') . '</p>',
  );

  if ($my_pages) {
    $form['remove_confirm']['#markup'] .= '<p><strong>' . t('Are you sure you want to remove yourself as a member?') . '</strong></p><hr />';
  }
  else {
    $form['remove_confirm']['#markup'] .= '<p><strong>' . t('Are you sure you want to remove this member?') . '</strong></p><hr />';
  }

  $form['remove'] = array(
    '#type' => 'submit',
    '#value' => t('Remove'),
    '#attributes' => array('class' => array('button-remove')),
  );

  $form['decline'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'pages/' . $page->getId() . '/configuration',
    '#attributes' => array('class' => array('button-decline')),
  );

  return $form;

}

/**
 * Validate the delete member form. Send the request to culturefeed.
 */
function culturefeed_pages_delete_member_form_validate($form, &$form_state) {

  // Send request to remove the membership.
  try {

    // Check if the user is admin or member.
    if (culturefeed_pages_is_user_admin_of_page($form_state['page']->getId(), $form_state['user'])) {
      DrupalCultureFeed::getLoggedInUserInstance()->pages()->removeAdmin($form_state['page']->getId(), $form_state['user']->id);
    }
    else {
      DrupalCultureFeed::getLoggedInUserInstance()->pages()->removeMember($form_state['page']->getId(), $form_state['user']->id);
    }

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    form_set_error('', t('There was an error while removing the member.'));
  }

}

/**
 * Submit the delete member form. Redirect to correct page.
 */
function culturefeed_pages_delete_member_form_submit($form, &$form_state) {

  if ($form_state['my_pages']) {
    $form_state['redirect'] = 'culturefeed/pages';
    $message = t('You are not longer a member of @page.', array(
      '@page' => $form_state['page']->getName())
    );
  }
  else {
    $form_state['redirect'] = 'pages/' . $form_state['page']->getId() . '/members/nojs';
    $message = t('User @nick is removed as member of page @page.', array(
      '@nick' => $form_state['user']->nick,
      '@page' => $form_state['page']->getName())
    );
  }

  drupal_set_message($message);

}

/**
 * Page to request approval to become admin member of the page.
 *
 * @param CultureFeed_Cdb_Item_Page $page
 * @param CultureFeed_User $cf_user
 * @param String $type
 */
function culturefeed_pages_page_request_admin_membership($page, $cf_user, $type = 'ajax') {

  $success = TRUE;

  try {
    $instance = DrupalCultureFeed::getLoggedInUserInstance();
    $messages = $instance->messages();
    $data = array(
      'senderPage' => $page->getId(),
      'recipientPage' => $page->getId(),
      'role' => CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN,
      'type' => 'requestadmin',
      'subject' => t('Request to become an administrator'),
      'body' => '{"link":"' . url('pages/' . $page->getId() . '/membership/request-approve/' . $cf_user, array('absolute' => TRUE)) . '"}',
    );

    $messages->sendMessage($data);
  }
  catch (Exception $e) {
    $success = FALSE;
    watchdog_exception('culturefeed_pages', $e);
  }

  $message = t('Your request to become an administrator of this page is sent');

  if ($type == 'ajax') {
    $commands = array();

    if ($success) {
      $commands[] = ajax_command_html('#request_adminship_link_' . $page->getId() . '_wrapper', $message);
    }

    ajax_deliver(array('#type' => 'ajax', '#commands' => $commands));
  }
  else {
    drupal_set_message($message);
    drupal_goto();
  }

}

/**
 * Page callback to approve admin membership.
 *
 * @param array $form
 * @param array $form_state
 * @param CultureFeed_Cdb_Item_Page $page
 * @param CultureFeed_User $cf_user
 */
function culturefeed_pages_page_request_admin_membership_approved($form, &$form_state, $page, $cf_user) {

  $form_state['page'] = $page;
  $form_state['cf_user'] = $cf_user;

  $drupal_uid = culturefeed_get_uid_for_cf_uid($cf_user->id, $cf_user->nick);
  $page_title = $page->getName();
  $message = t('!username wants to be an administrator of !page.', array(
    '!username' => l($cf_user->nick, 'user/' . $drupal_uid),
    '!page' => l($page_title, culturefeed_search_detail_path('page', $page->getId(), $page_title)),
  ));

  $form['message'] = array('#type' => 'markup', '#markup' => $message);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('OK'),
  );

  return $form;

}

/**
 * Submit callback to approve admin membership.
 *
 * @param array $form
 * @param array $form_state
 */
function culturefeed_pages_page_request_admin_membership_approved_submit($form, $form_state) {

  $page = $form_state['page'];
  $cf_user = $form_state['cf_user'];

  $drupal_uid = culturefeed_get_uid_for_cf_uid($cf_user->id, $cf_user->nick);

  $page_title = $page->getName();
  $page_path = culturefeed_search_detail_path('page', $page->getId(), $page_title);

  $message = t('!username successfully added as administrator of page !page.', array(
    '!username' => l($cf_user->nick, 'user/' . $drupal_uid),
    '!page' => l($page_title, $page_path),
  ));

  $success = TRUE;
  try {
    $instance = DrupalCultureFeed::getLoggedInUserInstance();
    $cf_pages = $instance->pages();
    $cf_pages->addAdmin($page->getId(), $cf_user->id, array('activityPrivate' => FALSE));
  }
  catch (Exception $e) {
    $success = FALSE;
    watchdog_exception('culturefeed_pages', $e);
  }

  if ($success) {
    drupal_set_message($message);
  }

  drupal_goto($page_path);

}

/**
 * Sent a request to become validated admin.
 */
function culturefeed_pages_page_request_validated_admin($page, $request_type) {

  // Send request to become a validated admin.
  $success = FALSE;
  try {

    $cf_pages = DrupalCultureFeed::getLoggedInUserInstance()->pages();
    $cf_pages->addValidatedadmin($page->getId());
    $message = t('Your request to become a validated administrator of this page is sent');
    $success = TRUE;
    
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    $message = t('The request could not be sent.');
  }

  if ($request_type == 'ajax') {
    $commands = array();

    if ($success) {
      $commands[] = ajax_command_html('#request_validated_adminship_link_' . $page->getId() . '_wrapper', $message);
    }

    ajax_deliver(array('#type' => 'ajax', '#commands' => $commands));
  }
  else {
    drupal_set_message($message, $success ? 'status': 'error');
    drupal_goto();
  }

}