<?php

/**
 * @file
 * Uitpas preprocess functions.
 */
 
/**
 * Returns html for admin promotions highlight form
 */
function theme_uitpas_ui_admin_promotions_highlight_form($vars) {
  $form = $vars['form'];
  $weight_class = 'uitpas_ui_promotions_highlight_weight';
  $table_id = 'uitpas_ui_promotions_highlight_weight';
  $table = array(
    'header' => array(t('Promotion'), t('Weight')),
    'rows' => array(),
    'attributes' => array('id' => $table_id),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => '',
    'empty' => '',
  );
  $rows = array();
  foreach (element_children($form['promotions']) as $element) {
    $form['promotions'][$element]['weight']['#attributes']['class'] = array($weight_class);
    $table['rows'][] = array(
      'data' => array(
        drupal_render($form['promotions'][$element]['value']),
        drupal_render($form['promotions'][$element]['weight']),
      ),
      'class' => array('draggable'),
    );
  }
  $output = theme_table($table);
  $output .= drupal_render_children($form);
  drupal_add_tabledrag($table_id, 'order', 'sibling', $weight_class);
  return $output;
}

/**
 * Implements hook_preprocess_uitpas_ui_advantage() for uitpas-ui-user-advantage.tpl.php.
 */
function uitpas_ui_preprocess_uitpas_ui_activity(&$vars) {
  $activity = $vars['event'] = $vars['activity'];
  $uitpas_activity = $vars['uitpas_activity'];
  template_preprocess_cnapi_ui_event($vars);
  //icons and legend are the same html wise for accessibility reasons
  $list = array(
    'title' => '',
    'attributes' => array(),
    'type' => 'ul',
    'items' => array(),
  );
  if ($uitpas_activity && $uitpas_activity->numberOfPoints) {
    $singular = t('Save point');
    $plural = t('Save !points points', array('!points' => $uitpas_activity->numberOfPoints));
    $list['items'][] = array(
      'data' => format_plural($uitpas_activity->numberOfPoints, $singular, $plural),
      'class' => array('uitpas_activity_has_points'),
    );
  }
  if ($vars['promotions']) {
    $link = 'actor/' . $activity['location']['actor']['cdbid'];
    $list['items'][] = array(
      'data' => l($vars['location']['title'], $link),
      'class' => array('uitpas_activity_has_promotion'),
    );
  }
  if ($uitpas_activity && $uitpas_activity->tariff < $uitpas_activity->price) {
    $list['items'][] = array(
      'data' => t('Reduction for opportunity groups'),
      'class' => array('uitpas_activity_has_tariff'),
    );
    $vars['price'] = "&euro; " . $uitpas_activity->tariff;
  }
  $vars['icons'] = theme_item_list($list);
  //headings
  $list = array(
    'title' => '',
    'attributes' => array(),
    'type' => 'ul',
    'items' => $vars['headings'],
  );
  $vars['headings'] = theme_item_list($list);
  //when
  $vars['when'] = "<label>" . t('When?') . "</label> " . $vars['when'];
  //where
  $where = l($vars['location']['title'], 'actor/' . $activity['location']['actor']['cdbid']) . "<br />";
  $where .= $vars['location']['address'];
  $vars['where'] = "<label>" . t('Where?') . "</label> " . $where;
  //price
  $vars['price'] = "<label>" . t('Price') . "</label> " . $vars['price'];
  //links
  $list = array(
    'title' => '',
    'attributes' => array(),
    'type' => 'ul',
    'items' => $vars['links'],
  );
  $vars['links'] = "<label>" . t('Links') . "</label> " . theme_item_list($list);
  //image
  $vars['image'] = (isset($vars['images'][0]['image'])) ? $vars['images'][0]['image'] : '';
}

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
  $singular = t('1 Point needed');
  $plural =  t('Points needed') . ':' . t('!points points', array('!points' => $promotion->points));
  $vars['points'] = format_plural($promotion->points, $singular, $plural);
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
 * Implements hook_preprocess_uitpas_ui_promotions_highlight() for uitpas-ui-user-promotions_highlight.tpl.php.
 */
