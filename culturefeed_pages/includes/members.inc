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
    '#markup' => '<div id="view-page">' . culturefeed_search_detail_l('page', $page->getId(), $page->getName(), 'Bekijk pagina') . '</div>'
  );
  $build['page'] = $list;
  $build['search_form'] = drupal_get_form('culturefeed_pages_search_user_form');

  if (isset($_GET['search'])) {
    if (strlen($_GET['search']) >= 3) {
      $build['search_result'] = culturefeed_pages_user_search_result($_GET['search'], $page, $user_list);
    }
    else {
      drupal_set_message('Gelieve minstens 3 karakters in te geven', 'error');
    }
  }

  culturefeed_pages_set_page_breadcrumb($page);

  return $build;

}

/**
 * Show the list of members from a page.
 */
function culturefeed_pages_manage_members_list(CultureFeed_Cdb_Item_Page $page, CultureFeed_Pages_UserList $user_list, $cf_user = NULL) {

  $uids = culturefeed_get_uids_for_memberships($user_list->memberships);

  $header = array(
    'Naam',
    'Functie',
    'Rol',
    'Lid sinds',
    '',
    '',
  );

  $rows = array();
  foreach ($user_list->memberships as $member) {

    $row = array();

    $name = '';
    if (!empty($member->user->depiction)) {
      $name = '<span class="depiction">' . theme('image', array('path' => $member->user->depiction . '?maxwidth=100')) . '</span>';
    }
    $name .= l($member->user->nick, url('user/' . $uids[$member->user->id]));

    $row['name'] = $name;

    // Show edit form or normal data.
    if (empty($cf_user) || $cf_user->id != $member->user->id) {

      $row['function'] = $member->relation;
      $row['role'] = $member->role;
      $row['member_since'] = date('d/m/Y H:i', $member->creationDate);
      $row['edit'] = l('Bewerk', 'pages/' . $page->getId() . '/membership/' . $member->user->id . '/edit/nojs', array('attributes' => array('class' => 'use-ajax')));

      $delete_options = array(
        'attributes' => array(
          'role' => 'button',
          'data-toggle' => 'modal',
          'data-target' => '#page_confirm',
          'data-remote' => url('pages/' . $page->getId() . '/membership/' . $member->user->id . '/delete/ajax'),
        ),
      );
      $row['delete'] = l('Verwijder als lid', 'pages/' . $page->getId() . '/membership/' . $member->user->id . '/delete/nojs', $delete_options);

    }
    else {

      $form = drupal_get_form('culturefeed_pages_edit_membership_form', $page, $cf_user, $user_list);
      $row['function'] = array(
        'data' => render($form),
        'colspan' => 2,
      );
      $row['member_since'] = date('d/m/Y H:i', $member->creationDate);
      $row['cancel'] = array(
        'data' => l('Annuleer bewerkingen', 'pages/' . $page->getId() . '/members/nojs', array('attributes' => array('class' => 'use-ajax'))),
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
    '#suffix' => '</div><div id="page_confirm" class="modal hide fade" tabindex="-1" role="dialog" aria-hidden="true">
        <div class="modal-body"></div>
      </div>',
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
    '#value' => 'Bewaar',
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
      '#value' => $membership->role == CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN ? 'Verwijder als beheerder' : 'Maak beheerder',
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
    form_set_error('', 'Er ging iets fout tijdens het bewaren. Gelieve opnieuw te proberen of een administrator te contacteren.');
  }

}

/**
 * Submit the edit membership form.
 */
function culturefeed_pages_edit_membership_form_submit(&$form, &$form_state) {

  $form_state['#success'] = TRUE; // This boolean will be checked at in the ajax callback.
  drupal_set_message('Het lidmaatschap werd aangepast');

}

/**
 * Form callback for the search user form.
 */
function culturefeed_pages_search_user_form() {

  $form = array();
  $form['search'] = array(
    '#title' => 'Zoek gebruikers',
    '#type' => 'textfield',
    '#default_value' => isset($_GET['search']) ? $_GET['search'] : '',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Zoek',
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
    $form['#prefix'] = '<div class="modal-header"><h3>Lid toevoegen</h3></div>';
  }

  $form_state['page'] = $page;
  $form_state['user'] = $user;
  $form_state['query'] = drupal_get_query_parameters();

  $form['body_wrapper'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="confirm-wrapper" class="modal-body">',
    '#suffix' => '</div>',
  );
  $form['body_wrapper']['add_confirm'] = array(
    '#type' => 'markup',
    '#markup' => format_string('<p><strong>Ben je zeker dat u @nick wil toevoegen als lid?</strong></p>', array('@nick' => $user->nick)),
  );

  $form['footer_wrapper'] = array(
    '#type' => 'markup',
    '#prefix' => '<div class="modal-footer">',
    '#suffix' => '</div>',
  );

  $form['footer_wrapper']['remove'] = array(
    '#type' => 'submit',
    '#value' => 'Toevoegen',
    '#attributes' => array('class' => array('button-add btn btn-primary')),
  );

  $form['footer_wrapper']['decline'] = array(
    '#type' => 'link',
    '#title' => 'Annuleer',
    '#href' => 'pages/' . $page->getId() . '/members/nojs',
    '#attributes' => array(
      'data-dismiss' => 'modal',
      'aria-hidden' => 'true',
      'class' => array('button-decline close btn')
    ),
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
    $message = format_string('@nick is nu lid van @page.', array('@nick' => $form_state['user']->nick, '@page' => $page->getName()));
    drupal_set_message($message);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    drupal_set_message('Er ging iets fout tijdens het toevoegen. Gelieve opnieuw te proberen, of een administrator te contacteren', 'error');
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
    $form['#prefix'] = '<div class="modal-header"><h3>Lid verwijderen</h3></div>';
  }

  $form_state['page'] = $page;
  $form_state['user'] = $user;
  $form_state['my_pages'] = $my_pages;

  $form['body_wrapper'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="confirm-wrapper" class="modal-body">',
    '#suffix' => '</div>',
  );
  $form['body_wrapper']['remove_confirm'] = array(
    '#type' => 'markup',
    '#markup' => '
      <p>Deze actie kan niet meer ongedaan gemaakt worden.</p>',
  );

  if ($my_pages) {
    $form['body_wrapper']['remove_confirm']['#markup'] .= '<p><strong>Ben je zeker dat je jezelf als lid wil verwijderen?</strong></p>';
  }
  else {
    $form['body_wrapper']['remove_confirm']['#markup'] .= '<p><strong>Ben je zeker dat je dit lid wil verwijderen?</strong></p>';
  }

  $form['footer_wrapper'] = array(
    '#type' => 'markup',
    '#prefix' => '<div class="modal-footer">',
    '#suffix' => '</div>',
  );

  $form['footer_wrapper']['remove'] = array(
    '#type' => 'submit',
    '#value' => 'Verwijder',
    '#attributes' => array('class' => array('button-remove btn btn-warning')),
  );

  $form['footer_wrapper']['decline'] = array(
    '#type' => 'link',
    '#title' => 'Annuleer',
    '#href' => 'pages/' . $page->getId() . '/configuration',
    '#attributes' => array(
      'data-dismiss' => 'modal',
      'aria-hidden' => 'true',
      'class' => array('button-decline close btn')
    ),
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
    form_set_error('', 'Er is een fout opgetreden tijdens het verwijderen van het lid.');
  }

}

/**
 * Submit the delete member form. Redirect to correct page.
 */
function culturefeed_pages_delete_member_form_submit($form, &$form_state) {

  if ($form_state['my_pages']) {
    $form_state['redirect'] = 'culturefeed/pages';
    $message = format_string('Je ben niet langer lid van @page.', array(
      '@page' => $form_state['page']->getName())
    );
  }
  else {
    $form_state['redirect'] = 'pages/' . $form_state['page']->getId() . '/members/nojs';
    $message = format_string('De gebruiker @nick is verwijderd als lid van @page.', array(
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
      'subject' => 'Aanvraag om beheerder te worden',
      'body' => '{"link":"' . url('pages/' . $page->getId() . '/membership/request-approve/' . $cf_user->id, array('absolute' => TRUE)) . '"}',
    );

    $messages->sendMessage($data);
  }
  catch (Exception $e) {
    $success = FALSE;
    watchdog_exception('culturefeed_pages', $e);
  }

  $message = '<div class="alert alert-success">Je aanvraag om beheerder te worden van deze pagina is verstuurd</div>';

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
  $message = format_string("!username wil beheerder worden van !page.", array(
    '!username' => l($cf_user->nick, 'user/' . $drupal_uid),
    '!page' => l($page_title, culturefeed_search_detail_path('page', $page->getId(), $page_title)),
  ));

  $form['message'] = array('#type' => 'markup', '#markup' => $message);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'OK',
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

  $message = format_string("!username werd succesvol toegevoegd als beheerder van !page.", array(
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
