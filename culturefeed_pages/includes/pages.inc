<?php
/**
 * @file
 * Page callback for culturefeed pages.
 */

/**
 * Page where users can search for a page. And join them.
 */
function culturefeed_pages_join_page_search() {

  $form = drupal_get_form('culturefeed_pages_join_form');

  $result = NULL;
  if (isset($_GET['page'])) {

    try {

      $searchService = culturefeed_get_search_service();
      $parameters = array();
      $parameters[] = new \CultuurNet\Search\Parameter\Query($_GET['page']);
      $result = $searchService->searchPages($parameters);

    }
    catch (Exception\ClientErrorResponseException $e) {
      watchdog_exception('culturefeed_pages', $e);
      return "Er is een fout opgetreden tijdens het laden van de zoekresultaten.";
    }
    catch (Exception\CurlException $e) {
      watchdog_exception('culturefeed_pages', $e);
      return "Er is een fout opgetreden tijdens het laden van de zoekresultaten.";
    }

  }

  return array(
    '#theme' => 'culturefeed_pages_join_page',
    '#form' => $form,
    '#result' => $result,
    '#search' => isset($_GET['page']) ? $_GET['page'] : NULL,
  );

}

/**
 * Form callback for the form on the join page.
 */
function culturefeed_pages_join_form() {

  $form = array();
  $form['#action'] = url('pages/join');

  $form['page'] = array(
    '#type' => 'textfield',
    '#title' => 'Naam van jouw pagina',
    '#autocomplete_path' => 'ajax/culturefeed/pages/page-suggestion',
    '#default_value' => isset($_GET['page']) ? $_GET['page'] : '',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'OK',
  );

  return $form;

}

/**
 * Submit the pages join form.
 */
function culturefeed_pages_join_form_submit($form, &$form_state) {

  $form_state['redirect'] = array(
    'pages/join',
    array('query' => array('page' => $form_state['values']['page'])),
  );

}

/**
 * Page callback to show the details of a culturefeed page.
 */
function culturefeed_pages_detail_page($page) {

  // Hardcoded breadcrumb requested. Use active trail, so breadcrumb can be altered.
  $active_trail = array();

  $active_trail[] = array(
    'title' => t('Home'),
    'href' => '<front>',
    'link_path' => '',
    'localized_options' => array(),
    'type' => 0,
  );

  $active_trail[] = array(
    'title' => 'Pages',
    'href' => 'pages/search',
    'link_path' => '',
    'localized_options' => array(),
    'type' => 0,
  );

  $active_trail[] = array(
    'title' => drupal_get_title(),
    'href' => $_GET['q'],
    'link_path' => '',
    'localized_options' => array(),
    'type' => 0,
  );

  menu_set_active_trail($active_trail);

  $isPageAdmin = FALSE;
  $cultureFeedUser = DrupalCultureFeed::getLoggedInUser();
  $adminMemberships = $cultureFeedUser->getMembershipsByRole(CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN);
  $isPageAdmin = in_array($page->getId(), array_keys($adminMemberships));

  if (!$page->isVisible() && !$isPageAdmin) {
    return array(
      '#theme' => 'culturefeed_page_invisible',
      '#item' => $page,
    );
  }
  else {
    return array(
      '#theme' => 'culturefeed_page',
      '#item' => $page,
    );
  }

}

/**
 * Page callback to join a page.
 * @param string $request_type
 *   Request type. Ajax or nojs
 * @param CultureFeed_Cdb_Item_Page $page
 *   Page to add membership to.
 */
