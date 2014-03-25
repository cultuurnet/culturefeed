<?php

/**
 * @file
 * Page callback functions for culturefeed_mailing module.
 */

/**
 * Page callback to toggle newsletter optin
 */
function culturefeed_mailing_toggle_optin($mailing_id, $optin, $type = 'ajax') {

  $success = TRUE;

  try {

    $account = DrupalCultureFeed::getLoggedInUser();
    if (!$account) {
      return NULL;
    }

    if ($optin == '1') {
      culturefeed_mailing_subscribe_user($account, $mailing_id);
    }
    else {
      DrupalCultureFeed::unsubscribeFromMailing($account->id, $mailing_id);
    }

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_mailing', $e);
    $success = FALSE;
    return;
  }

  if ($type == 'ajax') {
    $commands = array();

    if ($success) {
      $data = _culturefeed_mailing_optin_toggle($mailing_id);
      $commands[] = ajax_command_html('#newsletter-optin-' . $mailing_id, $data);
    }

    ajax_deliver(array('#type' => 'ajax', '#commands' => $commands));
  }
  else {
    if (!$success) {
      drupal_set_message(t('Error occurred'), 'error');
    }
    drupal_goto();
  }
}

/**
 * Form callback for the mailing subscribe form.
 */
function culturefeed_mailing_subscribe_form() {

  try {
    $account = DrupalCultureFeed::getLoggedInUser();
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    return;
  }

  // Show nothing if not logged in, or no mailing list available.
  if (!$account || !(variable_get('culturefeed_mailing_list', ''))) {
    return;
  }

  $form = array();
  $form['mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Email address'),
    '#required' => TRUE,
    '#size' => 40,
    '#maxlength' => 255,
    '#default_value' => (!empty($_GET['email'])) ? $_GET['email'] : $account->mbox,
    '#weight' => -10,
  );

  $form['zip'] = array(
    '#type' => 'textfield',
    '#title' => t('Zipcode'),
    '#required' => TRUE,
    '#size' => 40,
    '#maxlength' => 255,
    '#default_value' => $account->zip,
    '#weight' => -9,
  );

  $form['firstname'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#required' => TRUE,
    '#size' => 40,
    '#maxlength' => 255,
    '#default_value' => $account->givenName,
    '#weight' => -8,
  );

  $form['lastname'] = array(
    '#type' => 'textfield',
    '#title' => t('Family name'),
    '#required' => TRUE,
    '#size' => 40,
    '#maxlength' => 255,
    '#default_value' => isset($account->familyName) ? $account->familyName : '',
    '#weight' => -7,
  );

  if (variable_get('culturefeed_mailing_list_optional', '')) {
    $form['optional_mailing'] = array(
      '#type' => 'checkbox',
      '#title' => variable_get('culturefeed_mailing_optional_description', ''),
    );
  }

  $form['privacy'] = array(
    '#type' => 'markup',
    '#value' => '<div id="privacy">' . l(t('Privacy statement'), 'privacyverklaring') . '</div>',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Subscribe'),
    '#weight' => 10
  );

  return $form;

}

/**
 * Submit the mailing subscription form.
 */
function culturefeed_mailing_subscribe_form_submit($form, &$form_state) {

  try {

    $account = DrupalCultureFeed::getLoggedInUser();

    // Subscribe to normal mailing.
    culturefeed_mailing_subscribe_user($account, variable_get('culturefeed_mailing_list', ''));

    // Subscribe to optional mailing.
    if (!empty($form_state['values']['optional_mailing'])) {
      culturefeed_mailing_subscribe_user($account, variable_get('culturefeed_mailing_list_optional', ''));
    }

    // Update user with entered fields.
    $fields = array('id', 'mbox', 'zip', 'firstname', 'lastname');

    $user_update = new CultureFeed_User();
    $user_update->id = $account->id;
    $user_update->mbox = $form_state['values']['mail'];
    $user_update->zip = $form_state['values']['zip'];
    $user_update->givenName = $form_state['values']['firstname'];
    $user_update->familyName = $form_state['values']['lastname'];

    DrupalCultureFeed::updateUser($user_update, $fields);

    $form_state['redirect'] = 'subscribe/subscribed';

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    drupal_set_message(t('An error occurred while subscribing, please try again later.'), 'error');
    return;
  }

}

/**
 * Page callback: Show a confirmation that the user is subscribed.
 */
function culturefeed_mailing_user_subscribed() {
  return variable_get('culturefeed_mailing_subscribed_message', t('You have successfully subscribed to our newsletter'));
}

/**
 * Page callback for the mailing list overview
 */
