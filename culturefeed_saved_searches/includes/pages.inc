<?php

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
    '#default_value' => isset($params['title']) ? $params['title'] : '',
  );
  $form['save_search_notification_frequency'] = array(
    '#title' => t('Stuur UiTalerts bij nieuwe resultaten:'),
    '#type' => 'select',
    '#options' => array(
      'ASAP' => 'dagelijks overzicht',
      'DAILY' => 'wekelijks overzicht',
      'WEEKLY' => 'onmiddellijk',
      'NEVER' => 'nee, bedankt',
    ),
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
  $savedSearch->query = urlencode($params['query']);
  $savedSearch->frequency = $form_state['values']['save_search_notification_frequency'];

  $service = DrupalCultureFeed::getLoggedInUserInstance()->savedSearches()->subscribe($savedSearch);

  // Construct URL redirect with same search params
  // @question: do we need to set the facets in the URl as well when we redirect to search page?
  $form_state['redirect'] = current_path();

}