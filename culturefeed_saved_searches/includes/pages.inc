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

  $form = array();
  $form['save_search_title'] = array(
    '#type' => 'textarea',
    '#rows' => 2,
    '#resizable' => FALSE,
    '#required' => TRUE,
    '#default_value' => isset($params['title']) ? $params['title'] : '',
  );
  $form['save_search_notification_frequency'] = array(
    '#title' => t('Stuur UiTalerts bij nieuwe resultaten:'),
    '#type' => 'select',
    '#options' => culturefeed_saved_searches_frequency_options(),
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
    '#markup' => '<span>' . t('UiTalerts worden verstuurd naar:') .'</span>',
  );
  $form['save_search_email']['email'] = array(
    '#type' => 'item',
    '#title' => $cf_account->mbox,
    '#markup' => '<span>' . l('change', 'culturefeed/profile/edit') .'</span>',
  );

  $form['save_search_info'] = array(
    '#type' => 'item',
    '#markup' => '<ul><li>' . t('Als er geen nieuwe resultaten zijn, sturen we geen UiTalert') . '</li><li>' . t('Je kan je via elke UiTalert uitschrijven voor deze dienst') . '</li></ul>',
    '#states' => array(
      'invisible' => array(
        ':input[name="save_search_notification_frequency"]' => array('value' => 'NEVER'),
      ),
    ),
  );

  $form['save_search_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Bewaar'),
  );

  // Show the date tooltip only if filter on date was set.
  if (isset($params['datefilter'])) {
    $form['save_search_date_info'] = array(
      '#type' => 'item',
      '#markup' => t('We sturen een UiTalert zodra er een nieuwe activiteit verschijnt binnen je bewaarde zoekopdracht. Hierbij wordt geen rekening gehouden met de datumfilter in je zoekopdracht.'),
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
  $params = drupal_get_query_parameters();

  $savedSearch->userId = $cf_account->id;
  $savedSearch->name = $params['title'];
  $savedSearch->query = $params['query'];
  $savedSearch->frequency = $form_state['values']['save_search_notification_frequency'];

  $service = DrupalCultureFeed::getLoggedInUserInstance()->savedSearches()->subscribe($savedSearch);

  // Construct URL redirect with same search params.
  $form_state['redirect'] = array('agenda/search', array('query' => culturefeed_saved_search_get_search_url_query($savedSearch)));

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