function culturefeed_mailing_lists_overview() {

  $rows = array();

  try {
    $query = new CultureFeed_SearchMailingsQuery();
    $lists = DrupalCultureFeed::getMailingList($query)->objects;
    $options = array();

    foreach ($lists as $list) {

      $edit = array(
        '#type' => 'link',
        '#title' => t('edit'),
        '#href' => 'admin/content/mailing-lists/edit/' . $list->id,
        '#options' => array('query' => drupal_get_destination()),
      );

      $delete = array(
        '#type' => 'link',
        '#title' => t('delete'),
        '#href' => 'admin/content/mailing-lists/delete/' . $list->id,
        '#options' => array('query' => drupal_get_destination()),
      );

      $test = array(
        '#type' => 'link',
        '#title' => t('send test mail'),
        '#href' => 'admin/content/mailing-lists/test/' . $list->id,
        '#options' => array('query' => drupal_get_destination()),
      );

      $item_list = array(
        '#theme' => 'item_list',
        '#attributes' => array('class' => array('links', 'inline')),
        '#items' => array(
          drupal_render($edit),
          drupal_render($delete),
          drupal_render($test),
        )
      );

      $options[$list->id] = $list->name;
      $rows[] = array(
        'id' => 'mailing-list-' . $list->id,
        'data' => array(
          $list->name,
          $list->id,
          $list->subject,
          $list->fromAddress,
          drupal_render($item_list),
        ),
      );
    }

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_mailing', $e);
    drupal_set_message($e->getMessage(), WATCHDOG_ERROR);
    return '';
  }

  return array(
    '#theme' => 'table',
    '#header' => array(t('Mailing list'), 'ID', t('Subject'), t('From address'), t('Actions')),
    '#rows' => $rows,
    '#empty' => t('No mailinglists yet'),
  );

}

/**
 * Form callback: confirm the deletion of a mailing.
 */
function culturefeed_mailing_delete_form($form, &$form_state, $mailing) {

  $form['#mailing'] = $mailing;

  // Always provide entity id in the same form key as in the entity edit form.
  $form['mail_id'] = array('#type' => 'value', '#value' => $mailing->id);

  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $mailing->name)),
    '',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );

}

/**
 * Submit the deletion form. Delete the mailing from culuturefeed.
 */
function culturefeed_mailing_delete_form_submit($form, &$form_state) {

  try {
    DrupalCultureFeed::deleteMailing($form_state['values']['mail_id']);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_mailing', $e);
    drupal_set_message(t('Error occurred while deleting the mailing'), 'error');
  }

}

/**
 * FAPI Callback for mailing add / edit form
 */
