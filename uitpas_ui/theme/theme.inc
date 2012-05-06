<?php

/**
 * @file
 * Uitpas preprocess functions.
 */
 
 /**
 * Implements hook_preprocess_uitpas_ui_advantage() for uitpas-ui-user-advantage.tpl.php.
 */
function uitpas_ui_preprocess_uitpas_ui_advantage(&$vars) {
  $advantage = $vars['advantage'];
  $vars['title'] = $advantage->title;
  $vars['period'] = '';
  if ($advantage->cashingPeriodBegin) {
    $period = t('From !date', array('!date' => date('j/n/Y', $advantage->cashingPeriodBegin)));
    if ($advantage->cashingPeriodEnd) {
      $period = t('to !date', array('!date' => date('j/n/Y', $advantage->cashingPeriodEnd)));
    }
    $vars['period'] = $period;
  }
  $location = array();
  foreach ($advantage->counters as $counter) {
    $location[] = $counter->name;
  }
  $vars['location'] = t('At') . " " . implode(', ', $location);
  $list = array(
    'title' => t('Valid for inhabitants of'),
    'attributes' => array(),
    'type' => 'ul',
    'items' => $advantage->validForCities,
  );
  $vars['available'] = theme_item_list($list);
}

 /**
 * Implements hook_preprocess_uitpas_ui_advantages_promotions() for uitpas-ui-user-advantages_promotions.tpl.php.
 */
function uitpas_ui_preprocess_uitpas_ui_advantages_promotions(&$vars) {
  $advantages = $vars['advantages'];
  $advantages_total = $vars['advantages_total'];
  $promotions = $vars['promotions'];
  $promotions_total = $vars['promotions_total'];
  $base_table = array(
    'attributes' => array(),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => '',
    'empty' => '',
  );
  $base_pager = array('tags' => array(), 'parameters' => array());
  //advantages
  $header = array(t('Welcome advantages'), t('valid till'), '');
  $rows = array();
  if (count($advantages)) {
    foreach ($advantages as $advantage) {
      $location = array();
      foreach ($advantage->counters as $counter) {
        $location[] = $counter->name;
      }
      $rows[] = array(
        $advantage->title,
        ($advantage->cashingPeriodEnd) ? date('j/n/Y', $advantage->cashingPeriodEnd) : '-',
        implode(', ', $location)
      );
    }
  }
  else {
    $rows[] = array(array('data' => t('No results found'), 'colspan' => 3));
  }
  $table = $base_table + array('header' => $header, 'rows' => $rows);
  $pager = $base_pager + array('element' => 0, 'quantity' => $advantages_total);
  $vars['advantages'] = theme_table($table) . theme_pager($pager);
  //promotions
  $header = array(t('Promotions'), t('valid till'), '');
  $rows = array();
  if (count($promotions)) {
    foreach ($promotions as $promotion) {
      $location = array();
      foreach ($promotion->counters as $counter) {
        $location[] = $counter->name;
      }
      $rows[] = array(
        $promotion->title,
        ($promotion->cashingPeriodEnd) ? date('j/n/Y', $promotion->cashingPeriodEnd) : '-',
        implode(', ', $location)
      );
    }
  }
  else {
    $rows[] = array(array('data' => t('No results found'), 'colspan' => 3));
  }
  $table = $base_table + array('header' => $header, 'rows' => $rows);
  $pager = $base_pager + array('element' => 1, 'quantity' => $promotions_total);
  $vars['promotions'] = theme_table($table) . theme_pager($pager);
  //info
  $vars['info'] = variable_format_value('uitpas_ui_advantages_promotions_info');
}

/**
 * Implements hook_preprocess_uitpas_ui_promotion() for uitpas-ui-user-promotion.tpl.php.
 */
function uitpas_ui_preprocess_uitpas_ui_promotion(&$vars) {
  $promotion = $vars['promotion'];
  $vars['title'] = $promotion->title;
  $vars['points'] = t('Points needed') . ':' . t('!points points', array('!points' => $promotion->points));
  $vars['period'] = '';
  if ($promotion->cashingPeriodBegin) {
    $period = t('From !date', array('!date' => date('j/m/Y', $promotion->cashingPeriodBegin)));
    if ($promotion->cashingPeriodEnd) {
      $period .= ' ' . t('to !date', array('!date' => date('j/m/Y', $promotion->cashingPeriodEnd)));
    }
    $vars['period'] = $period;
  }
  $location = array();
  foreach ($promotion->counters as $counter) {
    $location[] = $counter->name;
  }
  $vars['location'] = t('At') . " " . implode(', ', $location);
  $list = array(
    'title' => t('Valid for inhabitants of'), 
    'attributes' => array(), 
    'type' => 'ul', 
    'items' => $promotion->validForCities,
  );
  $vars['available'] = theme_item_list($list);
}

