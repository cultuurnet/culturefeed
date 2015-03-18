<?php
/**
 * @file
 * Page callback for culturefeed pages.
 */
use \CultuurNet\Search\Parameter;

/**
 * Page where users can search for a pag and join them.
 */
function culturefeed_pages_page_basic_search() {

  $form = drupal_get_form('culturefeed_pages_basic_search_form');

  $items = array();
  $total_results = 0;
  if (isset($_GET['search'])) {

    try {

      $current_page = isset($_GET['page']) ? $_GET['page'] : 0;
      $actortypes = variable_get('culturefeed_pages_actor_types', array());

      // Prepare multiple search terms for better match
      $search_terms = str_replace('-', ' ', $_GET['search']);
      $search_terms = explode(' ', $search_terms);
      foreach ($search_terms as $search_term) {
        $search_query[] = '(' . $search_term . ' OR *' . $search_term . '*)' ;
      }
      $search_query = implode(' AND ', $search_query);

      $parameters = array();
      $parameters[] = new Parameter\FilterQuery('category_id' . ':(' . implode(' OR ', $actortypes) . ')');
      $parameters[] = new Parameter\Query($search_query);
      $parameters[] = new Parameter\Sort('title_sort', Parameter\Sort::DIRECTION_ASC);
      $parameters[] = new Parameter\Start($current_page * CULTUREFEED_PAGES_BASIC_ITEMS_PER_PAGE);
      $parameters[] = new Parameter\Rows(CULTUREFEED_PAGES_BASIC_ITEMS_PER_PAGE);


      $result = culturefeed_get_search_service()->searchPages($parameters);
      $items = $result->getItems();
      $total_results = $result->getTotalCount();
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

  pager_default_initialize($total_results, CULTUREFEED_PAGES_BASIC_ITEMS_PER_PAGE);

  $build = array();
  $build['page'] = array(
    '#theme' => 'culturefeed_pages_basic_search_page',
    '#form' => $form,
    '#results' => $items,
    '#total_results' => $total_results,
    '#search' => isset($_GET['search']) ? $_GET['search'] : NULL,
  );

  $build['pager'] = array(
    '#theme' => 'pager',
    '#quantity' => 5
  );

  return $build;
}

/**
 * Form callback for the basic search form.
 */
function culturefeed_pages_basic_search_form() {

  $form = array();

  $form['page'] = array(
    '#type' => 'textfield',
    '#title' => t('Term'),
    '#autocomplete_path' => 'ajax/culturefeed/pages/page-suggestion',
    '#default_value' => isset($_GET['search']) ? $_GET['search'] : '',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  return $form;

}

/**
 * Submit the basic search form.
 */
function culturefeed_pages_basic_search_form_submit($form, &$form_state) {

  $form_state['redirect'] = array(
    $_GET['q'],
    array('query' => array('search' => $form_state['values']['page'])),
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
function culturefeed_pages_page_suggestion_autocomplete_page($search_string, $return_ids = FALSE) {

  $matches = array();
  if ($search_string) {

    try {

      $actortypes = variable_get('culturefeed_pages_actor_types', array());

      $parameters = array();
      $parameters[] = new Parameter\FilterQuery('category_actortype_id' . ':(' . implode(' OR ', $actortypes) . ')');

      // Get the list of suggestions from service.
      $suggestions = culturefeed_get_search_service()->searchSuggestions($search_string, array('page'), FALSE, $parameters);
      if ($suggestions->hasSuggestions()) {
        foreach ($suggestions as $suggestion) {
          $page = culturefeed_pages_page_load($suggestion->getCdbid());
          if ($return_ids) {
            $matches[$suggestion->getCdbid()] = check_plain($suggestion->getTitle());
          } else {
            $matches[$suggestion->getTitle()] = check_plain($suggestion->getTitle());
          }
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

  $query = drupal_get_query_parameters();
  $page_name = !empty($query['search']) ? $query['search'] : '';

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $page_name,
    '#weight' => -7,
  );

  $form['tagline'] = array(
    '#type' => 'textfield',
    '#title' => t('Baseline'),
    '#default_value' => '',
    '#weight' => -6,
  );

  $form['cover'] = array(
    '#type' => 'managed_file',
    '#title' => t('Cover Photo'),
    '#description' => t('Allowed extensions: jpg, jpeg, gif or png'),
    '#size' => 26,
    '#default_value' => '',
    '#weight' => -5,
    '#process' => array('file_managed_file_process', 'culturefeed_image_file_process'),
    '#upload_validators' => array(
      'file_validate_extensions' => array('jpg jpeg png gif'),
    ),
    '#upload_location' => 'public://pages',
  );

  $actortypes = culturefeed_search_get_actortype_categories();
  $available_actortypes = variable_get('culturefeed_pages_actor_types', array());
  $options = empty($available_actortypes) ? $actortypes : array_intersect_key($actortypes, $available_actortypes);

  $form['categoryId'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#options' => $options,
    // Pages can have multiple actortypes but we need to fix this in addPage and updatePage first
    // '#multiple' => TRUE,
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
    '#title' => t('Street and number'),
    '#default_value' => '',
    '#weight' => 1,
  );

  $form['zip'] = array(
    '#type' => 'textfield',
    '#title' => t('Zipcode'),
    '#default_value' => '',
    '#weight' => 2,
  );

  $form['city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#default_value' => '',
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
    '#type' => 'managed_file',
    '#title' => t('Picture or logo'),
    '#description' => t('Allowed extensions: jpg, jpeg, gif or png'),
    '#size' => 26,
    '#default_value' => '',
    '#weight' => 16,
    '#process' => array('file_managed_file_process', 'culturefeed_image_file_process'),
    '#upload_validators' => array(
      'file_validate_extensions' => array('jpg jpeg png gif'),
    ),
    '#upload_location' => 'public://pages',
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
  }

  if (strlen($form_state['values']['description']) > 400) {
    form_set_error('description', t('The maximum is 400 characters'));
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
  $errors = form_get_errors();
  if ($errors) {
    return;
  }

  // Local storage of the universal id.
  $page_id = NULL;

  // Send request to add a page.
  try {
    /** @var CultureFeed_Pages $cf_pages */
    $cf_pages = DrupalCultureFeed::getLoggedInUserInstance()->pages();
    $page_id = $cf_pages->addPage($form_state['post_params']);

    // Create activity for the page creation.
    $activity = new CultureFeed_Activity();
    $activity->nodeId = $page_id;
    $activity->nodeTitle = $form_state['values']['name'];
    $activity->contentType = CultureFeed_Activity::CONTENT_TYPE_CULTUREFEED_PAGE;
    $activity->type = CultureFeed_Activity::TYPE_PAGE_CREATED;
    $activity->userId = DrupalCultureFeed::getLoggedInUserId();
    DrupalCultureFeed::createActivity($activity);

    drupal_set_message(format_string(t('Page "@page" has been created.'), array('@page' => $form_state['values']['name'])));
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    form_set_error('', t('There was an error while creating the page.'));
  }

  if ($page_id) {

    $form_state['page_id'] = $page_id;
    $form_state['page_title'] = $form_state['values']['name'];

    // Upload Page image.
    if (!empty($form_state['values']['image'])) {

      $file = file_load($form_state['values']['image']);
      if ($file) {
        try {
          $file_upload = culturefeed_prepare_curl_upload_from_file($file);
          $cf_pages->addImage($page_id, array('image' => $file_upload));
        }
        catch (Exception $e) {
          watchdog_exception('culturefeed_pages', $e);
          drupal_set_message(t('An error occurred when uploading your image.'), 'error');
        }
      }
    }

    // Upload Page cover.
    if (!empty($form_state['values']['cover'])) {

      $file_cover = file_load($form_state['values']['cover']);
      if ($file_cover) {
        try {
          $file_upload = culturefeed_prepare_curl_upload_from_file($file_cover);
          $cf_pages->addCover($page_id, array('image' => $file_upload));
        }
        catch (Exception $e) {
          watchdog_exception('culturefeed_pages', $e);
          drupal_set_message(t('An error occurred when uploading your cover.'), 'error');
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
  drupal_goto('pages/switch/' . $form_state['page_id']);

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
    '#markup' => culturefeed_search_detail_l("page", $page->getId(), $page->getName(), t('View page')),
    '#weight' => -25,
  );

  // Hidden page ID.
  $form['pageId'] = array(
    '#type' => 'hidden',
    '#value' => $page->getId(),
  );

  $form['name']['#default_value'] = $page->getName();
  $form['description']['#default_value'] = $page->getDescription();
  $form['categoryId']['#default_value'] = $page->getCategories();

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
  $form_state['#old_image'] = 0;
  if (!empty($image)) {

    // Create temp file to preview the external image.
    $file = culturefeed_create_temporary_image($image, file_default_scheme() . '://pages');
    if ($file) {
      $form_state['#old_image'] = $file->fid;
      $form['image']['#default_value'] = $file->fid;
    }

    $form['image']['#title'] = t('Select another Image or Logo');

  }

  // The cover.
  $cover = $page->getCover();
  $form_state['#old_cover'] = 0;
  if (!empty($cover)) {

    // Creat temp file to preview the external cover.
    $file_cover = culturefeed_create_temporary_image($cover, file_default_scheme() . '://pages');
    if ($file_cover) {
      $form_state['#old_cover'] = $file_cover->fid;
      $form['cover']['#default_value'] = $file_cover->fid;
    }

    $form['cover']['#title'] = t('Select another Cover Photo');
  }

  // Baseline
  $tagline = $page->getTagline();
  if (!empty($tagline)) {
    $form['tagline']['#default_value'] = isset($tagline) ? $tagline : '';
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
    if (empty($form_state['values']['image']) && $form_state['#old_image'] > 0) {
      try {
        $cf_pages->removeImage($uid);
      }
      catch (Exception $e) {
        watchdog_exception('culturefeed_pages', $e);
      }
    }

    // Upload new page image.
    if ($form_state['values']['image'] && $form_state['#old_image'] != $form_state['values']['image']) {

      $file = file_load($form_state['values']['image']);
      if ($file) {
        $file_upload = culturefeed_prepare_curl_upload_from_file($file);
        try {
          $cf_pages->addImage($uid, array('image' => $file_upload));
        }
        catch (Exception $e) {
          watchdog_exception('culturefeed_pages', $e);
          drupal_set_message(t('An error occurred when uploading your image.'), 'error');
        }
      }
    }

    // Remove the cover if requested.
    if (empty($form_state['values']['cover']) && $form_state['#old_cover'] > 0) {
      try {
        $cf_pages->removeCover($uid);
      }
      catch (Exception $e) {
        watchdog_exception('culturefeed_pages', $e);
      }
    }

    // Upload new page cover.
    if ($form_state['values']['cover'] && $form_state['#old_cover'] != $form_state['values']['cover']) {

      $file_cover = file_load($form_state['values']['cover']);
      if ($file_cover) {
        try {
          $file_upload = culturefeed_prepare_curl_upload_from_file($file_cover);
          $cf_pages->addCover($uid, array('image' => $file_upload));
        }
        catch (Exception $e) {
          watchdog_exception('culturefeed_pages', $e);
          drupal_set_message(t('An error occurred when uploading/editing your cover.'), 'error');
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
    '#markup' => '<h2>' . t('Features') . '</h2>',
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
    '#title' => t('Allow users to follow my page'),
    '#description' => t('Followers receive a notification when your page is updated, so that they stay informed of new activities, threads, ...'),
    '#default_value' => !empty($permissions->allowFollowers),
  );

  $form['allow_members'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow users to request membership'),
    '#description' => t('Members can, depending on the roles or rights, collaborate to maintain and update your page. Furthermore, page memberships are added to user profiles and can be used as an alias to add comments.'),
    '#default_value' => !empty($permissions->allowMembers),
  );

  $form['allow_comments'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow users to add comments to my activities'),
    '#description' => t('Only available for organizations who added their activities via www.uitdatabank.be.'),
    '#default_value' => !empty($permissions->allowComments),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  if ($page->isVisible()) {
    $form['remove_link'] = array(
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
    /*$form['publish_link'] = array(
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
      <p>' . t('By deleting a page, the page will no longer appear in search results and all contact info will be removed.') . '</p>
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

  $form_state['redirect'] = '';
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
          culturefeed_pages_set_active_page($membership->page);
          break;
        }
      }
    }
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    return;
  }

  // Also remove the activity for it.
  try {

    $query = new CultureFeed_SearchActivitiesQuery();
    $query->max = 1;
    $query->type = CultureFeed_Activity::TYPE_PAGE_CREATED;
    $query->userId = $cf_user->id;
    $query->nodeId = $page->getId();
    $activities = DrupalCultureFeed::searchActivities($query);
    if ($activities && $activities->total > 0) {
      DrupalCultureFeed::deleteActivity($activities->objects[0]->id);
    }
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
  }

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
  $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery('type:event OR type:production');
  $parameters[] = new \CultuurNet\Search\Parameter\Query('"'.$page->getId().'"');

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
 * Page callback to show an admin overview that contains all the news
 * for given page.
 */
function culturefeed_pages_page_manage_news(CultureFeed_Cdb_Item_Page $page) {

  $curr_page = isset($_GET['page']) ? $_GET['page'] : 0;

  $query = new CultureFeed_SearchActivitiesQuery();
  $query->max = CULTUREFEED_PAGES_ADMIN_NEWS_COUNT;
  $query->start = $curr_page * $query->max;
  $query->type = CultureFeed_Activity::TYPE_NEWS;
  $query->consumer = variable_get('culturefeed_api_application_key', '');
  $query->nodeId = $page->getId();
  $query->skipCache = TRUE;

  // Execute search and show results.
  $build = array();
  try {

    $result = DrupalCultureFeed::searchActivities($query);
    pager_default_initialize($result->total, CULTUREFEED_PAGES_ADMIN_NEWS_COUNT);

    $build['page'] = array(
      '#theme' => 'culturefeed_pages_news_admin_page',
      '#results' => $result->objects,
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
 * Form callback to add a news item.
 */
function culturefeed_pages_add_news_form($form, &$form_state, CultureFeed_Cdb_Item_Page $page) {

  $form = array();

  $form_state['page'] = $page;

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#required' => TRUE,
  );

  $form['image'] = array(
    '#type' => 'managed_file',
    '#title' => t('Image'),
    '#description' => t('Allowed extensions: jpg, jpeg, gif or png'),
    '#size' => 26,
    '#default_value' => '',
    '#process' => array('file_managed_file_process', 'culturefeed_image_file_process'),
    '#upload_validators' => array(
      'file_validate_extensions' => array('jpg jpeg png gif'),
    ),
    '#upload_location' => 'public://news',
  );

  $form['body'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Publish'),
  );

  return $form;
}

/**
 * Validate the news form. Create the activity.
 */
function culturefeed_pages_add_news_form_validate($form, $form_state) {

  // Don't create if other validations had errors.
  $errors = form_get_errors();
  if ($errors) {
    return;
  }

  $activity = new CultureFeed_Activity();
  $activity->type = CultureFeed_Activity::TYPE_NEWS;
  $activity->userId = DrupalCultureFeed::getLoggedInUserId();
  $activity->onBehalfOf = $form_state['page']->getId();
  $activity->contentType = 'page';
  $activity->nodeId = $form_state['page']->getId();
  $activity->nodeTitle = $form_state['page']->getName();

  $value = new stdClass();
  $value->title = $form_state['values']['title'];
  $value->body = $form_state['values']['body'];

  if (!empty($form_state['values']['image'])) {
    $file = file_load($form_state['values']['image']);
    $value->image = file_create_url($file->uri);
  }

  $activity->value = json_encode($value);

  try {
    DrupalCultureFeed::createActivity($activity);
    // If activity created we need to change the status of the file to permanent
    if (!empty($form_state['values']['image'])) {
      $file = file_load($form_state['values']['image']);
      $file->status = 1;
      file_save($file);
    }
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    form_set_error('', t('There was an error while creating the news item'));
  }

}

/**
 * Submit the 'add news' form, redirect to overview.
 */
function culturefeed_pages_add_news_form_submit($form, &$form_state) {
  drupal_set_message(t('The news item %title was created', array('%title' => $form_state['values']['title'])));
  $form_state['redirect'] = 'pages/' . $form_state['page']->getId() . '/news';
}

/**
 * Form callback to show a confirmation form for deleting a news item.
 */
function culturefeed_pages_delete_news_form($form, &$form_state, $page, $news_item) {

  // Real title is stored in the json.
  $title = $news_item->nodeTitle;
  $content = json_decode($news_item->value);
  if ($content && !empty($content->title)) {
    $title = $content->title;
  }

  $form_state['news'] = $news_item;
  $form_state['title'] = $title;
  $form_state['page'] = $page;

  return confirm_form($form, t('Are you sure you want to delete %title?', array('%title' => $title)), 'pages/' . $page->getId() . '/news', '<p>' . t('This action cannot be undone.') . '</p>', t('Delete'), t('Cancel')
  );
}

/**
 * Submit the confirmation form. Delete the news item.
 */
function culturefeed_pages_delete_news_form_submit($form, &$form_state) {

  if ($form_state['values']['confirm']) {

    try {
      DrupalCultureFeed::deleteActivity($form_state['news']->id);
      $form_state['redirect'] = 'pages/' . $form_state['page']->getId() . '/news';
      drupal_set_message(t('News item %title has been deleted.', array('%title' => $form_state['title'])));
    }
    catch (Exception $e) {
      drupal_set_message(t('There was an error while deleting your news item.', array('%title' => $form_state['title'])));
      watchdog_exception('culturefeed_pages', $e);
    }

  }

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

        // We need the full user list of every page, to check if the user is the only admin.
        if ($is_page_admin) {
          $admin_link = l(t('Administer page'), $path);
          $admins_of_page = culturefeed_pages_get_admins_of_page($membership->page);
        }
        else {
          // Show request admin link.
          $admin_link = theme('culturefeed_pages_admin_request_link', array(
            'page' => $membership->page,
            'cf_user' => $cf_user,
          ));
        }

        // If user is only admin, he can't delete his membership.
        if ($is_page_admin && count($admins_of_page) == 1) {
          $member_link = theme('culturefeed_pages_membership_delete_not_possible', array('page' => $membership->page));
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
          $member_link = l(t('Remove membership'), 'pages/' . $membership->page->getId() . '/membership/delete/nojs', $delete_options);
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

  $build['search-pages'] = culturefeed_pages_page_basic_search();

  return array(
    '#theme' => 'culturefeed_pages_page_my_pages',
    '#build' => $build,
    '#user' => $cf_user,
  );

}

/**
 * Ajax callback to get the agenda of a page.
 */
function culturefeed_pages_page_ajax_get_agenda(CultureFeed_Cdb_Item_Page $page) {

  $requested_page = isset($_GET['page']) ? $_GET['page'] : 0;
  $max_results = variable_get('pages_block_agenda_total_items', 5) * ($requested_page + 1);

  // Execute search and show results.
  $result = culturefeed_pages_get_activities_from_page($page, $max_results);
  if (!$result || $result->getTotalCount() == 0) {
    return;
  }

  // Render the results.
  $search_results = $result->getItems();
  $items = array();
  foreach ($search_results as $search_result) {
    $items[] = theme('culturefeed_' . $search_result->getType() . '_mini_summary', array('item' => $search_result));
  }

  $build = array();
  $build['list'] = array(
    '#theme' => 'culturefeed_pages_page_agenda',
    '#items' => $items,
    '#page' => $page,
    '#class' => 'page-activities',
  );

  if ($result->getTotalCount() > $max_results) {
    $build['list']['#read_more_path'] = 'culturefeed/ajax/page/agenda/' . $page->getId();
  }

  $commands = array(ajax_command_replace('#page-agenda-ajax-wrapper-' . $page->getId(), render($build)));

  ajax_deliver(array('#type' => 'ajax', '#commands' => $commands));
}