function culturefeed_pages_page_join($request_type, $page) {

  $message = '';

  // Send request to join.
  try {

    $cf_pages = DrupalCultureFeed::getLoggedInUserInstance()->pages();
    $cf_pages->addMember($page->getId(), DrupalCultureFeed::getLoggedInUserId());
    $message = format_string('U bent nu lid van de pagina @page.', array('@page' => $page->getName()));

    // If it's your first membership and you don't have an active page yet,
    // also login as that page.
    $memberships = DrupalCultureFeed::getLoggedInUser()->pageMemberships;
    if (count($memberships) > 0 && !culturefeed_pages_has_active_page()) {
      culturefeed_pages_set_active_page($page);
    }

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    $message = format_string('U kon niet toegevoegd worden aan @page.', array('@page' => $page->getName()));
  }

  // Deliver as ajax callback or normal page.
  if ($request_type != 'ajax') {
    drupal_set_message($message);
    drupal_goto();
  }
  else {
    $commands[] = ajax_command_html('#join-' . $page->getId(), $message);
    ajax_deliver(array('#type' => 'ajax', '#commands' => $commands));
  }

}

/**
 * Page callback for the page suggestions autocomplete.
 */
function culturefeed_pages_page_suggestion_autocomplete_page($search_string) {

  $searchService = culturefeed_get_search_service();
  $matches = array();

  if ($search_string) {

    try {

      // Get the list of suggestions from service.
      $result = $searchService->searchSuggestions($search_string, 'page');
      if ($result->hasSuggestions()) {
        $suggestions = $result->getSuggestions();
        foreach ($suggestions as $suggestion) {
          $matches[$suggestion] = check_plain($suggestion);
        }

      }

    }
    catch (ClientErrorResponseException $e) {
      watchdog_exception('culturefeed_search', $e);
    }

  }

  drupal_json_output($matches);

}

/**
 * Creates form elements to add a page.
 */
function culturefeed_pages_add_form($form, &$form_state) {

  $form = array();

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => 'Naam',
    '#default_value' => '',
    '#weight' => -5,
  );

  $form['categoryId'] = array(
    '#type' => 'select',
    '#title' => 'Type',
    '#options' => culturefeed_search_get_actortype_categories(),
    '#default_value' => CULTUREFEED_ACTORTYPE_ORGANISATION,
    '#weight' => -4,
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => 'Beschrijving',
    '#description' => 'Maximum 250 karakters',
    '#default_value' => '',
    '#maxlength' => 250,
    '#weight' => -3,
  );

  // Form alters can use weight -2 -> 0 here.

  $form['street'] = array(
    '#type' => 'textfield',
    '#title' => 'Adres',
    '#default_value' => '',
    '#attributes' => array('placeholder' => 'straat en huisnummer'),
    '#weight' => 1,
  );

  $form['zip'] = array(
    '#type' => 'textfield',
    '#title' => '',
    '#default_value' => '',
    '#attributes' => array('placeholder' => 'postcode'),
    '#weight' => 2,
  );

  $form['city'] = array(
    '#type' => 'textfield',
    '#title' => '',
    '#default_value' => '',
    '#attributes' => array('placeholder' => 'gemeente'),
    '#weight' => 3,
  );

  // Form alters can use weight 5 -> 9 here.

  $form['contactInfoTel'] = array(
    '#type' => 'textfield',
    '#title' => 'Telefoon nummer',
    '#default_value' => '',
    '#weight' => 10,
  );

  $form['contactInfoEmail'] = array(
    '#type' => 'textfield',
    '#title' => 'Emailadres',
    '#default_value' => '',
    '#weight' => 11,
  );

  $form['websites'] = array(
    '#type' => 'fieldset',
    '#title' => 'Websites',
    '#default_value' => '',
    '#weight' => 12,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['websites']['linkWebsite'] = array(
    '#type' => 'textfield',
    '#title' => 'Website',
    '#default_value' => '',
  );

  $form['websites']['linkTicketing'] = array(
    '#type' => 'textfield',
    '#title' => 'Ticketing',
    '#default_value' => '',
  );

  $form['websites']['linkFacebook'] = array(
    '#type' => 'textfield',
    '#title' => 'Facebook pagina',
    '#default_value' => '',
  );

  $form['websites']['linkTwitter'] = array(
    '#type' => 'textfield',
    '#title' => 'Twitter pagina',
    '#default_value' => '',
  );

  $form['websites']['linkGooglePlus'] = array(
    '#type' => 'textfield',
    '#title' => 'Google+ pagina',
    '#default_value' => '',
  );

  $form['websites']['linkYouTube'] = array(
    '#type' => 'textfield',
    '#title' => 'YouTube pagina',
    '#default_value' => '',
  );

  $form['websites']['linkBlog'] = array(
    '#type' => 'textfield',
    '#title' => 'Blog pagina',
    '#default_value' => '',
  );

  $form['image'] = array(
    '#type' => 'file',
    '#title' => 'Foto',
    '#size' => 26,
    '#default_value' => '',
    '#weight' => 14,
  );

  $form['visible'] = array(
    '#type' => 'hidden',
    '#value' => "true",
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Opslaan',
    '#weight' => 20,
  );

  $form['#validate'] = array(
    'culturefeed_pages_add_arguments_prepare',
    'culturefeed_pages_add_request_send'
  );

  $form['#submit'] = array(
    'culturefeed_pages_add_form_submit'
  );

  return $form;

}