/**
 * Implements hook_preprocess_uitpas_ui_recent_actions() for uitpas-ui-user-recent-actions.tpl.php.
 */
function uitpas_ui_preprocess_uitpas_ui_recent_actions(&$vars) {
  $vars['available'] = theme_item_list($list);
}

/**
 * Implements hook_preprocess_uitpas_ui_user_profile() for uitpas-ui-user-profile.tpl.php.
 */
function uitpas_ui_preprocess_uitpas_ui_user_profile(&$vars) {
  $uitpas_user = $vars['uitpas_user'];
  $passholder = $vars['passholder'];
  $advantages = $vars['advantages'];
  $promotions = $vars['promotions'];
  $nearby_promotions = $vars['nearby_promotions'];
  if (isset($uitpas_user->depiction)) {
    $vars['image'] = theme_image(array('path' => $uitpas_user->depiction, 'attributes' => array()));
  }
  else {
    $vars['image'] = '';
  }
  $vars['name'] = l($uitpas_user->nick, 'myprofile');
  $vars['points'] = t('%points points saved', array('%points' => $passholder->points));
  //advantages and promotions
  $links = array('links' => array(), 'heading' => array(), 'attributes' => array());
  //advantages
  foreach ($advantages as $advantage) {
    $links['links'][] = array(
      'title' => $advantage->title,
      'href' => 'advantage/' . $advantage->id,
    );
  }
  //promotions
  foreach ($promotions as $promotion) {
    $links['links'][] = array(
      'title' => $promotion->title,
      'href' => 'promotion/' . $promotion->id,
    );
  }
  $vars['advantages_promotions'] = theme_links($links);
  //nearby promotions
  $links = array('links' => array(), 'heading' => array(), 'attributes' => array());
  foreach ($nearby_promotions as $promotion) {
    $links['links'][] = array(
      'title' => $promotion->title,
      'href' => 'promotion/' . $promotion->id,
    );
  }
  $vars['nearby_promotions'] = theme_links($links);
}

/**
 * Implements hook_preprocess_uitpas_ui_user_profile_activities() for uitpas-ui-user-profile-activities.tpl.php.
 */
function uitpas_ui_preprocess_uitpas_ui_user_profile_activities(&$vars) {
  $activities = $vars['activities'];
  $vars['title'] = t('My activities');
  $header = array(t('Date'), t('Activity name'), t('Location'), t('Points'));
  $rows = array();
  foreach ($activities as $activity) {
    $rows[] = array(
      date('j/m/Y', $activity->date),
      $activity->name,
      $location,
      t('!points points', array('!points' => $activity->points)),
    );
  }
  $table = array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array(),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => '',
    'empty' => '',
  );
  $vars['activities'] = theme_table($table);
}

/**
 * Implements hook_preprocess_uitpas_ui_user_profile_advantages() for uitpas-ui-user-profile-advantages.tpl.php.
 */
function uitpas_ui_preprocess_uitpas_ui_user_profile_advantages(&$vars) {
  $passholder = $vars['passholder'];
  $advantages = $vars['advantages'];
  $promotions = $vars['promotions'];
  $cashed_in_advantages = $vars['cashed_in_advantages'];
  $cashed_in_promotions = $vars['cashed_in_promotions'];
  $vars['title'] = t('My advantages (!points points saved)', array('!points' => $passholder->points));
  $base_table = array(
    'attributes' => array(),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => '',
    'empty' => '',
  );
  //promotions
  $header = array(
    array('data' => t('Trade-in options'), 'colspan' => 2),
    array('data' => t('valid till'), 'colspan' => 2),
  );
  $rows = array();
  foreach ($promotions as $promotion) {
    $location = array();
    foreach ($promotion->counters as $counter) {
      $location[] = $counter->name;
    }
    $rows[] = array(
      t('!points points', array('!points' => $promotion->points)),
      l($promotion->title, 'promotion/' . $promotion->id),
      ($promotion->cashingPeriodEnd) ? date('j/n/Y', $promotion->cashingPeriodEnd) : '-',
      implode(', ', $location)
    );
  }
  $table = $base_table + array('header' => $header, 'rows' => $rows);
  $vars['promotions'] = theme_table($table);
  //advantages
  $header = array(
    array('data' => t('Advantages')),
    array('data' => t('valid till'), 'colspan' => 2),
  );
  $rows = array();
  foreach ($advantages as $advantage) {
    $location = array();
    foreach ($advantage->counters as $counter) {
      $location[] = $counter->name;
    }
    $rows[] = array(
      l($advantage->title, 'advantage/' . $advantage->id),
      ($promotion->cashingPeriodEnd) ? date('j/n/Y', $promotion->cashingPeriodEnd) : '-',
      implode(', ', $location)
    );
  }
  $table = $base_table + array('header' => $header, 'rows' => $rows);
  $vars['advantages'] = theme_table($table);
  //cashed in promotions and advantages
  $header = array(
    array('data' => t('History of cashed in advantages and promotions'), 'colspan' => 3),
    array('data' => t('Cashed in on ')),
  );
  $rows = array();
  //cashed in promotions
  foreach ($cashed_in_promotions as $promotion) {
    $location = array();
    foreach ($promotion->counters as $counter) {
      $location[] = $counter->name;
    }
    $rows[] = array(
      t('!points points', array('!points' => $promotion->points)),
      l($promotion->title, 'promotion/' . $promotion->id),
      implode(', ', $location),
      '?',
    );
  }
  //cashed in advantages
  foreach ($cashed_in_advantages as $advantage) {
    $location = array();
    foreach ($advantage->counters as $counter) {
      $location[] = $counter->name;
    }
    $rows[] = array(
      t('Welcome'),
      l($advantage->title, 'advantage/' . $advantage->id),
      implode(', ', $location),
      '?',
    );
  }
  $table = $base_table + array('header' => $header, 'rows' => $rows);
  $vars['cashed_in_promotions_advantages'] = theme_table($table);
}

