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

  $items = array();
  if (isset($_GET['page'])) {

    try {

      $parameters = array();
      $parameters[] = new \CultuurNet\Search\Parameter\Query($_GET['page']);
      $result = culturefeed_get_search_service()->searchPages($parameters);
      $items = $result->getItems();

    }
    catch (Exception\ClientErrorResponseException $e) {
      watchdog_exception('culturefeed_pages', $e);
      return t('There was an error while loading the search results.');
    }
    catch (Exception\CurlException $e) {
      watchdog_exception('culturefeed_pages', $e);
      return t('There was an error while loading the search results.');
    }

  }

  return array(
    '#theme' => 'culturefeed_pages_join_page',
    '#form' => $form,
    '#results' => $items,
    '#search' => isset($_GET['page']) ? $_GET['page'] : NULL,
  );

}

/**
 * Form callback for the form on the join page.
 */
function culturefeed_pages_join_form() {

  $form = array();

  $form['page'] = array(
    '#type' => 'textfield',
    '#title' => t('Name of your page'),
    '#autocomplete_path' => 'ajax/culturefeed/pages/page-suggestion',
    '#default_value' => isset($_GET['page']) ? $_GET['page'] : '',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('OK'),
  );

  return $form;

}

/**
 * Submit the pages join form.
 */
function culturefeed_pages_join_form_submit($form, &$form_state) {

  $form_state['redirect'] = array(
    $_GET['q'],
    array('query' => array('page' => $form_state['values']['page'])),
  );

}

/**
 * Page callback to show the details of a culturefeed page.
 */
function culturefeed_pages_detail_page(CultureFeed_Cdb_Item_Page $page) {

  culturefeed_pages_set_page_breadcrumb($page);

  if (!$page->isVisible() && !culturefeed_pages_is_user_admin_of_page($page->getId())) {
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
  $status_code = 'error';

  // Send request to join.
  try {

    $cf_pages = DrupalCultureFeed::getLoggedInUserInstance()->pages();
    $cf_pages->addMember($page->getId(), DrupalCultureFeed::getLoggedInUserId(), array('activityPrivate' => FALSE));
    $message = format_string(t('You are now a member of <strong>@page.</strong><br />Manage your membership of this and other pages with') . ' ' . l(t('My pages'), 'culturefeed/pages'), array('@page' => $page->getName()));
    $status_code = 'status';

    // If it's your first membership and you don't have an active page yet,
    // also login as that page.
    $memberships = DrupalCultureFeed::getLoggedInUser()->pageMemberships;
    if (count($memberships) > 0 && !culturefeed_pages_has_active_page()) {
      culturefeed_pages_set_active_page($page);
    }

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);

    if ($e->getCode() == CultureFeed_Pages_Default::ACTION_NOT_ALLOWED) {
      $message = format_string(t('It is not allowed to join @page as a member. Please contact the administrator of the page to become a member.'), array('@page' => $page->getName()));
    }
    else {
      $message = format_string(t('You could not be added to @page.'), array('@page' => $page->getName()));
    }
  }

  // Deliver as ajax callback or normal page.
  if ($request_type != 'ajax') {
    drupal_set_message($message, $status_code);
    drupal_goto();
  }
  else {
    $commands[] = ajax_command_html('#join-' . $page->getId(), $message);
    ajax_deliver(array('#type' => 'ajax', '#commands' => $commands));
  }

}

/**
 * Page callback to follow a page.
 * @param string $request_type
 *   Request type. Ajax or nojs
 * @param CultureFeed_Cdb_Item_Page $page
 *   Page to follow.
 * @param Boolean $follow
 *   action is follow, false to defollow.
 */