function uitpas_ui_preprocess_uitpas_ui_promotions_highlight(&$vars) {
  $promotions = $vars['promotions'];
  $rows = array();
  foreach ($promotions as $promotion) {
    $singular = t('1 point');
    $plural = t('!points points', array('!points' => $promotion->points));
    $rows[] = array(
      format_plural($promotion->points, $singular, $plural),
      l($promotion->title, 'promotion/' . $promotion->id),
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
}

/**
 * Implements hook_preprocess_uitpas_ui_recent_actions() for uitpas-ui-user-recent-actions.tpl.php.
 */
function uitpas_ui_preprocess_uitpas_ui_recent_actions(&$vars) {
  $actions = $vars['actions'];
  $list = array(
    'title' => '',
    'attributes' => array(),
    'type' => 'ul',
    'items' => array(),
  );
  foreach ($actions as $action) {
    $args = array(
      '!name' => ($action->nick) ? $action->nick : t('Anonymous'),
      '!points' => $action->points,
      '!location' => $action->createdVia,
      '!time_ago' => format_interval(time() - $action->creationDate, 1),
    );
    $singular = t('!name saved 1 point at !location, !time_ago ago', $args);
    $plural = t('!name saved !points points at !location, !time_ago ago', $args);
    $image = ($action->depiction) ? theme_image(array('path' => $action->depiction, 'attributes' => array())) : '';
    $list['items'][] = $image . format_plural($action->points, $singular, $plural);
  }
  
  $vars['actions'] = theme_item_list($list);
  $vars['form'] = (count($vars['form'])) ? drupal_render($vars['form']) : '';
}

/**
 * Implements hook_preprocess_uitpas_ui_user_profile() for uitpas-ui-user-profile.tpl.php.
 */
function uitpas_ui_preprocess_uitpas_ui_user_profile(&$vars) {
  $uitpas_user = $vars['uitpas_user'];
  $passholder = $vars['passholder'];
  $advantages = $vars['advantages'];
  $promotions = $vars['promotions'];
  $coming_promotions = $vars['coming_promotions'];
  if (isset($uitpas_user->depiction)) {
    $vars['image'] = theme_image(array('path' => $uitpas_user->depiction, 'attributes' => array()));
  }
  else {
    $vars['image'] = '';
  }
  $vars['name'] = l($uitpas_user->nick, 'myprofile/uitid');
  $singular = t('1 point saved');
  $plural = t('%points points saved', array('%points' => $passholder->points));
  $vars['points'] = format_plural($passholder->points, $singular, $plural);
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
  //coming promotions
  $links = array('links' => array(), 'heading' => array(), 'attributes' => array());
  foreach ($coming_promotions as $promotion) {
    $links['links'][] = array(
      'title' => $promotion->title . ' (' . t('!points points', array('!points' => $promotion->points)) . ')',
      'href' => 'promotion/' . $promotion->id,
    );
  }
  $vars['coming_promotions'] = (count($coming_promotions)) ? theme_links($links) : '';
  $vars['all_promotions'] = l(t('Show all promotions'), 'myprofile/advantages');
}

/**
 * Implements hook_preprocess_uitpas_ui_user_profile_activities() for uitpas-ui-user-profile-activities.tpl.php.
 */
function uitpas_ui_preprocess_uitpas_ui_user_profile_activities(&$vars) {
  $activities = $vars['activities'];
  $activities_total = $vars['activities_total'];
  $header = array(t('Date'), t('Activity name'), t('Location'), t('Points'));
  $rows = array();
  if (count($activities)) {
    foreach ($activities as $activity) {
      $singular = t('1 point');
      $plural = t('!points points', array('!points' => $activity->points));
      $rows[] = array(
        date('j/m/Y', $activity->creationDate),
        $activity->nodeTitle,
        $activity->location,
        format_plural($activity->points, $singular, $plural),
      );
    }
  }
  else {
    $rows[] = array(array('data' => t('No results found.'), 'colspan' => 4));
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
  $pager = array('tags' => array(), 'parameters' => array(), 'element' => 0, 'quantity' => $activities_total);
  $vars['activities'] = theme_table($table) . theme_pager($pager);
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
  if (count($promotions)) {
    foreach ($promotions as $promotion) {
      $location = array();
      foreach ($promotion->counters as $counter) {
        $location[] = $counter->name;
      }
      $singular = t('1 point');
      $plural = t('!points points', array('!points' => $promotion->points));
      $rows[] = array(
        format_plural($promotion->points, $singular, $plural),
        l($promotion->title, 'promotion/' . $promotion->id),
        ($promotion->cashingPeriodEnd) ? date('j/n/Y', $promotion->cashingPeriodEnd) : '-',
        implode(', ', $location)
      );
    }
  }
  else {
    $rows[] = array(array('data' => t('No results found.'), 'colspan' => 3));
  }
  $table = $base_table + array('header' => $header, 'rows' => $rows);
  $vars['promotions'] = theme_table($table);
  //advantages
  $header = array(
    array('data' => t('Advantages')),
    array('data' => t('valid till'), 'colspan' => 2),
  );
  $rows = array();
  if (count($advantages)) {
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
  }
  else {
    $rows[] = array(array('data' => t('No results found.'), 'colspan' => 3));
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
    $singular = t('1 point');
    $plural = t('!points points', array('!points' => $promotion->points));
    $rows[] = array(
      format_plural($promotion->points, $singular, $plural),
      l($promotion->title, 'promotion/' . $promotion->id),
      $promotion->counters[0]->name,
      date('j/n/Y', $promotion->cashingDate),
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
      $advantage->counters[0]->name,
      date('j/n/Y', $advantage->cashingDate),
    );
  }
  if (!count($rows)) {
    $rows[] = array(array('data' => t('No results found'), 'colspan' => 4));
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
  if (isset($uitpas_user->depiction)) {
    $vars['image'] = theme_image(array('path' => $uitpas_user->depiction, 'attributes' => array()));
  }
  else {
    $vars['image'] = '';
  }
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
    $list['items'][] = $action->nodeTitle . '<br />' .  date('j/m/Y', date('j/m/Y', $action->creationDate));
  }
  $vars['actions'] = theme_item_list($list);
  $vars['form'] = drupal_render($vars['form']);
  $vars['info'] = variable_format_value('uitpas_ui_user_profile_uitid_info');
}