/**
 * Validator to create and prepare all post parameters to create a culturefeed page.
 * @param array $form
 * @param array $form_state
 */
function culturefeed_pages_add_arguments_prepare(& $form, & $form_state) {

  // Basic validation.
  if (empty($form_state['values']['name'])) {
    form_set_error('name', 'De titel moet ingevuld zijn');
  }

  if (strlen($form_state['values']['description']) > 250) {
    form_set_error('description', 'Het maximum karakters bedraagt 250');
  }

  // Add the post params to the form so other modules can add their params.
  $post = $form_state['values'];
  unset($post['form_id'], $post['form_token'], $post['form_build_id'], $post['submit'], $post['op'], $post['image']);
  $form_state['post_params'] = $post;

}

/**
 * Validator which sends the request to create the page.
 * @param array $form
 * @param array $form_state
 */
function culturefeed_pages_add_request_send(& $form, & $form_state) {

  // Local storage of the universal id.
  $uid = NULL;

  // Send request to add a page.
  try {
    $cf_pages = DrupalCultureFeed::getLoggedInUserInstance()->pages();
    $uid = $cf_pages->addPage($form_state['post_params']);
    drupal_set_message(format_string('De pagina "@page" is aangemaakt.', array('@page' => $form_state['values']['name'])));
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    form_set_error('', 'Er is een fout opgetreden tijdens het aanmaken van de pagina.');
  }

  if ($uid) {

    $form_state['page_id'] = $uid;
    $form_state['page_title'] = $form_state['values']['name'];

    // Upload Page image.
    if ($_FILES['files']['tmp_name']['image']) {
      $image_path = $_FILES['files']['tmp_name']['image'];

      $image = file_get_contents($image_path);

      if ($image) {
        try {
          $cf_pages->addImage($uid, array('image' => $image));
        }
        catch (Exception $e) {
          watchdog_exception('culturefeed_pages', $e);
          drupal_set_message('Er is een fout opgetreden bij het opladen van jouw afbeelding.', 'error');
        }
      }
    }

  }

}

/**
 * Submit handler to create a culturefeed page.
 * @param array $form
 * @param array $form_state
 */
function culturefeed_pages_add_form_submit(& $form, & $form_state) {

  // Redirect the user to the created page detail.
  drupal_goto(culturefeed_search_detail_path('page', $form_state['page_id'], $form_state['page_title']));

}

/**
 * Form to edit a page.
 * @param array $form
 * @param array $form_state
 */
