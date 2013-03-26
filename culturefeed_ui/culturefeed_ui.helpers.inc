<?php

/**
 * Put noindex and canonical metatags for SEO on relevant culturefeed pages.
 */
function culturefeed_ui_set_canonical_and_noindex() {
  $canonical = FALSE;
  $noindex = FALSE;

  if (arg(0) == 'culturefeed') {
    if (arg(1) == 'activities' && arg(2) == 'recent') {
      $noindex = TRUE;
    }
    if (arg(1) == 'users' && arg(2) == 'search') {
      $noindex = TRUE;
    }
  }

  // Add the "noindex, follow" meta tag if needed.
  if ($noindex) {
    $element = array(
      '#tag' => 'meta',
      '#attributes' => array(
        'name' => 'robots',
        'content' => 'noindex, follow',
      ),
    );

    drupal_add_html_head($element, 'nofollow');
  }

  // Add the rel="canonical" tag if needed.
  if ($canonical) {
    $canonical['options']['absolute'] = TRUE;
    drupal_add_html_head_link(array('rel' => 'canonical', 'href' => url($canonical['path'], $canonical['options'])), TRUE);
  }
}

function culturefeed_ui_block_ajaxload($config) {
  $block['subject'] = $config['title'];
  $block['content'] = array(
    '#markup' => '<div id="' . $config['id'] . '">Bezig met laden...</div>',
    '#id' => $config['id'],
    '#ajax' => array(
      'event' => 'ajaxload',
      'path' => $config['path'],
    ),
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'culturefeed_ui') . '/js/culturefeed_ui.js',
        array('data' => array(), 'type' => 'setting'),
      ),
    ),
  );

  $block['content'] = ajax_pre_render_element($block['content']);

  return $block;
}

function culturefeed_ui_is_culturefeed_profile_page() {
  $account = menu_get_object('user');

  if (arg(0) == 'user' && is_numeric(arg(1)) && !arg(2) && DrupalCultureFeed::isCultureFeedUser($account->uid)) {
    return TRUE;
  }

  return FALSE;
}


function culturefeed_ui_online_account($online_account_type, $online_account = NULL) {
  $delete_link = '';

  if ($online_account) {
    $delete_link = array(
      '#type' => 'link',
      '#title' => 'Verwijderen',
      '#href' => 'culturefeed/onlineaccounts/delete/' . $online_account_type . '/' . $online_account->accountName . '/nojs',
      '#attributes' => array('class' => 'delete-link'),
      '#options' => array('query' => drupal_get_destination()),
      '#ajax' => array(),
    );

    $delete_link = drupal_render($delete_link);
  }

  $statement = 'Ik geef toestemming aan UiTiD om mijn acties op ' . check_plain(variable_get('site_name', 'Drupal')) . ' automatisch te publiceren op ' . $online_account_type . '. ' . l('Gebruiksvoorwaarden', 'node/2512');

  $publish_link = '';
  if ($online_account) {
    $publish_link = array(
      '#type' => 'link',
      '#title' => $online_account->publishActivities ? 'Publiek' : 'Privé',
      '#href' => 'culturefeed/onlineaccounts/privacy/' . $online_account_type . '/' . $online_account->accountName . '/' . ($online_account->publishActivities ? '0' : '1') . '/nojs',
      '#attributes' => array('id' => 'onlineaccount-privacy-' . $online_account->accountName, 'class' => 'privacy-link ' . ($online_account->publishActivities ? 'status-publiek' : 'status-prive'), 'title' => ($online_account->publishActivities ? 'Zet uit' : 'Zet aan')),
      '#options' => array('query' => drupal_get_destination()),
      '#ajax' => array(),
    );

    $publish_link = drupal_render($publish_link) . $statement;
  }

  $redirect = url('culturefeed/account/edit', array('absolute' => TRUE, 'query' => array('closepopup' => 'true')));
  $connect_link = l('Account toevoegen', DrupalCultureFeed::getUrlAddSocialNetwork($online_account_type, $redirect), array('attributes' => array('class' => 'culturefeedconnect')));

  $vars = array(
    'account_type'     => $online_account_type,
    'account'          => $online_account,
    'publish_link'     => $publish_link,
    'delete_link'      => $delete_link,
    'connect_link'     => $connect_link,
  );

  return theme('culturefeed_ui_online_account', $vars);
}

function culturefeed_ui_activity_types() {
  return array(
    CultureFeed_Activity::TYPE_LIKE     => 'Vind ik leuk',
    CultureFeed_Activity::TYPE_IK_GA    => 'Ga hier naartoe',
    CultureFeed_Activity::TYPE_FACEBOOK => 'Gedeeld op Facebook',
    CultureFeed_Activity::TYPE_TWITTER  => 'Gedeeld op Twitter',
  );
}

function culturefeed_ui_users_list($users) {
  $items = array();

  $accounts = culturefeed_get_uids_for_users($users->objects);

  foreach ($users->objects as $user) {
    if (!isset($accounts[$user->id])) {
      continue;
    }

    $items[] = theme('culturefeed_ui_user_summary', array('user' => $user, 'uid' => $accounts[$user->id]));
  }

  return $items;
}


/**
 * Helper function to build facebook privacy toggle
 */
function _culturefeed_ui_facebook_privacy_toggle() {
  try {
    $online_account = DrupalCultureFeed::getUserOnlineAccount('facebook');
    if ($online_account) {
      $publish_link = array(
        '#type' => 'link',
        '#title' => $online_account->publishActivities ? 'Publiek' : 'Privé',
        '#href' => 'culturefeed/toggle/privacy/facebook/' . $online_account->accountName . '/' . ($online_account->publishActivities ? '0' : '1') . '/nojs',
        '#attributes' => array('id' => 'onlineaccount-privacy-' . $online_account->accountName, 'class' => 'privacy-link ' . ($online_account->publishActivities ? 'status-publiek' : 'status-prive'), 'title' => ($online_account->publishActivities ? 'Zet uit' : 'Zet aan')),
        '#options' => array('query' => drupal_get_destination()),
        '#ajax' => array(),
      );

      $publish_link = drupal_render($publish_link) . 'Publiceer mijn acties op Facebook <div class="tooltip">?</div><div class="tooltip-text">Hiermee zullen je acties (vind ik leuk, ga ik naartoe,...), automatisch op je Facebook Timeline verschijnen.</div>';

      return theme('culturefeed_ui_online_account', array('publish_link' => $publish_link));
    }
    else {
      return NULL;
    }
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    drupal_set_message('Er is een fout opgetreden.', 'error');
    return;
  }
}
