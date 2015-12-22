<?php
/**
 * @file
 * Page callbacks for the saved searches module.
 */

/**
 * Save searches form.
 */
function culturefeed_saved_searches_save_search_form() {

  $params = drupal_get_query_parameters();
  if (!isset($params['query'])) {
    drupal_set_message(t('No query to save was given'), 'error');
    drupal_goto();
  }
  
  // Encode double quotes for back end  
  $query_parts = explode('&', $params['query']);
  $parsed_array = array();
  foreach ($query_parts as $query_part) {
    if (substr($query_part, 0, 1) === 'q') {
      $query_part = str_replace('"', '%2522', $query_part);
    }
    $parsed_array[] = $query_part;
  } 
  $params['query'] = implode($parsed_array, '&');

  $form = array();
  $form['save_search_title'] = array(
    '#type' => 'textarea',
    '#rows' => 2,
    '#resizable' => FALSE,
    '#required' => TRUE,
    '#default_value' => isset($params['title']) ? $params['title'] : '',
  );
  $form['save_search_notification_frequency'] = array(
    '#title' => t('Send mailalert with new results:'),
    '#type' => 'select',
    '#options' => culturefeed_saved_searches_frequency_options(),
    '#default_value' => 'DAILY',
  );

  $cf_account = DrupalCultureFeed::getLoggedInUser();
  $form['save_search_email'] = array(
    '#type' => 'container',
    '#states' => array(
      'invisible' => array(
        ':input[name="save_search_notification_frequency"]' => array('value' => 'NEVER'),
      ),
    ),
  );
  $form['save_search_email']['pre'] = array(
    '#type' => 'item',
    '#markup' => '<span>' . t('Mailalerts will be sent to:') .'</span>',
  );
  $form['save_search_email']['email'] = array(
    '#type' => 'item',
    '#title' => $cf_account->mbox,
    '#markup' => '<span>' . l(t('Change'), 'culturefeed/account/edit') .'</span>',
  );

  $form['save_search_info'] = array(
    '#type' => 'item',
    '#markup' => '<ul><li>' . t('We won\'t send emails when there are no new results') . '</li><li>' . t('You can unsubscribe for this service in any mail you will receive') . '</li></ul>',
    '#states' => array(
      'invisible' => array(
        ':input[name="save_search_notification_frequency"]' => array('value' => 'NEVER'),
      ),
    ),
  );

  $form['query'] = array(
    '#type' => 'value',
    '#value' => $params['query'],
  );

  $form['save_search_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  // Show the date tooltip only if filter on date was set.
  if (isset($params['datefilter'])) {
    $form['save_search_date_info'] = array(
      '#type' => 'item',
      '#markup' => t('We will send a mailalert as soon as there are new results for your search. Therefore we will ignore any date filters in your search'),
      '#states' => array(
        'invisible' => array(
          ':input[name="save_search_notification_frequency"]' => array('value' => 'NEVER'),
        ),
      ),
    );
  }

  return $form;
}

/**
 * Submit handler for the save search-form.
 *
 * @param type $form
 * @param type $form_state
 */
function culturefeed_saved_searches_save_search_form_submit($form, &$form_state) {

  $cf_account = DrupalCultureFeed::getLoggedInUser();
  $savedSearch = new CultureFeed_SavedSearches_SavedSearch();

  $savedSearch->userId = $cf_account->id;
  $savedSearch->name = $form_state['values']['save_search_title'];
  $savedSearch->query = $form_state['values']['query'] . '&group=true&rows=100&sort=creationdate desc';
  $savedSearch->frequency = $form_state['values']['save_search_notification_frequency'];

  try {
    DrupalCultureFeed::getLoggedInUserInstance()->savedSearches()->subscribe($savedSearch);
    drupal_set_message(t('You search has been saved'));
  }
  catch (Exception $e) {
    watchdog_exception('saved_searches', $e);
    drupal_set_message(t('There was an error while saving your search. Please try again.'), 'error');
  }
  
  // Remove %2522 encoding
  $savedSearch->query = str_replace('%2522', '"', $savedSearch->query);

  // Construct URL redirect with same search params.
  $form_state['redirect'] = array('agenda/search', array('query' => culturefeed_search_get_search_url_query($savedSearch->query)));

}

/**
 * Page callback to show the management listing of all saved searches of current user.
 */
function culturefeed_saved_searches_page_my_searches() {

  $build = array();

  try {

    $build['list'] = array(
      '#theme' => 'culturefeed_saved_searches_manage_list',
      '#searches' => DrupalCultureFeed::getLoggedInUserInstance()->savedSearches()->getList(),
    );

  }
  catch (Exception $e) {
    drupal_set_message(t('A problem occurred while loading your searches. Please try again later.'), 'error');
  }

  return $build;

}

/**
 * Page callback to view a given search.
 */
function culturefeed_saved_searches_redirect(CultureFeed_SavedSearches_SavedSearch $search) {
  drupal_goto('agenda/search', array('query' => culturefeed_search_get_search_url_query($search->query)));
}

/**
 * Form callback to delete a given search.
 */
function culturefeed_saved_searches_delete_form($form, &$form_state, CultureFeed_SavedSearches_SavedSearch $search) {
  $form_state['search'] = $search;
  return confirm_form($form, t('Are you sure you want to delete this search?'), 'culturefeed/searches', '<p>' . t('This action cannot be undone.') . '</p>', t('Delete'), t('Cancel'));
}

/**
 * Submit the delete form.
 */
function culturefeed_saved_searches_delete_form_submit($form, &$form_state) {

  try {
    DrupalCultureFeed::getLoggedInUserInstance()->savedSearches()->unsubscribe($form_state['search']->id, DrupalCultureFeed::getLoggedInUserId());
    $form_state['redirect'] = 'culturefeed/searches';
    drupal_set_message(t('Your search has been deleted'));
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_saved_searches', $e);
    drupal_set_message(t('A problem occurred while deleting your search. Please try again later.'), 'error');
  }

}