function culturefeed_pages_edit_page_form($form, & $form_state, CultureFeed_Cdb_Item_Page $page){

  $form = culturefeed_pages_add_form($form, $form_state);
  $form_state['page'] = $page;

  // Link to the detail page.
  $form['detail_link'] = array(
    '#type' => 'markup',
    '#markup' => culturefeed_search_detail_l("page", $page->getId(), $page->getName(), 'Bekijk pagina', array('attributes' => array('class' => array('view-link')))),
    '#weight' => -25,
  );

  // General information.
  $form['basic'] = array(
    '#type' => 'markup',
    '#markup' => '<h2>Basisinformatie</h2>',
    '#weight' => -20,
  );

  // Hidden page ID.
  $form['pageId'] = array(
    '#type' => 'hidden',
    '#value' => $page->getId(),
  );

  $form['name']['#default_value'] = $page->getName();
  $form['description']['#default_value'] = $page->getDescription();

  // Address.
  $address = $page->getAddress();
  if (!empty($address)) {
    $form['street']['#default_value'] = $address->getStreet();
    $form['zip']['#default_value'] = $address->getZip();
    $form['city']['#default_value'] = $address->getCity();
  }

  // Contact information.
  $form['contactInfoTel']['#default_value'] = $page->getTelephone();
  $form['contactInfoEmail']['#default_value'] = $page->getEmail();

  // Websites list.
  $links = $page->getLinks();
  if (!empty($links)) {
    $form['websites']['linkWebsite']['#default_value'] = isset($links['linkWebsite']) ? $links['linkWebsite'] : '';
    $form['websites']['linkTicketing']['#default_value'] = isset($links['linkTicketing']) ? $links['linkTicketing'] : '';
    $form['websites']['linkFacebook']['#default_value'] = isset($links['linkFacebook']) ? $links['linkFacebook'] : '';
    $form['websites']['linkTwitter']['#default_value'] = isset($links['linkTwitter']) ? $links['linkTwitter'] : '';
    $form['websites']['linkGooglePlus']['#default_value'] = isset($links['linkGooglePlus']) ? $links['linkGooglePlus'] : '';
    $form['websites']['linkYouTube']['#default_value'] = isset($links['linkYouTube']) ? $links['linkYouTube'] : '';
    $form['websites']['linkBlog']['#default_value'] = isset($links['linkBlog']) ? $links['linkBlog'] : '';
  }

  // The image.
  $image = $page->getImage();
  if (!empty($image)) {
    $form['current_image'] = array(
      '#theme' => 'image',
      '#path' => $image . "?width=100&height=100",
      '#weight' => 13,
    );
    $form['image']['#title'] = 'Selecteer andere foto';

    $form['remove_image'] = array(
      '#type' => 'checkbox',
      '#title' => 'Verwijder foto',
      '#weight' => 14,
    );
  }

  $form['#validate'] = array(
    'culturefeed_pages_add_arguments_prepare',
    'culturefeed_pages_edit_request_send',
  );

  $form['#submit'] = array(
    'culturefeed_pages_add_form_submit'
  );

  return $form;

}
/**
 * Validator which sends the request to update a page.
 * @param array $form
 * @param array $form_state
 */
function culturefeed_pages_edit_request_send(& $form, & $form_state) {

  // Local storage of the universal id.
  $uid = $form_state['values']['pageId'];

  // Send request to add a page.
  try {
    $cf_pages = DrupalCultureFeed::getLoggedInUserInstance()->pages();
    $uid = $cf_pages->updatePage($uid, $form_state['post_params']);
    drupal_set_message(format_string('De pagina "@page" is aangepast.', array('@page' => $form_state['values']['name'])));
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    form_set_error('', 'Er is een fout opgetreden tijdens het bewerken van de pagina.');
  }

  if ($uid) {

    $form_state['page_id'] = $uid;
    $form_state['page_title'] = $form_state['values']['name'];

    // Remove the image
    if (!empty($form_state['values']['remove_image'])) {
      // @todo create the call to remove the image from a page.
    }

    // Upload Page image.
    if ($_FILES['files']['tmp_name']['image']) {
      $image_path = $_FILES['files']['tmp_name']['image'];

      $image = file_get_contents($image_path);

      if ($image) {
        try {
          $cf_pages->addImage($uid, array('image' => $image));
        }
        catch (Exception $e) {
          watchdog_exception('culturefeed_pages', $e);
          drupal_set_message('Er is een fout opgetreden bij het opladen van jouw afbeelding.', 'error');
        }
      }
    }

  }

}

