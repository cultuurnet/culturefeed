<?php

/**
 * @file
 * Page callback functions for culturefeed_mailing module.
 */

/**
 * Page callback to toggle newsletter optin.
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
  catch (CultureFeed_Exception $e) {

    // If user was already subscribed, don't show a message.
    if ($e->error_code != CultureFeed::CODE_MAILING_ALREADY_SUBSCRIBED) {
      watchdog_exception('culturefeed_mailing', $e);
      $success = FALSE;
    }

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_mailing', $e);
    $success = FALSE;
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

  $account = culturefeed_load_logged_in_user();

  // Redirect to loginpage if not logged in and light id permission is off.
  if (!$account && !CULTUREFEED_API_LIGHT_ID_ALLOWED) {
    drupal_goto('authenticated', array('query' => array('destination' => 'subscribe')));
  }

  // Show nothing if no mailing list available.
  if (!(variable_get('culturefeed_mailing_list', ''))) {
    return;
  }

  $form = array();

  // Set defaults for the email field.
  if ($account) {
    $email = (!empty($_GET['email'])) ? $_GET['email'] : ($account ? $account->mbox : '');
    $disabled = TRUE;
    $description = '<span>' . t('Change your email address <a href="!url">here</a>.', array('!url' => url('culturefeed/account/edit'))) . '</span>';
  }
  else {
    $email = (!empty($_GET['email'])) ? $_GET['email'] : '';
    $disabled = FALSE;
    $description = '';
  }
  $form['mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Email address'),
    '#required' => TRUE,
    '#size' => 40,
    '#maxlength' => 255,
    '#default_value' => $email,
    '#weight' => -10,
    '#disabled' => $disabled,
    '#description' => $description,
  );

  $show_zip = variable_get('culturefeed_mailing_block_show_zip', 0);

  $postal = '';
  if (!empty($_GET['zip'])) {
    $postal = $_GET['zip'];
  }
  elseif ($account) {
    $postal = $account->zip;
  }

  $form['zip'] = array(
    '#type' => 'textfield',
    '#title' => t('Zipcode'),
    '#required' => !CULTUREFEED_API_LIGHT_ID_ALLOWED,
    '#size' => 40,
    '#maxlength' => 255,
    '#default_value' => $postal,
    '#weight' => -9,
    '#access' => !CULTUREFEED_API_LIGHT_ID_ALLOWED || (CULTUREFEED_API_LIGHT_ID_ALLOWED && $show_zip),
    '#attributes' => array(
      'class' => array('zip-field'),
    ),
  );

  $form['firstname'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#required' => TRUE,
    '#size' => 40,
    '#maxlength' => 255,
    '#default_value' => ($account ? $account->givenName : ''),
    '#weight' => -8,
    '#access' => !CULTUREFEED_API_LIGHT_ID_ALLOWED,
  );

  $form['lastname'] = array(
    '#type' => 'textfield',
    '#title' => t('Family name'),
    '#required' => TRUE,
    '#size' => 40,
    '#maxlength' => 255,
    '#default_value' => ($account && isset($account->familyName) ? $account->familyName : ''),
    '#weight' => -7,
    '#access' => !CULTUREFEED_API_LIGHT_ID_ALLOWED,
  );

  if (variable_get('culturefeed_mailing_list_optional', '')) {
    $form['optional_mailing'] = array(
      '#type' => 'checkbox',
      '#title' => variable_get('culturefeed_mailing_optional_description', ''),
      '#access' => !CULTUREFEED_API_LIGHT_ID_ALLOWED,
    );
  }

  $form['privacy'] = array(
    '#type' => 'markup',
    '#value' => '<div id="privacy">' . l(t('Privacy statement'), 'privacyverklaring') . '</div>',
    '#access' => !CULTUREFEED_API_LIGHT_ID_ALLOWED,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Subscribe'),
    '#weight' => 10,
  );

  if (CULTUREFEED_API_LIGHT_ID_ALLOWED && $show_zip && empty($postal)) {
    // Attach jquery.cookie library.
    $form['#attached']['library'][] = array('system', 'jquery.cookie');

    // Attach scripts.
    $form['#attached']['js'][] = array(
      'data' => drupal_get_path('module', 'culturefeed_mailing') . '/js/culturefeed_mailing.js',
      'type' => 'file',
      'weight' => 0,
    );
  }

  return $form;

}

/**
 * Submit the mailing subscription form.
 */
function culturefeed_mailing_subscribe_form_submit($form, &$form_state) {

  if (!empty($form_state['values']['zip'])) {
    $zip = $form_state['values']['zip'];
    _culturefeed_mailing_set_postal_to_cookie($form_state['values']['zip']);
  }
  else {
    $zip = '';
  }

  if (CULTUREFEED_API_LIGHT_ID_ALLOWED) {

    try {
      culturefeed_mailing_subscribe_user_light($form_state['values']['mail'], variable_get('culturefeed_mailing_list', ''), $zip);
    }
    catch (CultureFeed_Exception $e) {

      // If user was already subscribed, don't show a message.
      if ($e->error_code != CultureFeed::CODE_MAILING_ALREADY_SUBSCRIBED) {
        drupal_set_message(t('An error occurred while subscribing, please try again later.'), 'error');
        return;
      }

    }
    catch (Exception $e) {
      drupal_set_message(t('An error occurred while subscribing, please try again later.'), 'error');
      return;
    }

  }
  else {
    $account = DrupalCultureFeed::getLoggedInUser();

    // Subscribe to normal mailing.
    try {
      culturefeed_mailing_subscribe_user($account, variable_get('culturefeed_mailing_list', ''));
    }
    catch (CultureFeed_Exception $e) {

      // If user was already subscribed, don't show a message.
      if ($e->error_code != CultureFeed::CODE_MAILING_ALREADY_SUBSCRIBED) {
        drupal_set_message(t('An error occurred while subscribing, please try again later.'), 'error');
        return;
      }

    }
    catch (Exception $e) {
      drupal_set_message(t('An error occurred while subscribing, please try again later.'), 'error');
      return;
    }

    // Subscribe to optional mailing.
    if (!empty($form_state['values']['optional_mailing'])) {
      try {
        culturefeed_mailing_subscribe_user($account, variable_get('culturefeed_mailing_list_optional', ''));
      }
      catch (CultureFeed_Exception $e) {

        // If user was already subscribed, don't show a message.
        if ($e->error_code != CultureFeed::CODE_MAILING_ALREADY_SUBSCRIBED) {
          drupal_set_message(t('An error occurred while subscribing, please try again later.'), 'error');
          return;
        }

      }
      catch (Exception $e) {
        drupal_set_message(t('An error occurred while subscribing, please try again later.'), 'error');
        return;
      }
    }

    // Update user with entered fields.
    $fields = array('id', 'mbox', 'zip', 'firstname', 'lastname');

    $user_update = new CultureFeed_User();
    $user_update->id = $account->id;
    $user_update->mbox = $form_state['values']['mail'];
    $user_update->zip = $form_state['values']['zip'];
    $user_update->givenName = $form_state['values']['firstname'];
    $user_update->familyName = $form_state['values']['lastname'];

    try {
      DrupalCultureFeed::updateUser($user_update, $fields);
    }
    catch (Exception $e) {
      // Show no error here.
    }
  }

  $form_state['redirect'] = 'subscribe/subscribed';

}