/**
 * Implements hook_preprocess_uitpas_ui_user_profile_advantages() for uitpas-ui-user-profile-advantages.tpl.php.
 */
function uitpas_ui_preprocess_uitpas_ui_user_profile_details(&$vars) {
  $uitpas_user = $vars['uitpas_user'];
  $passholder = $vars['passholder'];
  $vars['title'] = t('My data');
  if (isset($uitpas_user->depiction)) {
    $vars['image'] = theme_image(array('path' => $uitpas_user->depiction, 'attributes' => array()));
  }
  else {
    $vars['image'] = '';
  }
  $vars['active_since'] = '<label>' . t('Active since') . ':</label> ' . '?';
  $vars['uitpas_number'] = '<label>' . t('UITpas number') . ':</label> ' . $passholder->uitpasNumber;
  $vars['intro'] = variable_format_value('uitpas_ui_user_profile_details_intro');
  $vars['national_identification_number'] = '<label>' . t('National identification number') . ':</label> ' . $passholder->inszNumber;
  $vars['first_name'] = '<label>' . t('First name') . ':</label> ' . $passholder->firstName;
  $vars['last_name'] = '<label>' . t('Name') . ':</label> ' . $passholder->name;
  $vars['dob'] = '<label>' . t('Date of birth') . ':</label> ' . date('j/m/Y', $passholder->dateOfBirth);
  $vars['pob'] = '<label>' . t('Place of birth') . ':</label> ' . $passholder->placeOfBirth;
  $vars['gender'] = '<label>' . t('Gender') . ':</label> ' . $passholder->gender;
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
  $status_end_date = t('valid till !date', array('j/m/Y' => date('', $passholder->kansenStatuutEndDate)));
  $vars['status_valid_till'] = '<label>' . t('Opportunity status') . ':</label> ' . $status_end_date;
  //$ocmw_end_date = t('valid till !date', array('j/m/Y' => date('', $passholder->kansenStatuutEndDate)));
  $vars['ocmw_valid_till'] = '<label>' . t('OCMW status') . ':</label> ' . '?';
}

/**
 * Implements hook_preprocess_uitpas_ui_user_profile_uitid() for uitpas-ui-user-profile-uitid.tpl.php.
 */
function uitpas_ui_preprocess_uitpas_ui_user_profile_uitid(&$vars) {
  $uitpas_user = $vars['uitpas_user'];
  $passholder = $vars['passholder'];
  $actions = $vars['actions'];
  if (isset($uitpas_user->depiction)) {
    $vars['image'] = theme_image(array('path' => $uitpas_user->depiction, 'attributes' => array()));
  }
  else {
    $vars['image'] = '';
  }
  $vars['first_name'] = $passholder->firstName;
  $vars['dob'] = date('j/m/Y', $passholder->dateOfBirth);
  $vars['pob'] = $passholder->placeOfBirth;
  $vars['gender'] = $passholder->gender;
  $vars['more_info'] = $passholder->moreInfo;
  $list = array('items' => array(), 'title' => t('Recent UITID actions'), 'type' => 'ul', 'attributes' => array());
  foreach ($actions as $action) {
    $list['items'][] = $action->name . '<br />' .  date('j/m/Y', $action->date);
  }
  $vars['actions'] = theme_item_list($list);
  $vars['form'] = drupal_render($vars['form']);
  $vars['info'] = variable_format_value('uitpas_ui_user_profile_uitid_info');
}