/**
 * Form callback to render a page to configure a page.
 * E.g. delete a page.
 * @param array $form
 * @param array $form_state
 */
function culturefeed_pages_configuration_page_form($form, & $form_state, CultureFeed_Cdb_Item_Page $page){

  //dvm($page);
  $form_state['page'] = $page;

  // Link to the detail page.
  $form['detail_link'] = array(
    '#type' => 'markup',
    '#markup' => culturefeed_search_detail_l("page", $page->getId(), $page->getName(), 'Bekijk pagina', array('attributes' => array('class' => array('view-link')))),
    '#weight' => -25,
  );

  // General information.
  $form['basic'] = array(
    '#type' => 'markup',
    '#markup' => '<h2>Instellingen</h2>',
    '#weight' => -20,
  );

  // Hidden page ID.
  $form['pageId'] = array(
    '#type' => 'hidden',
    '#value' => $page->getId(),
  );

  $permissions = $page->getPermissions();

  $form['allow_followers'] = array(
    '#type' => 'checkbox',
    '#title' => 'mijn pagina volgen',
    '#description' => 'Op die manier kunnen gebruikers op de hoogte blijven van alle updates over deze pagina.',
    '#default_value' => !empty($permissions->allowFollowers),
  );

  $form['allow_members'] = array(
    '#type' => 'checkbox',
    '#title' => 'lid worden van mijn pagina',
    '#description' => 'Op die manier wordt je pagina ook toegankelijk via de profielen, beoordelingen en reacties van alle collega\'s die actief zijn op deze site.',
    '#default_value' => !empty($permissions->allowMembers),
  );

  $form['allow_comments'] = array(
    '#type' => 'checkbox',
    '#title' => 'beoordelingen schrijven over mijn activiteiten',
    '#description' => 'Enkel voor organisaties die activiteiten organiseren en ingevoerd hebben via <a href="http://www.UiTdatabank.be">www.UiTdatabank.be</a>.',
    '#default_value' => !empty($permissions->allowComments),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Opslaan',
  );

  if ($page->isVisible()) {
    $form['remove-link'] = array(
      '#type' => 'markup',
      '#markup' => l('Pagina verwijderen',
          'pages/' . $page->getId() . '/remove/nojs',
          array(
            'attributes' => array(
              'role' => 'button',
              'data-toggle' => 'modal',
              'data-target' => '#page_confirm',
              'data-remote' => url('pages/' . $page->getId() . '/remove/ajax'),
            ),
          )
      ),
    );
  }
  else {
    // @todo We can use this to quickly reinvoke the status.
    /*$form['publish-link'] = array(
      '#type' => 'markup',
      '#markup' => l('Pagina opnieuw publiceren',
        'pages/' . $page->getId() . '/publish',
        array(
          'attributes' => array(
            'role' => 'button',
            'data-toggle' => 'modal',
            'data-target' => '#page_confirm',
          ),
        )
      ),
    );*/
  }

  $form['page_confirm'] = array(
    '#type' => 'markup',
    '#markup' => '
      <div id="page_confirm" class="modal hide fade" tabindex="-1" role="dialog" aria-hidden="true">
        <div class="modal-body"></div>
      </div>',
  );

  return $form;

}

/**
 * Submit handler to save the permissions of the configuration page.
 * @param array $form
 * @param array $form_state
 */
