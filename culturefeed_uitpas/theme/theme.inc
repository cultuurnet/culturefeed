<?php

/**
 * @file
 * Culturefeed uitpas preprocess functions.
 */

/**
 * Implements hook_preprocess_preprocess_culturefeed_uitpas_profile_details().
 */
function culturefeed_uitpas_preprocess_culturefeed_uitpas_profile_details(&$vars) {

  $uitpas_user = $vars['uitpas_user'];
  $passholder = $uitpas_user->passholder;
  $card_system = $uitpas_user->card_system;

  // Details.
  $vars['uitpas_number'] = isset($card_system) ? '<label>' . t('UITpas number') . ':</label> ' . $card_system->currentCard->uitpasNumber : NULL;
  $vars['intro'] = variable_get('culturefeed_uitpas_profile_details_intro');
  $vars['first_name'] = '<label>' . t('First name') . ':</label> ' . $passholder->firstName;
  $vars['last_name'] = '<label>' . t('Name') . ':</label> ' . $passholder->name;
  $vars['dob'] = '<label>' . t('Date of birth') . ':</label> ' . date('j/m/Y', $passholder->dateOfBirth);
  $vars['pob'] = '<label>' . t('Place of birth') . ':</label> ' . $passholder->placeOfBirth;
  $vars['gender'] = '<label>' . t('Gender') . ':</label> ' . t(($passholder->gender == 'MALE') ? 'Male' : 'Female');
  $vars['nationality'] = '<label>' . t('Nationality') . ':</label> ' . $passholder->nationality;
  $vars['street'] = '<label>' . t('Street') . ':</label> ' . $passholder->street;
  $vars['nr'] = '<label>' . t('Nr') . ':</label> ' . $passholder->number;
  $vars['bus'] = '<label>' . t('Bus') . ':</label> ' . $passholder->box;
  $vars['zip'] = '<label>' . t('Zip') . ':</label> ' . $passholder->postalCode;
  $vars['city'] = '<label>' . t('City') . ':</label> ' . $passholder->city;
  $vars['tel'] = '<label>' . t('Telephone') . ':</label> ' . $passholder->telephone;
  $vars['mobile'] = '<label>' . t('Mobile') . ':</label> ' . $passholder->gsm;
  $vars['email'] = '<label>' . t('Email') . ':</label> ' . $passholder->email;
  $vars['status_title'] = t('Status');
  $vars['kansen_statuut'] = $passholder->kansenStatuut;
  $vars['kansen_statuut_valid_end_date'] = (time() < $passholder->kansenStatuutEndDate);
  $status_end_date = t('valid till !date', array('!date' => date('j/m/Y', $passholder->kansenStatuutEndDate)));
  $vars['status_valid_till'] = '<label>' . t('Opportunity status') . ':</label> ' . $status_end_date;
  if (count($passholder->memberships)) {
    $memberships = array();
    foreach ($passholder->memberships as $membership) {
      $endate = t('valid till !date', array('!date' => date('j/m/Y', $membership->endDate)));
      $memberships[] = '<label>' . $membership->association . ':</label> ' . $endate;
    }
    $vars['memberships'] = implode('<br />', $memberships);
  }
  else {
    $vars['memberships'] = '';
  }
  $vars['outro'] = variable_get('culturefeed_uitpas_profile_details_outro');

}

/**
 * Implements hook_preprocess_culturefeed_uitpas_promotions_highlight().
 */
function culturefeed_uitpas_preprocess_culturefeed_uitpas_promotions_highlight(&$vars) {

  $promotions = $vars['promotions'];
  $rows = array();

  foreach ($promotions as $promotion) {

    $singular = '%points point';
    $plural = '%points points';
    $rows[] = array(
      array(
        'data' => format_plural($promotion->points, $singular, $plural, array('%points' => $promotion->points)),
        'class' => array('points'),
      ),
      $promotion->title,
    );
  }

  $table = array(
    'header' => array(),
    'rows' => $rows,
    'attributes' => array(),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => '',
    'empty' => '',
  );

  $vars['promotions'] = theme_table($table);
  $more_text = variable_get('culturefeed_uitpas_promotions_highlight_more_link_text', t('Show all promotions'));
  $vars['more'] = l($more_text, 'advantages_promotions');

}

/**
 * Implements hook_preprocess_culturefeed_uitpas_recent_actions().
 */
function culturefeed_uitpas_preprocess_culturefeed_uitpas_recent_actions(&$vars) {

  $actions = $vars['actions'];
  $list = array(
    'title' => '',
    'attributes' => array(),
    'type' => 'ul',
    'items' => array(),
  );

  foreach ($actions as $action) {

    // Subject.
    $subject = $action->nodeTitle;
    // Args.
    $args = array(
      '!name' => ($action->nick) ? $action->nick : t('Anonymous'),
      '!points' => $action->points,
      '!location' => $action->createdVia,
      '!time_ago' => format_interval(time() - $action->creationDate, 1),
      '!subject' => $subject,
    );

    $singular = "<strong>!name</strong> saved 1 point at !subject, <span class=\"time-ago\">!time_ago ago</span>";
    $plural = "<strong>!name</strong> saved !points points at !subject, <span class=\"time-ago\">!time_ago ago</span>";
    $image = ($action->depiction) ? $action->depiction : variable_get('culturefeed_uitpas_user_default_image');
    if ($image) {
      $image = theme_image(array('path' => $image, 'attributes' => array()));
    }

    $list['items'][] = $image . format_plural($action->points, $singular, $plural, $args);

  }

  $vars['actions'] = theme_item_list($list);

}