function culturefeed_mailing_lists_form($form, &$form_state, $list = NULL) {
  $form = array();

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $list ? $list->name : '',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  $form['from_address'] = array(
    '#type' => 'textfield',
    '#title' => t('From address'),
    '#default_value' => $list ? $list->fromAddress : '',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => $list ? $list->subject : '',
    '#size' => 200,
    '#maxlength' => 255,
    '#required' => TRUE,
  );

  $frequency = array(
    CultureFeed_Mailing::MAILING_FREQUENCY_ONCE => 'Once',
    CultureFeed_Mailing::MAILING_FREQUENCY_DAILY => 'Daily',
    CultureFeed_Mailing::MAILING_FREQUENCY_WEEKLY => 'Weekly',
    CultureFeed_Mailing::MAILING_FREQUENCY_BIWEEKLY => 'Biweekly',
    CultureFeed_Mailing::MAILING_FREQUENCY_MONTHLY => 'Monthly',
  );

  $form['frequency'] = array(
    '#type' => 'select',
    '#title' => t('Frequency'),
    '#options' => $frequency,
    '#default_value' => $list ? $list->frequency : '',
    '#required' => TRUE,
  );
  
  $scheduleddate = $list->scheduledDate;
  
  $form['date'] = array(
    '#type' => 'textfield',
    '#title' => t('Scheduled date'),
    '#description' => 'YYYY-MM-DD (only for frequency ONCE)',
    '#default_value' => $scheduleddate ? substr($scheduleddate, 0, 10) : date('Y-m-d'),
    '#date_format' => 'Y-m-d',
    '#required' => TRUE,
  );

  $form['search_query'] = array(
    '#type' => 'textfield',
    '#title' => t('Search Query'),
    '#default_value' => $list ? $list->searchQuery : '',
    '#size' => 60,
    '#maxlength' => 999,
  );

  $form['recommendation_query'] = array(
    '#type' => 'textfield',
    '#title' => t('Recommendation Query'),
    '#default_value' => $list ? $list->recommendationQuery : '',
    '#size' => 60,
    '#maxlength' => 128,
  );

  $form['template'] = array(
    '#type' => 'textarea',
    '#title' => t('Template'),
    '#default_value' => $list ? $list->template : '',
    '#rows' => 20,
    '#required' => TRUE,
  );

  $form['block_1'] = array(
    '#type' => 'textarea',
    '#title' => t('Block 1'),
    '#default_value' => $list ? $list->block1 : '',
    '#rows' => 20,
  );

  $form['block_2'] = array(
    '#type' => 'textarea',
    '#title' => t('Block 2'),
    '#default_value' => $list ? $list->block2 : '',
    '#rows' => 20,
  );

  $form['block_3'] = array(
    '#type' => 'textarea',
    '#title' => t('Block 3'),
    '#default_value' => $list ? $list->block3 : '',
    '#rows' => 20,
  );

  $form['block_4'] = array(
    '#type' => 'textarea',
    '#title' => t('Block 4'),
    '#default_value' => $list ? $list->block4 : '',
    '#rows' => 20,
  );

  $form['block_5'] = array(
    '#type' => 'textarea',
    '#title' => t('Block 5'),
    '#default_value' => $list ? $list->block5 : '',
    '#rows' => 20,
  );

  $form['block_6'] = array(
    '#type' => 'textarea',
    '#title' => t('Block 6'),
    '#default_value' => $list ? $list->block6 : '',
    '#rows' => 20,
  );

  $form['id'] = array(
    '#type' => 'value',
    '#value' => $list ? $list->id : NULL,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;

}

/**
 * Submit handler for mailing add / edit form
 */
function culturefeed_mailing_lists_form_submit($form, &$form_state) {

  // Check if mailing is new or update existing
  if ($form_state['values']['id']) {
    try {
      $mailing = DrupalCultureFeed::getMailing($form_state['values']['id']);
      $mailing->name = $form_state['values']['name'];
      $mailing->subject = $form_state['values']['subject'];
      $mailing->fromAddress = $form_state['values']['from_address'];
      $mailing->frequency = $form_state['values']['frequency'];
      $mailing->scheduledDate = $form_state['values']['date'];
      $mailing->template = $form_state['values']['template'];
      $mailing->block1 = $form_state['values']['block_1'];
      $mailing->block2 = $form_state['values']['block_2'];
      $mailing->block3 = $form_state['values']['block_3'];
      $mailing->block4 = $form_state['values']['block_4'];
      $mailing->block5 = $form_state['values']['block_5'];
      $mailing->block6 = $form_state['values']['block_6'];
      $mailing->searchQuery = $form_state['values']['search_query'];
      $mailing->recommendationQuery = $form_state['values']['recommendation_query'];
      $return = DrupalCultureFeed::updateMailing($mailing);
      drupal_set_message(t('Mailing list updated'));
    }
    catch (Exception $e) {
      watchdog('culturefeed_mailing', $e->getMessage());
      drupal_set_message(t('Could not update mailing list'), 'error');
    }

  }
  else {
    try {
      $mailing = new CultureFeed_Mailing();
      $mailing->consumerKey = variable_get('culturefeed_api_application_key', '');
      $mailing->name = $form_state['values']['name'];
      $mailing->subject = $form_state['values']['subject'];
      $mailing->fromAddress = $form_state['values']['from_address'];
      $mailing->frequency = $form_state['values']['frequency'];
      $mailing->scheduledDate = $form_state['values']['date'];
      $mailing->template = $form_state['values']['template'];
      $mailing->block1 = $form_state['values']['block_1'];
      $mailing->block2 = $form_state['values']['block_2'];
      $mailing->block3 = $form_state['values']['block_3'];
      $mailing->block4 = $form_state['values']['block_4'];
      $mailing->block5 = $form_state['values']['block_5'];
      $mailing->block6 = $form_state['values']['block_6'];
      $mailing->searchQuery = $form_state['values']['search_query'];
      $mailing->recommendationQuery = $form_state['values']['recommendation_query'];
      $mailing->enabled = FALSE;
      DrupalCultureFeed::createMailing($mailing);
      drupal_set_message(t('New mailing list created'));
    }
    catch (Exception $e) {
      watchdog('culturefeed_mailing', $e->getMessage());
      drupal_set_message(t('Could not save new mailing list'), 'error');
    }
  }

  if (!isset($_GET['destination'])) {
    drupal_goto('admin/content/mailing-lists');
  }
}


/**
 * FAPI callback to send test mail
 */
function culturefeed_mailing_test_list_form($form, &$form_state, $list) {
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $list->id,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('send test mail'),
  );
  return $form;
}


/**
 * Submit handler to send test mail
 */
function culturefeed_mailing_test_list_form_submit($form, &$form_state) {
  try {
    DrupalCultureFeed::sendTestMailing(DrupalCultureFeed::getLoggedInUserId(), $form_state['values']['id']);
    drupal_set_message(t('Test mail sent'));
  }
  catch (Exception $e) {
    watchdog('culturefeed_mailing', $e->getMessage());
    drupal_set_message(t('Mail could not be sent'));
  }
}

/**
 * Show the overview of newsletters + the possiblity to toggle them.
 */
function culturefeed_mailing_page_my_newsletters() {

  $query = new CultureFeed_SearchMailingsQuery();
  $query->max = 999;
  $query->consumerKey = variable_get('culturefeed_api_application_key', '');
  $mailings = DrupalCultureFeed::searchMailings($query);
  
  $build = array();
  $build['intro'] = array(
    '#markup' => '',
  );

  if ($mailings->total > 0) {
    $build['mailings'] = array(
      '#theme' => 'culturefeed_mailing_user_profile_admin',
      '#mailings' => $mailings->objects,
    );
  }

  return $build;

}