function culturefeed_pages_configuration_page_form_submit($form, &$form_state) {

  $params = array(
    'allowFollowers' => empty($form_state['values']['allow_followers']) ? "false" : "true",
    'allowMembers' => empty($form_state['values']['allow_members']) ? "false" : "true",
    'allowComments' => empty($form_state['values']['allow_comments']) ? "false" : "true",
  );

  // Send request to change permissions for a page.
  try {
    $cf_pages = DrupalCultureFeed::getLoggedInUserInstance()->pages();
    $uid = $cf_pages->changePermissions($form_state['values']['pageId'], $params);
    drupal_set_message(format_string('De pagina instellingen zijn bewaard.'));
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    form_set_error('', 'Er is een fout opgetreden tijdens het bewaren van de pagina instellingen.');
  }

}

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

  $extra_trails = array();
  $extra_trails[] = array(
    'title' => drupal_get_title(),
    'href' => $_GET['q'],
    'link_path' => '',
    'localized_options' => array(),
    'type' => 0,
  );
  culturefeed_pages_set_page_breadcrumb($page, $extra_trails);

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
        $cf_pages->addMember($form['#membership']->page->getId(), $form['#membership']->user->id, array('relation' => $form['#membership']->relation));
        $form_state['new_role'] = CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_MEMBER;
      }
      else {
        $cf_pages->addAdmin($form['#membership']->page->getId(), $form['#membership']->user->id);
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
 * Switch to another CultureFeed Page.
 * @param CultureFeed_Cdb_Item_Page $page
 */
function culturefeed_pages_switch_page(CultureFeed_Cdb_Item_Page $page) {
  culturefeed_pages_set_active_page($page);
  drupal_goto(culturefeed_search_detail_path('page', $page->getId(), $page->getName()));
}

/**
 * Page callback to add a confirimation to the
 * @param CultureFeed_Cdb_Item_Page $page
 */
function culturefeed_pages_remove_page_confirm($page, $request_type = 'ajax') {

  $form = drupal_get_form('culturefeed_pages_remove_page_confirm_form', $page, $request_type);

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
 */
function culturefeed_pages_remove_page_confirm_form($form, &$form_state, $page, $request_type) {

  if ($request_type == 'ajax') {
    $form['#prefix'] = '<div class="modal-header"><h3>Pagina verwijderen</h3></div>';
  }

  $form_state['page_id'] = $page->getId();
  $form_state['page'] = $page;

  $form['body_wrapper'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="confirm-wrapper" class="modal-body">',
    '#suffix' => '</div>',
  );
  $form['body_wrapper']['page_remove_confirm'] = array(
    '#type' => 'markup',
    '#markup' => '
      <p>Door een pagina te verwijderen, zal de pagina niet meer verschijnen in de zoekresultaten en worden je al je contactgegevens niet verwijderd.</p>
      <p>Leden van deze pagina en activiteiten in naam van deze pagina worden niet verwijderd.</p>
      <p><strong>Ben je zeker?</strong></p>',
  );

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
 * Function to actually delete the page.
 */
function culturefeed_pages_remove_page_confirm_form_submit($form, &$form_state) {

  $page = $form_state['page'];

  // Send request to remove a page.
  try {
    $cf_pages = DrupalCultureFeed::getLoggedInUserInstance()->pages();
    $uid = $cf_pages->removePage($page->getId());
    drupal_set_message(format_string('De pagina "@page" is verwijderd.', array('@page' => $page->getName())));
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    form_set_error('', 'Er is een fout opgetreden tijdens het verwijderen van de pagina.');
  }

  $form_state['redirect'] = culturefeed_search_detail_url("page", $page->getId(), $page->getName());

}

/**
 * Page callback to add a confirimation to the
 * @param CultureFeed_Cdb_Item_Page $page
 */
function culturefeed_pages_publish_page_confirm($page) {

  $form = drupal_get_form('culturefeed_pages_publish_page_confirm_form', $page);
  $output = drupal_render($form);

  print $output;
  exit();
}

/**
 * Form confirmation callback to show a form to confirm the publishing of a page.
 */
function culturefeed_pages_publish_page_confirm_form($form, &$form_state, $page) {

  $form_state['page_id'] = $page->getId();
  $form_state['page'] = $page;

  $form['wrapper'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="confirm-wrapper">',
    '#suffix' => '</div>',
  );
  $form['wrapper']['page_confirm'] = array(
    '#type' => 'markup',
    '#markup' => '
      <p>Door een pagina te publiceren, zal de pagina opnieuw verschijnen in de zoekresultaten.</p>
      <p>Leden van deze pagina en activiteiten in naam van deze pagina worden opnieuw in het leven geroepen.</p>
      <p><strong>Ben je zeker?</strong></p>',
  );

  $form['wrapper']['publish'] = array(
    '#type' => 'submit',
    '#value' => 'Publiceer',
    '#attributes' => array('class' => array('button-publish')),
  );

  $form['wrapper']['decline'] = array(
    '#type' => 'button',
    '#value' => 'Annuleer',
    '#attributes' => array(
      'data-dismiss' => 'modal',
      'aria-hidden' => 'true',
      'class' => array('button-decline close')
    ),
  );

  return $form;

}

/**
 * Function to actually publish the page.
 */
function culturefeed_pages_publish_page_confirm_form_submit($form, &$form_state) {

  $page = $form_state['page'];

  // Send request to (re-)publish a page.
  try {
    $cf_pages = DrupalCultureFeed::getLoggedInUserInstance()->pages();
    $uid = $cf_pages->publishPage($page->getId());
    drupal_set_message(format_string('De pagina "@page" is hersteld.', array('@page' => $page->getName())));
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    form_set_error('', 'Er is een fout opgetreden tijdens het herstellen van de pagina.');
  }

  $form_state['redirect'] = culturefeed_search_detail_url("page", $page->getId(), $page->getName());

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
    DrupalCultureFeed::getLoggedInUserInstance()->pages()->addMember($form_state['page']->getId(), $form_state['user']->id);
    $message = format_string('@nick is nu lid van @page.', array('@nick' => $form_state['user']->nick, '@page' => $page->getName()));
    drupal_set_message($message);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    drupal_set_message('Er ging iets fout tijdens het toevoegen. Gelieve opnieuw te proberen, of een administrator te contacteren', 'error');
  }

}

/**
 * Page callback to add a confirimation to the
 * @param CultureFeed_Cdb_Item_Page $page
 */
function culturefeed_pages_page_delete_member(CultureFeed_Cdb_Item_Page $page, CultureFeed_User $user, $request_type = 'nojs') {

  $form = drupal_get_form('culturefeed_pages_delete_member_form', $page, $user, $request_type);

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
 */
function culturefeed_pages_delete_member_form($form, &$form_state, CultureFeed_Cdb_Item_Page $page, CultureFeed_User $user, $request_type) {

  if ($request_type == 'ajax') {
    $form['#prefix'] = '<div class="modal-header"><h3>Lid verwijderen</h3></div>';
  }

  $form_state['page'] = $page;
  $form_state['user'] = $user;

  $form['body_wrapper'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="confirm-wrapper" class="modal-body">',
    '#suffix' => '</div>',
  );
  $form['body_wrapper']['remove_confirm'] = array(
    '#type' => 'markup',
    '#markup' => '
      <p>Deze actie kan niet meer ongedaan gemaakt worden.</p>
      <p><strong>Ben je zeker dat u dit lid wil verwijderen?</strong></p>',
  );

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
    DrupalCultureFeed::getLoggedInUserInstance()->pages()->removeMember($form_state['page']->getId(), $form_state['user']->id);
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

  $form_state['redirect'] = 'pages/' . $form_state['page']->getId() . '/members/nojs';
  $message = format_string('De gebruiker @nick is verwijderd als lid van @page.', array(
    '@nick' => $form_state['user']->nick,
    '@page' => $form_state['page']->getName())
  );
  drupal_set_message($message);

}
