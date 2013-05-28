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

      $searchService = culturefeed_get_search_service();
      $parameters = array();
      $parameters[] = new \CultuurNet\Search\Parameter\Query($_GET['page']);
      $result = $searchService->searchPages($parameters);
      $items = $result->getItems();

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
    '#items' => $items,
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
    $message = format_string('U bent nu lid van de pagina @page.', array('@page' => $page->getName()));
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
      $message = format_string('Het is niet toegelaten om lid te worden van @page. Gelieve een beheerder te contacteren om toch lid te worden.', array('@page' => $page->getName()));
    }
    else {
      $message = format_string('U kon niet toegevoegd worden aan @page.', array('@page' => $page->getName()));
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
    $message = format_string('U kon niet toegevoegd worden aan @page.', array('@page' => $page->getName()));
  }

  // Deliver as ajax callback or normal page.
  if ($request_type != 'ajax') {
    drupal_set_message($message);
    drupal_goto();
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
    '#description' => 'Maximum 400 karakters',
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
    '#title' => 'Telefoonnummer',
    '#default_value' => '',
    '#weight' => 10,
  );

  $form['contactInfoEmail'] = array(
    '#type' => 'textfield',
    '#title' => 'E-mailadres',
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
    '#title' => 'Facebook',
    '#default_value' => '',
  );

  $form['websites']['linkTwitter'] = array(
    '#type' => 'textfield',
    '#title' => 'Twitter',
    '#default_value' => '',
  );

  $form['websites']['linkGooglePlus'] = array(
    '#type' => 'textfield',
    '#title' => 'Google+',
    '#default_value' => '',
  );

  $form['websites']['linkYouTube'] = array(
    '#type' => 'textfield',
    '#title' => 'YouTube',
    '#default_value' => '',
  );

  $form['websites']['linkBlog'] = array(
    '#type' => 'textfield',
    '#title' => 'Blog',
    '#default_value' => '',
  );

  $form['image'] = array(
    '#type' => 'file',
    '#title' => 'Afbeelding of logo',
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

  if (strlen($form_state['values']['description']) > 400) {
    form_set_error('description', 'Het maximum karakters bedraagt 400');
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
    '#description' => 'Op die manier wordt je pagina ook toegankelijk via de profielen, reacties en reacties van alle collega\'s die actief zijn op deze site.',
    '#default_value' => !empty($permissions->allowMembers),
  );

  $form['allow_comments'] = array(
    '#type' => 'checkbox',
    '#title' => 'reacties schrijven over mijn activiteiten',
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
    drupal_set_message(format_string('De pagina instellingen zijn bewaard.'));
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    form_set_error('', 'Er is een fout opgetreden tijdens het bewaren van de pagina instellingen.');
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
    DrupalCultureFeed::getLoggedInUserInstance()->pages()->publishPage($page->getId());
    drupal_set_message(format_string('De pagina "@page" is hersteld.', array('@page' => $page->getName())));
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    form_set_error('', 'Er is een fout opgetreden tijdens het herstellen van de pagina.');
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

      case 'Titel':
        $sort_field = 'title_sort';
      break;

      case 'Datum':
        $sort_field = 'startdate';
      break;

      case 'Reacties':
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
    $build['error'] = array('#markup' => "Er is een fout opgetreden tijdens het laden van de activiteiten.");
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

          $admin_link = l('Pagina beheren', $path);

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
          $admin_link = '<div id="request_adminship_link_' . $membership->page->getId() . '_wrapper">' . l('Beheerder worden', 'pages/' . $membership->page->getId() . '/membership/request-admin/' . $cf_user->id . '/nojs',
            array('query' => drupal_get_destination(), 'attributes' => array('class' => array('use-ajax btn'), 'data-toggle' => 'tooltip', 'data-original-title' => 'Hiermee wordt een aanvraag gestuurd naar de huidige beheerder van deze pagina.'))) . '</div>';

        }

        // If user is only admin, he can't delete his membership.
        if ($is_page_admin && count($admins_of_page) == 1) {
          $member_link = '<p class="muted">Verwijderen niet mogelijk <span href="#" data-toggle="tooltip" data-placement="top" title data-original-title="Je bent de enige beheerder van deze pagina. Je kan jezelf niet verwijderen als lid"><i class="icon icon-question-sign"></i></span></p>';
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
        '#header' => array('Pagina', 'Lid sinds', '', ''),
        '#rows' => $rows,
      );

    }

    // Build all the follow rows.
    if (!empty($cf_user->following)) {

      $followed_pages = $cf_user->following;

      $rows = array();
      foreach ($followed_pages as $followed_page) {

        $path = culturefeed_search_detail_path('page', $followed_page->page->getId(), $followed_page->page->getName());

        $follow_link = l('Niet meer volgen', 'culturefeed/pages/defollow/nojs/' . $followed_page->page->getId(), array('attributes' => array('class' => array('use-ajax'))));
        $become_member_link = 'Je bent lid van deze pagina';
        if (!in_array($followed_page->page->getId(), $membership_page_ids)) {
          $become_member_link = l('Lid worden', 'culturefeed/pages/join/nojs/' . $followed_page->page->getId(), array('attributes' => array('class' => array('use-ajax'))));
        }

        if ($is_page_admin) {
          $admin_link = l('Pagina beheren', $path);
        }
        else {
          $admin_link = l('Stuur aanvraag om beheerder te worden', 'pages/' . $membership->page->getId() . '/membership/request-admin/' . $cf_user->id . '/nojs', array('query' => drupal_get_destination()));
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
        '#header' => array('Pagina', 'Volgt sinds', '', ''),
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