function culturefeed_pages_page_follow($request_type, $page, $follow = TRUE) {

  $message = '';

  // Send request to follow.
  try {

    $cf_pages = DrupalCultureFeed::getLoggedInUserInstance()->pages();
    if ($follow) {
      $cf_pages->follow($page->getId(), array('activityPrivate' => FALSE));

      $message = theme('culturefeed_pages_follow_success_message', array('page' => $page));

    }
    else {
      $cf_pages->defollow($page->getId(), DrupalCultureFeed::getLoggedInUserId());
      $message = theme('culturefeed_pages_unfollow_success_message', array('page' => $page));
    }

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    $message = format_string(t('You could not be added to @page.'), array('@page' => $page->getName()));
  }

  // Deliver as ajax callback or normal page.
  if ($request_type != 'ajax') {
    drupal_set_message($message);
    drupal_goto(culturefeed_search_detail_path('page', $page->getId(), $page->getName()));
  }
  else {
    $commands[] = ajax_command_html('.follow-' . $page->getId(), $message);
    ajax_deliver(array('#type' => 'ajax', '#commands' => $commands));
  }
}

/**
 * Page callback for the page suggestions autocomplete.
 */
function culturefeed_pages_page_suggestion_autocomplete_page($search_string) {

  $matches = array();
  if ($search_string) {

    try {

      // Get the list of suggestions from service.
      $result = culturefeed_get_search_service()->searchSuggestions($search_string, array('page'));
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

  // Cache if possible per request.
  drupal_page_footer();
}

/**
 * Creates form elements to add a page.
 */
function culturefeed_pages_add_form($form, &$form_state) {

  $form = array();

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => '',
    '#weight' => -5,
  );

  $form['categoryId'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#options' => culturefeed_search_get_actortype_categories(),
    '#default_value' => CULTUREFEED_ACTORTYPE_ORGANISATION,
    '#weight' => -4,
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('Maximum 400 characters'),
    '#default_value' => '',
    '#maxlength' => 400,
    '#weight' => -3,
  );

  // Form alters can use weight -2 -> 0 here.

  $form['street'] = array(
    '#type' => 'textfield',
    '#title' => t('Address'),
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
    '#title' => t('Phone'),
    '#default_value' => '',
    '#weight' => 10,
  );

  $form['contactInfoEmail'] = array(
    '#type' => 'textfield',
    '#title' => t('Email address'),
    '#default_value' => '',
    '#weight' => 11,
  );

  $form['linkWebsite'] = array(
    '#type' => 'textfield',
    '#title' => t('Website'),
    '#default_value' => '',
    '#weight' => 12,
  );

  $form['otherWebsites'] = array(
    '#type' => 'fieldset',
    '#title' => t('Other websites'),
    '#default_value' => '',
    '#weight' => 13,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['otherWebsites']['linkTicketing'] = array(
    '#type' => 'textfield',
    '#title' => t('Ticketing'),
    '#default_value' => '',
  );

  $form['otherWebsites']['linkFacebook'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook'),
    '#default_value' => '',
  );

  $form['otherWebsites']['linkTwitter'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter'),
    '#default_value' => '',
  );

  $form['otherWebsites']['linkGooglePlus'] = array(
    '#type' => 'textfield',
    '#title' => t('Google+'),
    '#default_value' => '',
  );

  $form['otherWebsites']['linkYouTube'] = array(
    '#type' => 'textfield',
    '#title' => t('YouTube'),
    '#default_value' => '',
  );

  $form['otherWebsites']['linkBlog'] = array(
    '#type' => 'textfield',
    '#title' => t('Blog'),
    '#default_value' => '',
  );

  $form['image'] = array(
    '#type' => 'file',
    '#title' => t('Picture or logo'),
    '#size' => 26,
    '#default_value' => '',
    '#weight' => 16,
  );

  $form['visible'] = array(
    '#type' => 'hidden',
    '#value' => "true",
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
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
    form_set_error('name', t('Title is required'));
    $form_state['#validation_failed'] = TRUE;
  }

  if (strlen($form_state['values']['description']) > 400) {
    form_set_error('description', t('The maximum is 400 characters'));
    $form_state['#validation_failed'] = TRUE;
  }

  // Add the post params to the form so other modules can add their params.
  $post = $form_state['values'];
  unset($post['form_id'], $post['form_token'], $post['form_build_id'], $post['submit'], $post['op'], $post['image']);

  // Make sure people don't save only http://
  $link_fields = array(
		'linkWebsite',
  	'linkTicketing',
    'linkFacebook',
  	'linkTwitter',
  	'linkGooglePlus',
  	'linkYouTube',
  	'linkBlog',
  );
  foreach ($link_fields as $link_field) {
  	if ($post[$link_field] == 'http://') {
  		$post[$link_field] = '';
  	}
  }

  $form_state['post_params'] = $post;

}

/**
 * Validator which sends the request to create the page.
 * @param array $form
 * @param array $form_state
 */
function culturefeed_pages_add_request_send(& $form, & $form_state) {

  // If a previous validation function failed, don't send the request.
  if (!empty($form_state['#validation_failed'])) {
    return;
  }

  // Local storage of the universal id.
  $uid = NULL;

  // Send request to add a page.
  try {
    $cf_pages = DrupalCultureFeed::getLoggedInUserInstance()->pages();
    $uid = $cf_pages->addPage($form_state['post_params']);
    drupal_set_message(format_string(t('Page "@page" has been created.'), array('@page' => $form_state['values']['name'])));
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    form_set_error('', t('There was an error while creating the page.'));
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
          drupal_set_message(t('An error occurred when uploading your image.'), 'error');
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
function culturefeed_pages_edit_page_form($form, & $form_state, CultureFeed_Cdb_Item_Page $page) {

  $form = culturefeed_pages_add_form($form, $form_state);
  $form_state['page'] = $page;

  // Link to the detail page.
  $form['detail_link'] = array(
    '#type' => 'markup',
    '#markup' => culturefeed_search_detail_l("page", $page->getId(), $page->getName(), t('View page'), array('attributes' => array('class' => array('view-link')))),
    '#weight' => -25,
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
    $form['linkWebsite']['#default_value'] = isset($links['linkWebsite']) ? $links['linkWebsite'] : '';
    $form['otherWebsites']['linkTicketing']['#default_value'] = isset($links['linkTicketing']) ? $links['linkTicketing'] : '';
    $form['otherWebsites']['linkFacebook']['#default_value'] = isset($links['linkFacebook']) ? $links['linkFacebook'] : '';
    $form['otherWebsites']['linkTwitter']['#default_value'] = isset($links['linkTwitter']) ? $links['linkTwitter'] : '';
    $form['otherWebsites']['linkGooglePlus']['#default_value'] = isset($links['linkGooglePlus']) ? $links['linkGooglePlus'] : '';
    $form['otherWebsites']['linkYouTube']['#default_value'] = isset($links['linkYouTube']) ? $links['linkYouTube'] : '';
    $form['otherWebsites']['linkBlog']['#default_value'] = isset($links['linkBlog']) ? $links['linkBlog'] : '';
  }

  // The image.
  $image = $page->getImage();
  if (!empty($image)) {
    $form['current_image'] = array(
      '#theme' => 'image',
      '#path' => $image . "?width=100&height=100",
      '#weight' => 14,
    );
    $form['image']['#title'] = t('Select another photo');

    $form['remove_image'] = array(
      '#type' => 'checkbox',
      '#title' => t('Remove photo'),
      '#weight' => 15,
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
    drupal_set_message(format_string(t('Page "@page" is updated.'), array('@page' => $form_state['values']['name'])));
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    form_set_error('', t('There was an error while updating the page.'));
  }

  if ($uid) {

    $form_state['page_id'] = $uid;
    $form_state['page_title'] = $form_state['values']['name'];

    // Remove the image if requested.
    if (!empty($form_state['values']['remove_image'])) {
      try {
        $cf_pages->removeImage($uid);
      }
      catch (Exception $e) {
        watchdog_exception('culturefeed_pages', $e);
      }
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
          drupal_set_message(t('An error occurred when uploading your image.'), 'error');
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

  $form_state['page'] = $page;

  // Link to the detail page.
  $form['detail_link'] = array(
    '#type' => 'markup',
    '#markup' => culturefeed_search_detail_l("page", $page->getId(), $page->getName(), t('View page'), array('attributes' => array('class' => array('view-link')))),
    '#weight' => -25,
  );

  // General information.
  $form['basic'] = array(
    '#type' => 'markup',
    '#markup' => '<h2>' . t('Settings') . '</h2>',
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
    '#title' => t('follow my page'),
    '#description' => t('This allows users to stay informed of any updates on this page.'),
    '#default_value' => !empty($permissions->allowFollowers),
  );

  $form['allow_members'] = array(
    '#type' => 'checkbox',
    '#title' => t('become a member of my page'),
    '#description' => t('In this way, your page can also be accessed via the profiles, comments and reactions of all colleagues who are active on this site.'),
    '#default_value' => !empty($permissions->allowMembers),
  );

  $form['allow_comments'] = array(
    '#type' => 'checkbox',
    '#title' => t('write comments about my activities'),
    '#description' => t('Only for organizations that provide activities and have entered via <a href="http://www.UiTdatabank.be">www.UiTdatabank.be</a>.'),
    '#default_value' => !empty($permissions->allowComments),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  if ($page->isVisible()) {
    $form['remove-link'] = array(
      '#type' => 'link',
      '#title' => t('Remove page'),
      '#href' => 'pages/' . $page->getId() . '/remove/nojs',
      '#attributes' => array(
        'role' => 'button',
        'data-toggle' => 'modal',
        'data-target' => '#page_confirm',
        'data-remote' => url('pages/' . $page->getId() . '/remove/ajax'),
      ),
    );
  }
  else {
    // @todo We can use this to quickly reinvoke the status.
    /*$form['publish-link'] = array(
      '#type' => 'link',
      '#title' => 'Pagina opnieuw publiceren',
      '#href' => 'pages/' . $page->getId() . '/publish/nojs',
      '#attributes' => array(
        'role' => 'button',
        'data-toggle' => 'modal',
        'data-target' => '#page_confirm',
        'data-remote' => url('pages/' . $page->getId() . '/publish/ajax'),
      ),
    );*/
  }

  $form['#suffix'] = '
    <div id="page_confirm" class="modal hide fade" tabindex="-1" role="dialog" aria-hidden="true">
      <div class="modal-body"></div>
    </div>';

  culturefeed_pages_set_page_breadcrumb($page);

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
    drupal_set_message(format_string(t('Page settings has been saved.')));
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    form_set_error('', t('There was an error while saving the settings page.'));
  }

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
    $form['#prefix'] = '<div class="modal-header"><h3>' . t('Remove page') . '</h3></div>';
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
      <p>' . t('By deleting a page, the page will no longer appear in search results, but it will not remove your contact info.') . '</p>
      <p>' . t('Members of this page, and activities in the name of this page will not be deleted.') . '</p>
      <p><strong>' . t('Are you sure?') . '</strong></p>',
  );

  $form['footer_wrapper'] = array(
    '#type' => 'markup',
    '#prefix' => '<div class="modal-footer">',
    '#suffix' => '</div>',
  );

  $form['footer_wrapper']['remove'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#attributes' => array('class' => array('button-remove btn btn-warning')),
    '#submit' => array('culturefeed_pages_remove_page_confirm_form_submit'),
    '#limit_validation_errors' => array(),
  );

  $form['footer_wrapper']['decline'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
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
    $cf_pages->removePage($page->getId());
    drupal_set_message(format_string(t('Page "@page" is deleted.'), array('@page' => $page->getName())));

    // Set the next available page as active.
    culturefeed_pages_remove_active_page();
    $cf_user = DrupalCultureFeed::getLoggedInUser();
    if (!empty($cf_user->pageMemberships)) {
      foreach ($cf_user->pageMemberships as $key => $membership) {
        if ($membership->page->getId() != $page->getId()) {
          culturefeed_pages_set_active_page($page);
          break;
        }
      }
    }

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    form_set_error('', t('There was an error while deleting the page.'));
  }

  $form_state['redirect'] = '';

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
      <p>' . t('By publishing a page, the page will re-appear in the search results.') . '</p>
      <p>' . t('Members of this page, and activities in the name of this page will be recreated.') . '</p>
      <p><strong>' . t('Are you sure?') . '</strong></p>',
  );

  $form['wrapper']['publish'] = array(
    '#type' => 'submit',
    '#value' => t('Publish'),
    '#attributes' => array('class' => array('button-publish')),
  );

  $form['wrapper']['decline'] = array(
    '#type' => 'button',
    '#value' => t('Cancel'),
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
    DrupalCultureFeed::getLoggedInUserInstance()->pages()->publishPage($page->getId());
    drupal_set_message(format_string(t('Page "@page" is recreated.'), array('@page' => $page->getName())));
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    form_set_error('', t('There was an error while recreating the page.'));
  }

  $form_state['redirect'] = culturefeed_search_detail_url("page", $page->getId(), $page->getName());

}

/**
 * Page callback to show a list of all agenda activities for a page.
 */
function culturefeed_pages_page_events(CultureFeed_Cdb_Item_Page $page) {

  $curr_page = isset($_GET['page']) ? $_GET['page'] : 0;

  // Add start index (page number we want) + items per page
  $parameters[] = new \CultuurNet\Search\Parameter\Start($curr_page * CULTUREFEED_PAGES_ADMIN_EVENTS_COUNT);
  $parameters[] = new \CultuurNet\Search\Parameter\Rows(CULTUREFEED_PAGES_ADMIN_EVENTS_COUNT);

  // Add grouping so returned events are not duplicate.
  $parameters[] = new \CultuurNet\Search\Parameter\Group();

  // Search on events that have a relation to the page ID.
  $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery('type:event');
  $parameters[] = new \CultuurNet\Search\Parameter\Query($page->getId());

  // Add sorting.
  $sort_field = 'startdate';
  $direction = 'asc';
  if (isset($_GET['order'])) {

    $direction = $_GET['sort'];

    switch ($_GET['order']) {

      case t('Title'):
        $sort_field = 'title_sort';
      break;

      case t('Date'):
        $sort_field = 'startdate';
      break;

      case t('Reactions'):
        $sort_field = 'comment_count';
      break;

    }

  }

  $parameters[] = new \CultuurNet\Search\Parameter\Sort($sort_field, $direction);
  $service = culturefeed_get_search_service();

  // Execute search and show results.
  $build = array();
  try {

    $result = $service->search($parameters);
    pager_default_initialize($result->getTotalCount(), CULTUREFEED_PAGES_ADMIN_EVENTS_COUNT);

    $build['page'] = array(
      '#theme' => 'culturefeed_pages_events_admin_page',
      '#items' => $result->getItems(),
      '#page' => $page,
    );
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    $build['error'] = array('#markup' => t('There was an error while loading the activities.'));
  }

  culturefeed_pages_set_page_breadcrumb($page);

  return $build;

}

/**
 * Page callback to show a list of my culturefeed pages.
 */
function culturefeed_pages_page_my_pages() {

  drupal_add_library('system', 'drupal.ajax');

  $build = array();

  try {

    $cf_user = DrupalCultureFeed::getLoggedInUser();
    $cf_pages = DrupalCultureFeed::getConsumerInstance()->pages();

    // Build all the membership rows.
    $membership_page_ids = array();
    if (!empty($cf_user->pageMemberships)) {

      $memberships = $cf_user->pageMemberships;
      foreach ($memberships as $membership) {

        $is_page_admin = $membership->role == CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN;
        $admins_of_page = array();
        $path = culturefeed_search_detail_path('page', $membership->page->getId(), $membership->page->getName());

        if ($is_page_admin) {

          $admin_link = l(t('Administer page'), $path);

          // We need the full user list of every page, to check if the user is the only admin.
          try {
            $admin_list = $cf_pages->getUserList($membership->page->getId(), array(CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN), FALSE);
          }
          catch (Exception $e) {
            watchdog_exception('culturefeed_pages', $e);
            $admin_list = new CultureFeed_Pages_UserList();
          }

          foreach ($admin_list->memberships as $admin) {
            $admins_of_page[$admin->user->id] = $admin->user->id;
          }

        }
        else {

          // Show request admin link.
          $admin_link = '<div id="request_adminship_link_' . $membership->page->getId() . '_wrapper">' . l(t('Become an administrator'), 'pages/' . $membership->page->getId() . '/membership/request-admin/' . $cf_user->id . '/nojs',
            array('query' => drupal_get_destination(), 'attributes' => array('class' => array('use-ajax btn'), 'data-toggle' => 'tooltip', 'data-original-title' => t('This is a request sent to the administrator of this page.')))) . '</div>';

        }

        // If user is only admin, he can't delete his membership.
        if ($is_page_admin && count($admins_of_page) == 1) {
          $member_link = '<p class="muted">' . t('Not possible to remove') . ' <span href="#" data-toggle="tooltip" data-placement="top" title data-original-title="' . t('You\'re the only administrator of this page. You can not remove yourself as a member') . '"><i class="icon icon-question-sign"></i></span></p>';
        }
        else {
          $delete_options = array(
            'attributes' => array(
              'role' => 'button',
              'data-toggle' => 'modal',
              'data-target' => '#page_confirm',
              'data-remote' => url('pages/' . $membership->page->getId() . '/membership/delete/ajax'),
            ),
          );
          $member_link = l('Verwijder mij als lid', 'pages/' . $membership->page->getId() . '/membership/delete/nojs', $delete_options);
        }

        $row = array(
          l($membership->page->getName(), $path),
          format_date($membership->creationDate, 'custom', 'd/m/Y H:i'),
          $member_link,
          $admin_link
        );

        $membership_page_ids[] = $membership->page->getId();
        $rows[] = $row;

      }

      // Pages where I am a member.
      $build['member_pages'] = array(
        '#theme' => 'table',
        '#header' => array(t('Page'), t('Member since'), '', ''),
        '#rows' => $rows,
      );

    }

    // Build all the follow rows.
    if (!empty($cf_user->following)) {

      $followed_pages = $cf_user->following;

      $rows = array();
      foreach ($followed_pages as $followed_page) {

        $path = culturefeed_search_detail_path('page', $followed_page->page->getId(), $followed_page->page->getName());

        $follow_link = l(t('Unfollow'), 'culturefeed/pages/defollow/nojs/' . $followed_page->page->getId(), array('attributes' => array('class' => array('use-ajax'))));
        $become_member_link = t('You are a member of this page');
        if (!in_array($followed_page->page->getId(), $membership_page_ids)) {
          $become_member_link = l(t('Become a member'), 'culturefeed/pages/join/nojs/' . $followed_page->page->getId(), array('attributes' => array('class' => array('use-ajax'))));
        }

        if ($is_page_admin) {
          $admin_link = l(t('Administer page'), $path);
        }
        else {
          $admin_link = l(t('Send request to become administrator'), 'pages/' . $membership->page->getId() . '/membership/request-admin/' . $cf_user->id . '/nojs', array('query' => drupal_get_destination()));
        }

        $row = array(
          l($followed_page->page->getName(), $path),
          format_date($followed_page->creationDate, 'custom', 'd/m/Y H:i'),
          $follow_link,
          $become_member_link
        );
        $rows[] = $row;

      }

      // Pages I follow.
      $build['followed-pages'] = array(
        '#theme' => 'table',
        '#header' => array(t('Page'), t('Following since'), '', ''),
        '#rows' => $rows,
      );

    }

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
  }

  $build['search-pages'] = culturefeed_pages_join_page_search();

  return array(
    '#theme' => 'culturefeed_pages_page_my_pages',
    '#build' => $build,
    '#user' => $cf_user,
  );

}
