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

  return array(
    '#theme' => 'culturefeed_page',
    '#page' => $page,
  );

}

/**
 * Page callback to join a page.
 * @param string $request_type
 *   Request type. Ajax or nojs
 * @param CultureFeed_Cdb_Item_Page $page
 *   Page to add membership to.
 */
function culturefeed_pages_page_join($request_type, $page) {

  $success = FALSE;

  // Send request to join.
  try {
    $cf_pages = DrupalCultureFeed::getLoggedInUserInstance()->pages();
    $cf_pages->addMember($page->getId(), DrupalCultureFeed::getLoggedInUserId());
    $success = TRUE;
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
  }

  // Correct message.
  if ($success) {
    $message = format_string('U bent nu lid van de pagina @page.', array('@page' => $page->getName()));
  }
  else {
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
 * Page callback for the region suggestions autocomplete.
 */
function culturefeed_pages_region_suggestion_autocomplete_page($search_string) {

  $matches = array();
  
  if ($search_string) {
  
    try {
  
      // Get the list of suggestions from service.
      $suggestions = culturefeed_search_get_city_suggestions($search_string);
      foreach ($suggestions as $key => $suggestion) {
        $matches[$key] = check_plain($suggestion);
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
 * 1 > POST http://test.uitid.be:80/culturefeed/rest/page
linkBlog=http://www.denhemel.com/blog
linkWebsite=http://www.denhemel.com
linkYouTube=http://www.youtube.com/user/DenHemel
linkeFacebook=https://www.facebook.com/den.hemel
linkTwitter=https://twitter.com/DenHemel
linkGooglePlus=https://google.com/DenHemel 
linkTicketing=http://www.denhemel.com/tickets
contactInfoTel=+32 13 772931
contactInfoEmail=info@denhemel.com
categoryId=8.11.0.0.0
tagline=Pop-en rocktempel
 */
function culturefeed_pages_add_form() {
  
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
    '#options' => array(),
    '#default_value' => 0,
    '#weight' => -4,
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => 'Beschrijving',
    '#default_value' => '',
    '#maxlength' => 250,
    '#weight' => -3,
  );

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

  $form['visible'] = array(
    '#type' => 'hidden',
    '#value' => TRUE,
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Opslaan',
    '#weight' => 10,
  );
  
  $form['#submit'] = array('culturefeed_pages_add_form_submit');
  
  return $form;
  
}

/**
 * Submit handler to create a culturefeed page.
 * @param array $form
 * @param array $form_state
 */
function culturefeed_pages_add_form_submit(& $form, & $form_state) {
  dsm('real form submit');
}
