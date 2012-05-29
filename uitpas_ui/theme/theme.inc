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
 * Implements hook_preprocess_uitpas_ui_activity() for uitpas-ui-activity.tpl.php.
 */
function uitpas_ui_preprocess_uitpas_ui_activity(&$vars) {
  $activity = $vars['event'] = $vars['activity'];
  $uitpas_activity = $vars['uitpas_activity'];
  template_preprocess_cnapi_ui_event($vars);
  //icons and legend are the same html wise for accessibility reasons
  $list = array(
    'title' => t('UITPAS advantages'),
    'attributes' => array(),
    'type' => 'ul',
    'items' => array(),
  );
  if ($uitpas_activity && $uitpas_activity->numberOfPoints) {
    $singular = 'Save point';
    $plural = 'Save !points points';
    $options = array('!points' => $uitpas_activity->numberOfPoints);
    $list['items'][] = array(
      'data' => format_plural($uitpas_activity->numberOfPoints, $singular, $plural, $options),
      'class' => array('uitpas_activity_has_points'),
    );
  }
  if ($vars['promotions']) {
    $path = uitpas_ui_helpers_link_object('actor', $activity['location']['actor']);
    $list['items'][] = array(
      'data' => l(t('Show advantages at this location'), $path),
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
  $vars['icons'] = (count($list['items'])) ? theme_item_list($list) : '';
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
  $path = uitpas_ui_helpers_link_object('actor', $activity['location']['actor']);
  $where = l($vars['location']['title'], $path) . "<br />";
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
 * Implements hook_preprocess_uitpas_ui_actor() for uitpas-ui-actor.tpl.php.
 */
function uitpas_ui_preprocess_uitpas_ui_actor(&$vars) {
  $actor = $vars['actor'];
  $promotions = $vars['promotions'];
  template_preprocess_cnapi_ui_actor($vars);
  //icons and legend are the same html wise for accessibility reasons
  $icons_list = $legend_list = array(
    'title' => t('UITPAS advantages at this location'),
    'attributes' => array(),
    'type' => 'ul',
    'items' => array(),
  );
  if ($vars['points']) {
    $icons_list['items'][] = $legend_list['items'][] = array(
      'data' => t('Save points'),
      'class' => array('uitpas_actor_has_points'),
    );
  }
  if (count($promotions)) {
    $icons_list['items'][] = array(
      'data' => t('Has promotions'),
      'class' => array('uitpas_actor_has_promotions'),
    );
    foreach ($promotions as $promotion) {
      $legend_list['items'][] = array(
        'data' => l($promotion->title, uitpas_ui_helpers_link_object('promotion', $promotion)),
        'class' => array('uitpas_actor_has_promotions'),
      );
    }
  }
  $vars['icons'] = (count($icons_list['items'])) ? theme_item_list($icons_list) : '';
  $vars['legend'] = theme_item_list($legend_list) . l(t('Show all advantages and promotions'), 'advantages_promotions');
  //address
  $vars['address'] = "<label>" . t('Address') . "</label> " . $vars['address'];
  //contact
  $contact = (isset($vars['contact']['mail'])) ? $vars['contact']['mail'] . "<br />" : '';
  $contact .= (isset($vars['contact']['phone'])) ? t('telephone') . ': ' . $vars['contact']['phone'] . "<br />" : '';
  $contact .= (isset($vars['contact']['fax'])) ? t('fax') . ': ' . $vars['contact']['fax'] : '';
  $vars['contact'] = ($contact) ? "<label>" . t('Contact') . "</label> " . $contact : '';
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
 * Implements hook_preprocess_uitpas_ui_advantage() for uitpas-ui-advantage.tpl.php.
 */
function uitpas_ui_preprocess_uitpas_ui_advantage(&$vars) {
  $advantage = $vars['advantage'];
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
  $location_options = variable_get_value('uitpas_ui_locations');
  $location_options = array_map('trim', explode("\n", $location_options));
	$valid_for_cities = array_intersect($location_options, $advantage->validForCities);
  asort($valid_for_cities);
  $list = array(
    'title' => t('Valid for inhabitants of'),
    'attributes' => array(),
    'type' => 'ul',
    'items' => $valid_for_cities,
  );
  $vars['available'] = theme_item_list($list);
}

 /**
 * Implements hook_preprocess_uitpas_ui_advantages_promotions() for uitpas-ui-advantages_promotions.tpl.php.
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
        l($advantage->title, uitpas_ui_helpers_link_object('advantage', $advantage)),
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
  $header = array(array('data' => t('Promotions'), 'colspan' => 2), t('valid till'), '');
  $rows = array();
  if (count($promotions)) {
    foreach ($promotions as $promotion) {
      $location = array();
      foreach ($promotion->counters as $counter) {
        $location[] = $counter->name;
      }
      $singular = '%points point';
      $plural = '%points points';
      $points = format_plural($promotion->points, $singular, $plural, array('%points' => $promotion->points));
      $rows[] = array(
        array('data' => $points, 'class' => array('uitpas_ui_points')),
        l($promotion->title, uitpas_ui_helpers_link_object('promotion', $promotion)),
        ($promotion->cashingPeriodEnd) ? date('j/n/Y', $promotion->cashingPeriodEnd) : '-',
        implode(', ', $location)
      );
    }
  }
  else {
    $rows[] = array(array('data' => t('No results found'), 'colspan' => 4));
  }
  $table = $base_table + array('header' => $header, 'rows' => $rows);
  $pager = $base_pager + array('element' => 1, 'quantity' => $promotions_total);
  $vars['promotions'] = theme_table($table) . theme_pager($pager);
  //info
  $vars['info'] = variable_format_value('uitpas_ui_advantages_promotions_info');
}

/**
 * Implements hook_preprocess_uitpas_ui_promotion() for uitpas-ui-promotion.tpl.php.
 */
function uitpas_ui_preprocess_uitpas_ui_promotion(&$vars) {
  $promotion = $vars['promotion'];
  $singular = 'Needed points: !points point';
  $plural = 'Needed points: !points points';
  $vars['points'] = format_plural($promotion->points, $singular, $plural, array('!points' => $promotion->points));
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
  $location_options = variable_get_value('uitpas_ui_locations');
  $location_options = array_map('trim', explode("\n", $location_options));
	$valid_for_cities = array_intersect($location_options, $promotion->validForCities);
  asort($valid_for_cities);
  $list = array(
    'title' => t('Valid for inhabitants of'), 
    'attributes' => array(), 
    'type' => 'ul', 
    'items' => $valid_for_cities,
  );
  $vars['available'] = theme_item_list($list);
}

/**
 * Implements hook_preprocess_uitpas_ui_promotions_highlight() for uitpas-ui-promotions_highlight.tpl.php.
 */
function uitpas_ui_preprocess_uitpas_ui_promotions_highlight(&$vars) {
  $promotions = $vars['promotions'];
  $rows = array();
  foreach ($promotions as $promotion) {
    $singular = '%points point';
    $plural = '%points points';
    $rows[] = array(
      array(
        'data' =>format_plural($promotion->points, $singular, $plural, array('%points' => $promotion->points)),
        'class' => array('uitpas_ui_points'),
      ),
      l($promotion->title, uitpas_ui_helpers_link_object('promotion', $promotion)),
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
  $vars['more'] = l(t('Show all promotions'), 'advantages_promotions');
}

/**
 * Implements hook_preprocess_uitpas_ui_recent_actions() for uitpas-ui-recent-actions.tpl.php.
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
    $singular = "!name saved 1 point at !location, <span class=\"uitpas_ui_time_ago\">!time_ago ago</span>";
    $plural = "!name saved !points points at !location, <span class=\"uitpas_ui_time_ago\">!time_ago ago</span>";
    $default_image = variable_get_value('uitpas_ui_user_default_image');
    $image = ($action->depiction) ? $action->depiction : variable_get_value('uitpas_ui_user_default_image');
    $image = theme_image(array('path' => $image, 'attributes' => array()));
    $list['items'][] = $image . format_plural($action->points, $singular, $plural, $args);
  }
  
  $vars['actions'] = theme_item_list($list);
  $vars['form'] = (count($vars['form'])) ? drupal_render($vars['form']) : '';
}

/**
 * Implements hook_preprocess_uitpas_ui_register_where() for uitpas-ui-register-where.tpl.php.
 */
function uitpas_ui_preprocess_uitpas_ui_register_where(&$vars) {
  $table = array(
    'header' =>  array(),
    'rows' =>  array(),
    'attributes' => array(),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => '',
    'empty' => '',
  );
  foreach ($vars['pos'] as $pos) {
    $path = '';
    $details = "<div class=\"uitpas_pos_details\">";
    $times = '-';
    if (isset($vars['actors'][$pos->id])) {
      $actor = $vars['actors'][$pos->id];
      $address = $actor['contactinfo']['address'];
      $details .= "<div class=\"uitpas_pos_address\">";
      if (isset($address['street'])) {
        $details .= $address['street'] . ' ' . $address['housenr'] . "<br />";
      }
      $details .= $address['zipcode'] . ' ' . $address['city'];
      $details .= "</div>";
      if (isset($actor['detail']['nl']['calendarsummary'])) {
        $times = $actor['detail']['nl']['calendarsummary'];
      }
      $path = uitpas_ui_helpers_link_object('actor', $actor);
    }
    else if ($pos->street || $pos->city) {
      $details .= "<div class=\"uitpas_pos_address\">";
      if ($pos->street) {
        $details .= $pos->street . ' ' . $pos->number . "<br />";
      }
      $details .= $pos->postalCode . ' ' .  $pos->city;
      $details .= "</div>";
    }
    //description
    $title = ($path) ? l($pos->name, $path) : $pos->name;
    $description = "<div class=\"uitpas_pos_name\">" . $title . "</div>";
    $description .= $details;
    if ($path) {
      $more = t('More info and contact information');
      $link = l($more, $path);
      $description .= "<div class=\"uitpas_pos_more\">" . $link . "</div>";
    }
    $description .= "</div>";
    $table['rows'][] = array($description, $times);
  }
  $pager = array('tags' => array(), 'parameters' => array(), 'element' => 0, 'quantity' => $vars['total']);
  $vars['pos'] = theme_table($table) . theme_pager($pager);
  $vars['info'] = variable_format_value('uitpas_ui_register_where_info');
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
      'href' => uitpas_ui_helpers_link_object('advantage', $advantage),
    );
  }
  //promotions
  foreach ($promotions as $promotion) {
    $links['links'][] = array(
      'title' => $promotion->title,
      'href' => uitpas_ui_helpers_link_object('promotion', $promotion),
    );
  }
  $vars['advantages_promotions'] = theme_links($links);
  //coming promotions
  $links = array('links' => array(), 'heading' => array(), 'attributes' => array());
  foreach ($coming_promotions as $promotion) {
    $links['links'][] = array(
      'title' => $promotion->title . ' (' . t('!points points', array('!points' => $promotion->points)) . ')',
      'href' => uitpas_ui_helpers_link_object('promotion', $promotion),
    );
  }
  $vars['coming_promotions'] = (count($coming_promotions)) ? theme_links($links) : '';
  $vars['all_promotions'] = l(t('Show all advantages'), 'myprofile/advantages');
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
      $singular = '!points point';
      $plural = '!points points';
      $rows[] = array(
        date('j/m/Y', $activity->creationDate),
        $activity->nodeTitle,
        $activity->location,
        format_plural($activity->points, $singular, $plural, array('!points' => $activity->points)),
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
        l($promotion->title, uitpas_ui_helpers_link_object('promotion', $promotion)),
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
        l($advantage->title, uitpas_ui_helpers_link_object('advantage', $advantage)),
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
      l($promotion->title, uitpas_ui_helpers_link_object('promotion', $promotion)),
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
      l($advantage->title, uitpas_ui_helpers_link_object('advantage', $advantage)),
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
 * Implements hook_preprocess_uitpas_ui_user_profile_details() for uitpas-ui-user-profile-details.tpl.php.
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
  $vars['kansenStatuut'] = $passholder->kansenStatuut;
  $vars['kansenStatuutValidEndDate'] = (time() < $passholder->kansenStatuutEndDate);
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
 * Implements hook_preprocess_uitpas_ui_user_profile_navigation() for uitpas-ui-user-profile-navigation.tpl.php.
 */
function uitpas_ui_preprocess_uitpas_ui_user_profile_navigation(&$vars) {
  $navigation = $vars['navigation'];
  $output = '';
  foreach ($vars['navigation'] as $nav) {
    $title = variable_get_value('uitpas_ui_user_profile_navigation_' . $nav . '_title');
    $path = variable_get_value('uitpas_ui_user_profile_navigation_' . $nav . '_path');
    $body = variable_format_value('uitpas_ui_user_profile_navigation_' . $nav . '_body');
    $output .= "<h3>" . l($title, $path) . "</h3>";
    $output .= "<div class=\"uitpas_ui_user_profile_navigation_" . $nav . "\">" . $body . "</div>";
  }
  $vars['navigation'] = $output;
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
    //subject
    $url = "http://www.uitinvlaanderen.be/agenda/";
    if ($action->contentType == 'event') {
      $url .= 'e';
    }
    else if ($action->contentType == 'actor') {
      $url .= 'a';
    }
    $url .= "/" . cnapi_ui_slug($action->nodeTitle) . "/". $action->nodeId;
    $subject = l($url, $action->nodeTitle);
    //args
    $args = array(
      '!name' => ($action->nick) ? $action->nick : t('Anonymous'),
      '!points' => $action->points,
      '!location' => $action->createdVia,
      '!time_ago' => format_interval(time() - $action->creationDate, 1),
      '!subject' => $subject,
    );
    switch ($action->type) {
      case CultureFeed_Activity::TYPE_UITPAS:
        $singular = "!name saved 1 point at !location, <span class=\"uitpas_ui_time_ago\">!time_ago ago</span>";
        $plural = "!name saved !points points at !location, <span class=\"uitpas_ui_time_ago\">!time_ago ago</span>";
        $list['items'][] = format_plural($action->points, $singular, $plural, $args);
        break;
      case CultureFeed_Activity::TYPE_IK_GA:
        $string  = t("!name goes to !subject, <span class=\"uitpas_ui_time_ago\">!time_ago ago</span>", $args);
        $list['items'][] = $string;
        break;
      case CultureFeed_Activity::TYPE_LIKE:
        $string = t("!name likes !subject, <span class=\"uitpas_ui_time_ago\">!time_ago ago</span>", $args);
        $list['items'][] = $string;
        break;
    }
  }
  $vars['actions'] = (count($list['items'])) ? theme_item_list($list) : '';
  $vars['form'] = drupal_render($vars['form']);
  $vars['info'] = variable_format_value('uitpas_ui_user_profile_uitid_info');
}