/**
 * Page callback: Show a confirmation that the user is subscribed.
 */
function culturefeed_mailing_user_subscribed() {
  return variable_get('culturefeed_mailing_subscribed_message', t('You have successfully subscribed to our newsletter'));
}

/**
 * Page callback for the mailing list overview.
 */
function culturefeed_mailing_lists_overview() {

  $rows = array();

  try {
    $query = new CultureFeed_SearchMailingsQuery();
    $lists = DrupalCultureFeed::getMailingList($query)->objects;
    $options = array();

    foreach ($lists as $list) {
      if (!empty($list->template)) {
        $template = $list->template;
      }
      else {
        $template = '';
      }

      $edit = array(
        '#type' => 'link',
        '#title' => t('edit'),
        '#href' => 'admin/content/mailing-lists/edit/' . $list->id,
        '#options' => array('query' => drupal_get_destination()),
      );

      $test = array(
        '#type' => 'link',
        '#title' => t('send test mail'),
        '#href' => 'admin/content/mailing-lists/test/' . $list->id,
        '#id' => 'ajax_link_' . $list->id,
        '#ajax' => array(
          'method' => 'html',
        ),
      );

      $item_list = array(
        '#theme' => 'item_list',
        '#attributes' => array('class' => array('links', 'inline')),
        '#items' => array(
          drupal_render($edit),
          drupal_render($test),
        ),
      );

      $send = array(
        '#type' => 'link',
        '#title' => t('activate'),
        '#href' => 'admin/content/mailing-lists/send/' . $list->id,
        '#options' => array('query' => drupal_get_destination(), 'attributes' => array('class' => array('button'))),
        '#prefix' => '<div class="mailing-inactive"></div>',
      );

      $disable = array(
        '#type' => 'link',
        '#title' => t('deactivate'),
        '#href' => 'admin/content/mailing-lists/disable/' . $list->id,
        '#options' => array('query' => drupal_get_destination()),
        '#prefix' => '<div class="mailing-active"></div>',
      );

      $options[$list->id] = $list->name;
      $rows[] = array(
        'id' => 'mailing-list-' . $list->id,
        'data' => array(
          $list->name,
          empty($template->name) ? '' : $template->name,
          $list->enabled == 0 ? drupal_render($send) : drupal_render($disable),
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
    '#header' => array(
      t('Mailing list'),
      t('Template'),
      t('Status'),
      t('Actions'),
    ),
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
    'admin/content/mailing-lists',
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
    if (isset($_GET['destination'])) {
      $form_state['redirect'] = $_GET['destination'];
    }
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_mailing', $e);
    drupal_set_message(t('Error occurred while deleting the mailing'), 'error');
  }

}

/**
 * FAPI Callback for mailing add / edit form.
 */
function culturefeed_mailing_lists_form($form, &$form_state, $list = NULL) {
  $form = array();

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => !empty($list->name) ? $list->name : '',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => !empty($list->description) ? $list->description : '',
    '#rows' => 5,
    '#required' => TRUE,
  );

  $user = DrupalCultureFeed::getLoggedInAccount();
  if (array_key_exists(4, $user->roles)) {
    $consumers = DrupalCultureFeed::getUserServiceConsumers(DrupalCultureFeed::getLoggedInUserId());
    if (!empty($list)) {
      $default_value = $list->consumerKey;
    }
    else {
      // For new template set default consumerKey from site.
      $default_value = variable_get('culturefeed_api_application_key', '');
    }
    $options = array();
    if (!empty($consumers)) {
      foreach ($consumers as $key => $consumer) {
        $options[$consumer->consumerKey] = $consumer->name;
      }
    }

    // This item is disabled as the consumer key can not be changed.
    $form['channel'] = array(
      '#type' => 'select',
      '#title' => t('Channel'),
      '#options' => $options,
      '#default_value' => $default_value,
      '#disabled' => TRUE,
      '#description' => t('The consumer key can only be changed from the UITid administrator.'),
    );
  }

  $templates = DrupalCultureFeed::getTemplateList();
  $template_options = array();
  $template_options[0] = t('- Select -');
  if (!empty($templates)) {
    foreach ($templates->objects as $key => $template) {
      $template_options[$template->id] = $template->name;
    }
  }

  $form['template'] = array(
    '#type' => 'select',
    '#title' => t('Template'),
    '#options' => $template_options,
    '#default_value' => !empty($list->template->id) ? $list->template->id : '',
  );

  $form['id'] = array(
    '#type' => 'value',
    '#value' => !empty($list->id) ? $list->id : NULL,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  if (!empty($list)) {
    $form['delete'] = array(
      '#type' => 'markup',
      '#markup' => l(t('Delete'), 'admin/content/mailing-lists/delete/' . $list->id, array('query' => drupal_get_destination())),
    );
  }

  return $form;

}

/**
 * Submit handler for mailing add / edit form.
 */
function culturefeed_mailing_lists_form_submit($form, &$form_state) {

  // Check if mailing is new or update existing.
  if ($form_state['values']['id']) {
    try {
      $mailing = DrupalCultureFeed::getMailing($form_state['values']['id']);
      $mailing->name = $form_state['values']['name'];
      $mailing->description = $form_state['values']['description'];
      $user = DrupalCultureFeed::getLoggedInAccount();

      if (array_key_exists(4, $user->roles)) {
        $mailing->consumerKey = $form_state['values']['channel'];
      }
      else {
        $mailing->consumerKey = variable_get('culturefeed_api_application_key', '');
      }
      $mailing->template_id = $form_state['values']['template'];
      $return = DrupalCultureFeed::updateMailing($mailing);
      drupal_set_message(t('Mailing list updated'));
    }
    catch (Exception $e) {
      watchdog('culturefeed_mailing', $e->getMessage());
      drupal_set_message(t('Could not update mailing'), 'error');
    }

  }
  else {
    try {
      $mailing = new CultureFeed_Mailing();
      $user = DrupalCultureFeed::getLoggedInAccount();
      if (array_key_exists(4, $user->roles)) {
        $mailing->consumerKey = $form_state['values']['channel'];
      }
      else {
        $mailing->consumerKey = variable_get('culturefeed_api_application_key', '');
      }
      $mailing->name = $form_state['values']['name'];
      $mailing->description = $form_state['values']['description'];
      $mailing->template_id = $form_state['values']['template'];
      $mailing->enabled = FALSE;
      DrupalCultureFeed::createMailing($mailing);
      drupal_set_message(t('New mailing created'));
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
 * FAPI callback to send test mail.
 */
function culturefeed_mailing_test_list($list) {
  try {
    DrupalCultureFeed::sendTestMailing(DrupalCultureFeed::getLoggedInUserId(), $list->id);
    $user = DrupalCultureFeed::getLoggedInUser();
    drupal_set_message(t('A test mail was sent to !user_mail', array('!user_mail' => $user->mbox)));
    $commands = array();
    $commands[] = ajax_command_remove('div.messages');
    $commands[] = ajax_command_before('#content', theme('status_messages'));
    return array(
      '#type' => 'ajax',
      '#commands' => $commands,
    );
  }
  catch (Exception $e) {
    watchdog('culturefeed_mailing', $e->getMessage());
    drupal_set_message(t('Mail could not be sent'), 'error');
    $commands = array();
    $commands[] = ajax_command_remove('div.messages');
    $commands[] = ajax_command_before('#content', theme('status_messages'));
    return array(
      '#type' => 'ajax',
      '#commands' => $commands,
    );
  }
}

/**
 * FAPI callback to activate mail.
 */
function culturefeed_mailing_send_list_form($form, &$form_state, $list) {
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $list->id,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Activate mail'),
  );
  return $form;
}


/**
 * Submit handler to activate mail.
 */
function culturefeed_mailing_send_list_form_submit($form, &$form_state) {
  try {
    DrupalCultureFeed::sendMailing($form_state['values']['id']);
    drupal_set_message(t('This mailing is active and will be send on scheduled time'));
  }
  catch (Exception $e) {
    watchdog('culturefeed_mailing', $e->getMessage());
    drupal_set_message(t('Mailing could not be activated'));
  }
}


/**
 * FAPI callback to deactivate mail.
 */
function culturefeed_mailing_disable_list_form($form, &$form_state, $list) {
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $list->id,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Deactivate scheduling'),
  );
  return $form;
}


/**
 * Submit handler to deactivate mail.
 */
function culturefeed_mailing_disable_list_form_submit($form, &$form_state) {
  try {
    DrupalCultureFeed::disableMailing($form_state['values']['id']);
    drupal_set_message(t('Scheduling for this mailing is deactivated'));
  }
  catch (Exception $e) {
    watchdog('culturefeed_mailing', $e->getMessage());
    drupal_set_message(t('Mailing could not be deactivated'));
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

/**
 * Page callback for the template list overview.
 */
function culturefeed_template_list_overview() {
  $rows = array();

  try {
    $templates = DrupalCultureFeed::getTemplateList()->objects;

    $options = array();

    foreach ($templates as $template) {
      $consumer = DrupalCultureFeed::getServiceConsumer($template->consumerKey);

      $edit = array(
        '#type' => 'link',
        '#title' => t('edit'),
        '#href' => 'admin/content/templates/edit/' . $template->id,
        '#options' => array('query' => drupal_get_destination()),
      );

      $item_list = array(
        '#theme' => 'item_list',
        '#attributes' => array('class' => array('links', 'inline')),
        '#items' => array(
          drupal_render($edit),
        ),
      );

      $options[$template->id] = $template->name;
      $rows[] = array(
        'id' => 'template' . $template->id,
        'data' => array(
          $template->name,
          $template->subject,
          $consumer->name,
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
    '#header' => array(t('Name'), t('Subject'), t('Channel'), t('Actions')),
    '#rows' => $rows,
    '#empty' => t('No mailinglists yet'),
  );

}

/**
 * Form callback: confirm the deletion of a template.
 */
function culturefeed_templates_delete_form($form, &$form_state, $template) {

  $form['#template'] = $template;

  // Always provide entity id in the same form key as in the entity edit form.
  $form['template_id'] = array('#type' => 'value', '#value' => $template->id);

  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $template->name)),
    '',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );

}

/**
 * Submit the deletion form. Delete the template from culuturefeed.
 */
function culturefeed_templates_delete_form_submit($form, &$form_state) {

  try {
    DrupalCultureFeed::deleteTemplate($form_state['values']['template_id']);
    if (isset($_GET['destination'])) {
      $form_state['redirect'] = $_GET['destination'];
    }
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_mailing', $e);
    drupal_set_message(t('Error occurred while deleting the template'), 'error');
  }

}

/**
 * FAPI Callback for template add / edit form.
 */
function culturefeed_templates_form($form, &$form_state, $template = NULL) {
  $form = array();

  $form['basic_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['basic_settings']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => !empty($template) ? $template->name : '',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $user = DrupalCultureFeed::getLoggedInAccount();
  if (array_key_exists(4, $user->roles)) {
    $consumers = DrupalCultureFeed::getUserServiceConsumers(DrupalCultureFeed::getLoggedInUserId());
    if (!empty($template)) {
      $default_value = $template->consumerKey;
    }
    else {
      // For new template set default consumerKey from site.
      $default_value = variable_get('culturefeed_api_application_key', '');
    }
    $options = array();
    if (!empty($consumers)) {
      foreach ($consumers as $key => $consumer) {
        $options[$consumer->consumerKey] = $consumer->name;
      }
    }

    // This item is disabled as the consumer key can not be changed.
    $form['basic_settings']['channel'] = array(
      '#type' => 'select',
      '#title' => t('Channel'),
      '#options' => $options,
      '#default_value' => $default_value,
      '#disabled' => TRUE,
      '#description' => t('The consumer key can only be changed from the UITid administrator.'),
    );
  }

  $path = 'http://tools.uitdatabank.be/docs/parameters-en-velden-2-DEV#Mail template';
  $link = l(t('Read documentation'), $path, array('attributes' => array('target' => '_blank')));

  $form['basic_settings']['template'] = array(
    '#type' => 'textarea',
    '#title' => t('Template'),
    '#default_value' => !empty($template) ? $template->template : '',
    '#rows' => 20,
    '#required' => TRUE,
    '#description' => t('This template supports different variables. !link', array('!link' => $link)),
  );

  $form['basic_settings']['from_address'] = array(
    '#type' => 'textfield',
    '#title' => t('From address'),
    '#default_value' => !empty($template) ? $template->fromAddress : '',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $form['content'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['content']['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => !empty($template) ? $template->subject : '',
    '#size' => 200,
    '#maxlength' => 255,
    '#required' => TRUE,
  );

  $form['content']['block_1'] = array(
    '#type' => 'textarea',
    '#title' => t('Block 1'),
    '#default_value' => !empty($template) ? $template->block1 : '',
    '#rows' => 20,
  );

  $form['content']['block_2'] = array(
    '#type' => 'textarea',
    '#title' => t('Block 2'),
    '#default_value' => !empty($template) ? $template->block2 : '',
    '#rows' => 20,
  );

  $form['content']['block_3'] = array(
    '#type' => 'textarea',
    '#title' => t('Block 3'),
    '#default_value' => !empty($template) ? $template->block3 : '',
    '#rows' => 20,
  );

  $form['content']['block_4'] = array(
    '#type' => 'textarea',
    '#title' => t('Block 4'),
    '#default_value' => !empty($template) ? $template->block4 : '',
    '#rows' => 20,
  );

  $form['content']['block_5'] = array(
    '#type' => 'textarea',
    '#title' => t('Block 5'),
    '#default_value' => !empty($template) ? $template->block5 : '',
    '#rows' => 20,
  );

  $form['content']['block_6'] = array(
    '#type' => 'textarea',
    '#title' => t('Block 6'),
    '#default_value' => !empty($template) ? $template->block6 : '',
    '#rows' => 20,
  );

  $form['extra_content'] = array(
    '#type' => 'fieldset',
    '#title' => t('Extra content & suggestions'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $path = 'http://tools.uitdatabank.be/docs/parameters-en-velden-2-DEV#Query variabelen';
  $link = l(t('More info'), $path, array('attributes' => array('target' => '_blank')));

  $form['extra_content']['information'] = array(
    '#type' => 'markup',
    '#markup' => '<div>' . t("You can enrich the content of the basic template with event suggestions based on a search query or the user's profile. !link", array('!link' => $link)) . '</div>',
  );

  $form['extra_content']['label'] = array(
    '#type' => 'markup',
    '#markup' => '<div>' . t('Use suggestions based on:') . '</div>',
  );

  $form['extra_content']['search_option'] = array(
    '#type' => 'checkbox',
    '#title' => t('A search query'),
    '#default_value' => !empty($template) ? $template->searchEnabled : '',
  );

  $form['extra_content']['search_container'] = array(
    '#type' => 'container',
    '#states' => array(
      'invisible' => array(
        'input[name="search_option"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['extra_content']['search_container']['search_text'] = array(
    '#type' => 'textfield',
    '#description' => t('Search API Query. Example: q=Concert&city=${city}&k=${lifestyleProfile}&sort=proximity'),
    '#default_value' => !empty($template) ? $template->searchQuery : '',
    '#size' => 60,
    '#maxlength' => 999,
  );

  $form['extra_content']['search_container']['no_search_results_checkbox'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send mail even if there are no results'),
    '#default_value' => !empty($template) ? $template->sendEmptySearchResult : '',
  );

  $form['extra_content']['recommendation_option'] = array(
    '#type' => 'checkbox',
    '#title' => t('Personal recommendations'),
    '#default_value' => !empty($template) ? $template->recommendationEnabled : '',
  );

  $form['extra_content']['recommendation_container'] = array(
    '#type' => 'container',
    '#states' => array(
      'invisible' => array(
        'input[name="recommendation_option"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['extra_content']['recommendation_container']['recommendation_text'] = array(
    '#type' => 'textfield',
    '#description' => t('Recommendation Query. Example: uid=${uid}'),
    '#default_value' => !empty($template) ? $template->recommendationQuery : '',
    '#size' => 60,
    '#maxlength' => 128,
  );

  $form['extra_content']['recommendation_container']['no_recommendations_results_checkbox'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send mail even if there are no results'),
    '#default_value' => !empty($template) ? $template->sendEmptyRecommendationResult : '',
  );

  $form['dispatch'] = array(
    '#type' => 'fieldset',
    '#title' => t('Dispatch'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $frequency = array(
    CultureFeed_Template::MAILING_FREQUENCY_ONCE => t('Once'),
    CultureFeed_Template::MAILING_FREQUENCY_DAILY => t('Daily'),
    CultureFeed_Template::MAILING_FREQUENCY_WEEKLY => t('Weekly'),
    CultureFeed_Template::MAILING_FREQUENCY_BIWEEKLY => t('Biweekly'),
    CultureFeed_Template::MAILING_FREQUENCY_MONTHLY => t('Monthly'),
  );

  $form['dispatch']['frequency'] = array(
    '#type' => 'select',
    '#title' => t('Frequency'),
    '#options' => $frequency,
    '#default_value' => !empty($template) ? $template->frequency : '',
    '#required' => TRUE,
  );

  // Container with options when ONCE frequency is selected.
  $form['dispatch']['date_container_once'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        'select[name="frequency"]' => array('value' => 'ONCE'),
      ),
    ),
  );

  if (!empty($template->scheduledDate)) {
    $scheduleddate = $template->scheduledDate;
  }
  $form['dispatch']['date_container_once']['once_date'] = array(
    '#type' => 'date_popup',
    '#date_format' => 'd-m-Y',
    '#default_value' => isset($scheduleddate) ? substr($scheduleddate, 0, 10) : date('Y-m-d'),
  );

  $form['dispatch']['date_container_once']['time_label'] = array(
    '#type' => 'markup',
    '#markup' => '<label>' . t('Start time') . '</label>',
  );

  $form['dispatch']['date_container_once']['once_hour'] = array(
    '#type' => 'textfield',
    '#default_value' => !empty($template->startHour) ? $template->startHour : 12,
    '#size' => 5,
    '#attributes' => array('placeholder' => t('hh')),
    '#prefix' => '<div class="container-inline">',
  );

  $form['dispatch']['date_container_once']['once_markup'] = array(
    '#type' => 'markup',
    '#markup' => ':',
  );

  $form['dispatch']['date_container_once']['once_minutes'] = array(
    '#type' => 'textfield',
    '#default_value' => !empty($template->startMinute) ? $template->startMinute : 0,
    '#size' => 5,
    '#attributes' => array('placeholder' => t('mm')),
    '#suffix' => '</div>',
  );

  // Container with options when DAILY frequency is selected.
  $form['dispatch']['date_container_daily'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        'select[name="frequency"]' => array('value' => 'DAILY'),
      ),
    ),
  );

  $form['dispatch']['date_container_daily']['daily_hour'] = array(
    '#type' => 'textfield',
    '#default_value' => !empty($template->startHour) ? $template->startHour : 12,
    '#size' => 5,
    '#attributes' => array('placeholder' => t('hh')),
    '#prefix' => '<div class="container-inline">',
  );

  $form['dispatch']['date_container_daily']['daily_markup'] = array(
    '#type' => 'markup',
    '#markup' => ':',
  );

  $form['dispatch']['date_container_daily']['daily_minutes'] = array(
    '#type' => 'textfield',
    '#default_value' => !empty($template->startMinute) ? $template->startMinute : 0,
    '#size' => 5,
    '#attributes' => array('placeholder' => t('mm')),
    '#suffix' => '</div>',
  );

  // Container with options when WEEKLY frequency is selected.
  $form['dispatch']['date_container_weekly'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        'select[name="frequency"]' => array('value' => 'WEEKLY'),
      ),
    ),
  );

  $form['dispatch']['date_container_weekly']['weekly_day'] = array(
    '#title' => t('On day'),
    '#type' => 'select',
    '#options' => _culturefeed_mailing_get_weekdays(),
    '#default_value' => !empty($template->startDayOfWeek) ? $template->startDayOfWeek : 1,
  );

  $form['dispatch']['date_container_weekly']['weekly_hour'] = array(
    '#type' => 'textfield',
    '#default_value' => !empty($template->startHour) ? $template->startHour : 12,
    '#size' => 5,
    '#attributes' => array('placeholder' => t('hh')),
    '#prefix' => '<div class="container-inline">',
  );

  $form['dispatch']['date_container_weekly']['weekly_markup'] = array(
    '#type' => 'markup',
    '#markup' => ':',
  );

  $form['dispatch']['date_container_weekly']['weekly_minutes'] = array(
    '#type' => 'textfield',
    '#default_value' => !empty($template->startMinute) ? $template->startMinute : 0,
    '#size' => 5,
    '#attributes' => array('placeholder' => t('mm')),
    '#suffix' => '</div>',
  );

  // Container with options when BIWEEKLY frequency is selected.
  $form['dispatch']['date_container_biweekly'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        'select[name="frequency"]' => array('value' => 'BIWEEKLY'),
      ),
    ),
  );

  $form['dispatch']['date_container_biweekly']['biweekly_day'] = array(
    '#title' => t('On day'),
    '#type' => 'select',
    '#options' => _culturefeed_mailing_get_weekdays(),
    '#default_value' => !empty($template->startDayOfWeek) ? $template->startDayOfWeek : 1,
  );

  $form['dispatch']['date_container_biweekly']['biweekly_hour'] = array(
    '#type' => 'textfield',
    '#default_value' => !empty($template->startHour) ? $template->startHour : 12,
    '#size' => 5,
    '#attributes' => array('placeholder' => t('hh')),
    '#prefix' => '<div class="container-inline">',
  );

  $form['dispatch']['date_container_biweekly']['biweekly_markup'] = array(
    '#type' => 'markup',
    '#markup' => ':',
  );

  $form['dispatch']['date_container_biweekly']['biweekly_minutes'] = array(
    '#type' => 'textfield',
    '#default_value' => !empty($template->startMinute) ? $template->startMinute : 0,
    '#size' => 5,
    '#attributes' => array('placeholder' => t('mm')),
    '#suffix' => '</div>',
  );

  // Container with options when MONTHLY frequency is selected.
  $form['dispatch']['date_container_monthly'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        'select[name="frequency"]' => array('value' => 'MONTHLY'),
      ),
    ),
  );

  $form['dispatch']['date_container_monthly']['monthly_day'] = array(
    '#title' => t('Day of the month'),
    '#type' => 'select',
    '#options' => _culturefeed_mailing_get_day_of_the_month(),
    '#default_value' => !empty($template->startDay) ? $template->startDay : 1,
  );

  $form['dispatch']['date_container_monthly']['monthly_hour'] = array(
    '#type' => 'textfield',
    '#default_value' => !empty($template->startHour) ? $template->startHour : 12,
    '#size' => 5,
    '#attributes' => array('placeholder' => t('hh')),
    '#prefix' => '<div class="container-inline">',
  );

  $form['dispatch']['date_container_monthly']['monthly_markup'] = array(
    '#type' => 'markup',
    '#markup' => ':',
  );

  $form['dispatch']['date_container_monthly']['monthly_minutes'] = array(
    '#type' => 'textfield',
    '#default_value' => !empty($template->startMinute) ? $template->startMinute : 0,
    '#size' => 5,
    '#attributes' => array('placeholder' => t('mm')),
    '#suffix' => '</div>',
  );

  $form['id'] = array(
    '#type' => 'value',
    '#value' => !empty($template) ? $template->id : NULL,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  if (!empty($template)) {
    $form['delete'] = array(
      '#type' => 'markup',
      '#markup' => l(t('Delete'), 'admin/content/templates/delete/' . $template->id, array('query' => drupal_get_destination())),
    );
  }

  return $form;

}

/**
 * Validate handler for template add / edit form.
 */
function culturefeed_templates_form_validate($form, &$form_state) {
  // Validate frequency and its options.
  $values = $form_state['values'];

  switch ($values['frequency']) {
    case 'ONCE':
      if ($values['once_date'] == '') {
        form_set_error('once_date', t('You need to set a date when selecting "once".'));
      }
      if ($values['once_hour'] == '') {
        form_set_error('once_hour', t('You need to set the hour when selecting "once".'));
      }
      elseif ($values['once_hour'] < 6 || $values['once_hour'] > 22) {
        form_set_error('once_hour', t('Invalid start hour: !hour. Start hour must be a value between 6 and 22 (both ends inclusive)', array('!hour' => $values['once_hour'])));
      }
      if ($values['once_minutes'] == '') {
        form_set_error('once_minutes', t('You need to set the minutes when selecting "once".'));
      }
      elseif ($values['once_minutes'] < 0 || $values['once_minutes'] > 59) {
        form_set_error('once_minutes', t('Invalid start minutes: !minutes. Start minutes must be a value between 0 and 59 (both ends inclusive)', array('!minutes' => $values['once_minutes'])));
      }
      break;

    case 'DAILY':
      if ($values['daily_hour'] == '') {
        form_set_error('daily_hour', t('You need to set the hour when selecting "daily".'));
      }
      elseif ($values['daily_hour'] < 6 || $values['daily_hour'] > 22) {
        form_set_error('daily_hour', t('Invalid start hour: !hour. Start hour must be a value between 6 and 22 (both ends inclusive)', array('!hour' => $values['daily_hour'])));
      }
      if ($values['daily_minutes'] == '') {
        form_set_error('daily_minutes', t('You need to set the minutes when selecting "daily".'));
      }
      elseif ($values['daily_minutes'] < 0 || $values['daily_minutes'] > 59) {
        form_set_error('daily_minutes', t('Invalid start minutes: !minutes. Start minutes must be a value between 0 and 59 (both ends inclusive)', array('!minutes' => $values['daily_minutes'])));
      }
      break;

    case 'WEEKLY':
      if ($values['weekly_day'] == '') {
        form_set_error('weekly_day', t('You need to set the day when selecting "weekly".'));
      }
      if ($values['weekly_hour'] == '') {
        form_set_error('weekly_hour', t('You need to set the hour when selecting "weekly".'));
      }
      elseif ($values['weekly_hour'] < 6 || $values['weekly_hour'] > 22) {
        form_set_error('weekly_hour', t('Invalid start hour: !hour. Start hour must be a value between 6 and 22 (both ends inclusive)', array('!hour' => $values['weekly_hour'])));
      }
      if ($values['weekly_minutes'] == '') {
        form_set_error('weekly_minutes', t('You need to set the minutes when selecting "weekly".'));
      }
      elseif ($values['weekly_minutes'] < 0 || $values['weekly_minutes'] > 59) {
        form_set_error('weekly_minutes', t('Invalid start minutes: !minutes. Start minutes must be a value between 0 and 59 (both ends inclusive)', array('!minutes' => $values['weekly_minutes'])));
      }
      break;

    case 'BIWEEKLY':
      if ($values['biweekly_day'] == '') {
        form_set_error('biweekly_day', t('You need to set the day when selecting "biweekly".'));
      }
      if ($values['biweekly_hour'] == '') {
        form_set_error('weekly_hour', t('You need to set the hour when selecting "biweekly".'));
      }
      elseif ($values['biweekly_hour'] < 6 || $values['biweekly_hour'] > 22) {
        form_set_error('biweekly_hour', t('Invalid start hour: !hour. Start hour must be a value between 6 and 22 (both ends inclusive)', array('!hour' => $values['biweekly_hour'])));
      }
      if ($values['biweekly_minutes'] == '') {
        form_set_error('weekly_minutes', t('You need to set the minutes when selecting "biweekly".'));
      }
      elseif ($values['biweekly_minutes'] < 0 || $values['biweekly_minutes'] > 59) {
        form_set_error('biweekly_minutes', t('Invalid start minutes: !minutes. Start minutes must be a value between 0 and 59 (both ends inclusive)', array('!minutes' => $values['biweekly_minutes'])));
      }
      break;

    case 'MONTHLY':
      if ($values['monthly_day'] == '') {
        form_set_error('monthly_day', t('You need to set the day when selecting "monthly".'));
      }
      if ($values['monthly_hour'] == '') {
        form_set_error('monthly_hour', t('You need to set the hour when selecting "monthly".'));
      }
      elseif ($values['monthly_hour'] < 6 || $values['monthly_hour'] > 22) {
        form_set_error('monthly_hour', t('Invalid start hour: !hour. Start hour must be a value between 6 and 22 (both ends inclusive)', array('!hour' => $values['monthly_hour'])));
      }
      if ($values['monthly_minutes'] == '') {
        form_set_error('monthly_minutes', t('You need to set the minutes when selecting "monthly".'));
      }
      elseif ($values['monthly_minutes'] < 0 || $values['monthly_minutes'] > 59) {
        form_set_error('monthly_minutes', t('Invalid start minutes: !minutes. Start minutes must be a value between 0 and 59 (both ends inclusive)', array('!minutes' => $values['monthly_minutes'])));
      }
      break;
  }

  if ($values['search_option'] == 1 && empty($values['search_text'])) {
    form_set_error('search_text', t('When selecting a search query, you need to provide a search query.'));
  }

  if ($values['recommendation_option'] == 1 && empty($values['recommendation_text'])) {
    form_set_error('recommendation_text', t('When selecting a recommendation query, you need to provide a recommendation query.'));
  }
}

/**
 * Submit handler for template add / edit form.
 */
function culturefeed_templates_form_submit($form, &$form_state) {

  // Check if mailing is new or update existing.
  if ($form_state['values']['id']) {
    try {
      $template = DrupalCultureFeed::getTemplate($form_state['values']['id']);
      if (!empty($form_state['values']['channel'])) {
        $template->consumerKey = $form_state['values']['channel'];
      }
      else {
        $template->consumerKey = variable_get('culturefeed_api_application_key', '');
      }
      $template->name = $form_state['values']['name'];
      $template->subject = $form_state['values']['subject'];
      $template->fromAddress = $form_state['values']['from_address'];
      $template->template = $form_state['values']['template'];
      $template->block1 = $form_state['values']['block_1'];
      $template->block2 = $form_state['values']['block_2'];
      $template->block3 = $form_state['values']['block_3'];
      $template->block4 = $form_state['values']['block_4'];
      $template->block5 = $form_state['values']['block_5'];
      $template->block6 = $form_state['values']['block_6'];
      $template->searchEnabled = $form_state['values']['search_option'];
      $template->searchQuery = $form_state['values']['search_text'];
      $template->sendEmptySearchResult = $form_state['values']['no_search_results_checkbox'];
      $template->recommendationEnabled = $form_state['values']['recommendation_option'];
      $template->recommendationQuery = $form_state['values']['recommendation_text'];
      $template->sendEmptyRecommendationResult = $form_state['values']['no_recommendations_results_checkbox'];
      // Save frequency and associated fields.
      $template->frequency = $form_state['values']['frequency'];
      switch ($form_state['values']['frequency']) {
        case 'ONCE':
          $template->scheduledDate = $form_state['values']['once_date'];
          $template->startHour = $form_state['values']['once_hour'];
          $template->startMinute = $form_state['values']['once_minutes'];
          break;

        case 'DAILY':
          $template->startHour = $form_state['values']['daily_hour'];
          $template->startMinute = $form_state['values']['daily_minutes'];
          break;

        case 'WEEKLY':
          $template->startDayOfWeek = $form_state['values']['weekly_day'];
          $template->startHour = $form_state['values']['weekly_hour'];
          $template->startMinute = $form_state['values']['weekly_minutes'];
          break;

        case 'BIWEEKLY':
          $template->startDayOfWeek = $form_state['values']['biweekly_day'];
          $template->startHour = $form_state['values']['biweekly_hour'];
          $template->startMinute = $form_state['values']['biweekly_minutes'];
          break;

        case 'MONTHLY':
          $template->startDay = $form_state['values']['monthly_day'];
          $template->startHour = $form_state['values']['monthly_hour'];
          $template->startMinute = $form_state['values']['monthly_minutes'];
          break;
      }
      $return = DrupalCultureFeed::updateTemplate($template);
      drupal_set_message(t('Template "!template_name" updated', array('!template_name' => $template->name)));
    }
    catch (Exception $e) {
      watchdog('culturefeed_mailing', $e->getMessage());
      drupal_set_message(t('Could not update template "!template_name"', array('!template_name' => $template->name)), 'error');
    }

  }
  else {
    try {
      $template = new CultureFeed_Template();
      if (!empty($form_state['values']['channel'])) {
        $template->consumerKey = $form_state['values']['channel'];
      }
      else {
        $template->consumerKey = variable_get('culturefeed_api_application_key', '');
      }
      $template->name = $form_state['values']['name'];
      $template->subject = $form_state['values']['subject'];
      $template->fromAddress = $form_state['values']['from_address'];
      $template->template = $form_state['values']['template'];
      $template->block1 = $form_state['values']['block_1'];
      $template->block2 = $form_state['values']['block_2'];
      $template->block3 = $form_state['values']['block_3'];
      $template->block4 = $form_state['values']['block_4'];
      $template->block5 = $form_state['values']['block_5'];
      $template->block6 = $form_state['values']['block_6'];
      $template->searchEnabled = $form_state['values']['search_option'];
      $template->searchQuery = $form_state['values']['search_text'];
      $template->sendEmptySearchResult = $form_state['values']['no_search_results_checkbox'];
      $template->recommendationEnabled = $form_state['values']['recommendation_option'];
      $template->recommendationQuery = $form_state['values']['recommendation_text'];
      $template->sendEmptyRecommendationResult = $form_state['values']['no_recommendations_results_checkbox'];
      // Save frequency and associated fields.
      $template->frequency = $form_state['values']['frequency'];
      switch ($form_state['values']['frequency']) {
        case 'ONCE':
          $template->scheduledDate = $form_state['values']['once_date'];
          $template->startHour = $form_state['values']['once_hour'];
          $template->startMinute = $form_state['values']['once_minutes'];
          break;

        case 'DAILY':
          $template->startHour = $form_state['values']['daily_hour'];
          $template->startMinute = $form_state['values']['daily_minutes'];
          break;

        case 'WEEKLY':
          $template->startDayOfWeek = $form_state['values']['weekly_day'];
          $template->startHour = $form_state['values']['weekly_hour'];
          $template->startMinute = $form_state['values']['weekly_minutes'];
          break;

        case 'BIWEEKLY':
          $template->startDayOfWeek = $form_state['values']['biweekly_day'];
          $template->startHour = $form_state['values']['biweekly_hour'];
          $template->startMinute = $form_state['values']['biweekly_minutes'];
          break;

        case 'MONTHLY':
          $template->startDay = $form_state['values']['monthly_day'];
          $template->startHour = $form_state['values']['monthly_hour'];
          $template->startMinute = $form_state['values']['monthly_minutes'];
          break;
      }
      DrupalCultureFeed::createTemplate($template);
      drupal_set_message(t('New template "!template_name" created', array('!template_name' => $template->name)));
    }
    catch (Exception $e) {
      watchdog('culturefeed_mailing', $e->getMessage());
      drupal_set_message(t('Could not save new template "!template_name"', array('!template_name' => $template->name)), 'error');
    }
  }

  if (!isset($_GET['destination'])) {
    drupal_goto('admin/content/templates');
  }
}

/**
 * Helper function to return array with days of the week for select element.
 *
 * @return array
 *   List of weekdays.
 */
function _culturefeed_mailing_get_weekdays() {
  $daysofweek = array(
    1 => t('Monday'),
    2 => t('Tuesday'),
    3 => t('Wednesday'),
    4 => t('Thursday'),
    5 => t('Friday'),
    6 => t('Saturday'),
    7 => t('Sunday'),
  );

  return $daysofweek;
}

/**
 * Helper function to return array with days of the month for select element.
 *
 * @return array
 *   List of days in current month.
 */
function _culturefeed_mailing_get_day_of_the_month() {
  $daysofmonth = array();

  foreach (range(1, 31) as $number) {
    $daysofmonth[$number] = $number;
  }

  return $daysofmonth;
}

/**
 * Page callback for the mailing content items markup for each block.
 */
function culturefeed_mailing_page_mailing_content() {

  $output_block_1 = '';
  $output_block_2 = '';
  $output_block_3 = '';
  $output_block_4 = '';
  $output_block_5 = '';
  $output_block_6 = '';

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'mail_item');
  // $query->propertyCondition('status', 1);
  $query->entityOrderBy('entity_id', 'DESC');
  $query->range(0, 5);
  $result = $query->execute();
  $nids = array_keys($result['node']);
  if ($nids) {
    $nodes = node_load_multiple($nids);
    foreach ($nodes as $node) {
      $link = field_view_field('node', $node, 'field_mail_item_link');
      $image = field_view_field('node', $node, 'field_mail_item_image');
      $intro = field_view_field('node', $node, 'field_mail_item_teaser');
      $region = field_view_field('node', $node, 'field_mail_item_region');

      $link_path = $link['#items'][0]['url'];
      $link_query = array($link['#items'][0]['query']);

      $utm = array(
        'utm_campaign' => 'UiTmail',
        'utm_medium' => 'email',
        'utm_source' => 'newsletter',
      );

      $utm_title = array('utm_content' => 'red-teaser-title');
      $query_title = array_merge($link_query, $utm, $utm_title);

      $utm_image = array('utm_content' => 'red-teaser-image');
      $query_image = array_merge($link_query, $utm, $utm_image);

      $utm_readon = array('utm_content' => 'red-teaser-read-more');
      $query_readon = array_merge($link_query, $utm, $utm_readon);

      $variables = array(
        'url_title' => url($link_path, array('query' => $query_title)),
        'url_image' => url($link_path, array('query' => $query_image)),
        'url_readon' => url($link_path, array('query' => $query_readon)),
        'title' => $node->title,
        'intro' => isset($intro['#items'][0]['value']) ? $intro['#items'][0]['value'] : '',
        'link_title' => $link['#items'][0]['title'],
        'image' => theme('image_style', array(
          'style_name' => 'thumbnail',
          'path' => $image['#items'][0]['uri'],
          'attributes' => array(
            'style' => 'border: none; margin:0 !important; height:100px; width: 100px;',
          ),
          'alt' => $node->title,
        )),
        'spacer' => theme_image(array(
          'path' => drupal_get_path('module', 'culturefeed_mailing') . '/theme/img/spacer.gif',
          'attributes' => array(
            'style' => 'LINE-HEIGHT: 1px; FONT-SIZE: 1px;',
            'width' => '1',
            'height' => '10',
          ),
        )),
      );

      foreach ($region['#items'] as $region_block) {
        if ($region_block['value'] == 'block_1') {
          $output_block_1 .= theme('culturefeed_mailing_content_block_1', $variables);
        }
        if ($region_block['value'] == 'block_2') {
          $output_block_2 .= theme('culturefeed_mailing_content_block_2', $variables);
        }
        if ($region_block['value'] == 'block_3') {
          $output_block_3 .= theme('culturefeed_mailing_content_block_3', $variables);
        }
        if ($region_block['value'] == 'block_4') {
          $output_block_4 .= theme('culturefeed_mailing_content_block_4', $variables);
        }
        if ($region_block['value'] == 'block_5') {
          $output_block_5 .= theme('culturefeed_mailing_content_block_5', $variables);
        }
        if ($region_block['value'] == 'block_6') {
          $output_block_6 .= theme('culturefeed_mailing_content_block_6', $variables);
        }
      }
    }

  }

  $output = '<h3>BLOCK 1</h3>';
  $output .= '<p>' . theme('textarea', array('element' => array('#value' => $output_block_1, '#rows' => 5))) . '</p><p>' . $output_block_1 . '</p>';

  $output .= '<h3>BLOCK 2</h3>';
  $output .= '<p>' . theme('textarea', array('element' => array('#value' => $output_block_2, '#rows' => 5))) . '</p><p>' . $output_block_2 . '</p>';

  $output .= '<h3>BLOCK 3</h3>';
  $output .= '<p>' . theme('textarea', array('element' => array('#value' => $output_block_3, '#rows' => 5))) . '</p><p>' . $output_block_3 . '</p>';

  $output .= '<h3>BLOCK 4</h3>';
  $output .= '<p>' . theme('textarea', array('element' => array('#value' => $output_block_4, '#rows' => 5))) . '</p><p>' . $output_block_4 . '</p>';

  $output .= '<h3>BLOCK 5</h3>';
  $output .= '<p>' . theme('textarea', array('element' => array('#value' => $output_block_5, '#rows' => 5))) . '</p><p>' . $output_block_5 . '</p>';

  $output .= '<h3>BLOCK 6</h3>';
  $output .= '<p>' . theme('textarea', array('element' => array('#value' => $output_block_6, '#rows' => 5))) . '</p><p>' . $output_block_6 . '</p>';

  return $output;

}
