<?php

/**
 * @file
 * Culturefeed uitpas preprocess functions.
 */

/**
 * Implements hook_preprocess_culturefeed_uitpas_advantage().
 */
function culturefeed_uitpas_preprocess_culturefeed_uitpas_advantage(&$vars) {

  $advantage = $vars['advantage'];

  // Image.
  $vars['image'] = '';
  if (isset($advantage->pictures[0])) {
    $vars['image'] = theme_image(array('path' => $advantage->pictures[0], 'attributes' => array()));
  }

  // Period.
  $vars['period'] = '';
  if ($advantage->cashingPeriodBegin) {

    $period = t('From !date', array('!date' => date('j/n/Y', $advantage->cashingPeriodBegin)));
    if ($advantage->cashingPeriodEnd) {
      $period .= ' ' . t('to !date', array('!date' => date('j/n/Y', $advantage->cashingPeriodEnd)));
    }
    $vars['period'] = $period;

  }

  // Location.
  $location = array();
  foreach ($advantage->counters as $counter) {
    $location[] = $counter->name;
  }
  $vars['location'] = t('At') . " " . implode(', ', $location);

  // Provider.
  $vars['provider'] = '';
  if (isset($advantage->owningCardSystem->name)) {
    $vars['provider'] = t('Provided by @provider', array('@provider' => culturefeed_uitpas_cardsystem_name($advantage->owningCardSystem)));
  }

  // Availability.
  $vars['available'] = '';
  if (count($advantage->validForCities)) {

    $valid_for_cities = $advantage->validForCities;
    asort($valid_for_cities);
    $list = array(
      'title' => t('Only available for'),
      'attributes' => array(),
      'type' => 'ul',
      'items' => $valid_for_cities,
    );
    $vars['available'] = theme_item_list($list);

  }

  // Description1.
  $vars['description1'] = '';
  if ($advantage->description1) {
    $vars['description1'] = $advantage->description1;
  }

  // Description2.
  $vars['description2'] = '';
  if ($advantage->description2) {
    $vars['description2'] = $advantage->description2;
  }

}

/**
 * Implements hook_preprocess_culturefeed_uitpas_advantages_promotions().
 */
function culturefeed_uitpas_preprocess_culturefeed_uitpas_advantages_promotions(&$vars) {

  $advantages = $vars['advantages'];
  $advantages_total = $vars['advantages_total'];
  $advantages_pager_element = $vars['advantages_pager_element'];

  $promotions = $vars['promotions'];
  $promotions_total = $vars['promotions_total'];
  $promotions_pager_element = $vars['promotions_pager_element'];

  $base_table = array(
    'attributes' => array(),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => '',
    'empty' => '',
  );
  $base_pager = array('tags' => array(), 'parameters' => array());

  // Advantages.
  $header = array(
    array('data' => t('Welcome advantages'), 'colspan' => 2),
    t('Valid till'),
    t('Description'),
    t('Counters'),
  );
  $rows = array();
  if (count($advantages)) {

    foreach ($advantages as $advantage) {

      $counters = array();
      foreach ($advantage->counters as $counter) {
        $counters[] = $counter->name;
      }

      $rows[] = array(
        l($advantage->title, 'advantage/' . culturefeed_search_slug($advantage->title) . '/' . $advantage->id),
        ($advantage->cashingPeriodEnd) ? date('j/n/Y', $advantage->cashingPeriodEnd) : t('end of stock'),
        ($advantage->description1) ? $advantage->description1 : '',
        implode(', ', $counters),
        (isset($advantage->pictures[0])) ? theme_image(array('path' => $advantage->pictures[0] . '?maxwidth=100&maxheight=100', 'attributes' => array())) : '',
      );

    }

  }
  else {
    $rows[] = array(array('data' => t('No results found'), 'colspan' => 2));
  }
  $table = $base_table + array('header' => $header, 'rows' => $rows);
  $pager = $base_pager + array('element' => $advantages_pager_element, 'quantity' => $advantages_total);
  $vars['advantages'] = theme_table($table) . theme_pager($pager);

  // Promotions.
  $header = array(
    array('data' => t('Promotions'), 'colspan' => 3),
    t('Valid till'),
    t('Description'),
    t('Counters'),
  );
  $rows = array();
  if (count($promotions)) {

    foreach ($promotions as $promotion) {

      $counters = array();
      foreach ($promotion->counters as $counter) {
        $counters[] = $counter->name;
      }

      $singular = '%points point';
      $plural = '%points points';
      $points = format_plural($promotion->points, $singular, $plural, array('%points' => $promotion->points));
      $rows[] = array(
        array('data' => $points, 'class' => array('uitpas_ui_points')),
        l($promotion->title, 'promotion/' . culturefeed_search_slug($promotion->title) . '/' . $promotion->id),
        ($promotion->cashingPeriodEnd) ? date('j/n/Y', $promotion->cashingPeriodEnd) : t('end of stock'),
        ($promotion->description1) ? $promotion->description1 : '',
        implode(', ', $counters),
        (isset($promotion->pictures[0])) ? theme_image(array('path' => $promotion->pictures[0] . '?maxwidth=100&maxheight=100', 'attributes' => array())) : '',
      );

    }

  }
  else {
    $rows[] = array(array('data' => t('No results found'), 'colspan' => 3));
  }
  $table = $base_table + array('header' => $header, 'rows' => $rows);
  $pager = $base_pager + array('element' => $promotions_pager_element, 'quantity' => $promotions_total);
  $vars['promotions'] = theme_table($table) . theme_pager($pager);

  // Info.
  $vars['info'] = variable_get('culturefeed_uitpas_advantages_promotions_info');

}

/**
 * Implements hook_preprocess_culturefeed_uitpas_profile_actions().
 */
function culturefeed_uitpas_preprocess_culturefeed_uitpas_profile_actions(&$vars) {

  $actions = $vars['actions'];
  $actions_total = $vars['actions_total'];
  $actions_pager_element = $vars['actions_pager_element'];

  $base_table = array(
    'attributes' => array(),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => '',
    'empty' => '',
  );

  $base_pager = array('tags' => array(), 'parameters' => array());

  // Intro.
  $vars['intro'] = variable_get('culturefeed_uitpas_profile_actions_intro');

  // Actions.
  $header = array(t('Date'), t('Activity name'), t('Location'), t('Points'));
  $rows = array();
  if (count($actions)) {

    foreach ($actions as $action) {

      $singular = '!points point';
      $plural = '!points points';
      $rows[] = array(
        date('j/m/Y', $action->creationDate),
        $action->nodeTitle,
        $action->location,
        format_plural($action->points, $singular, $plural, array('!points' => $action->points)),
      );

    }

  }
  else {
    $rows[] = array(array('data' => t('No results found.'), 'colspan' => 4));
  }

  $table = $base_table + array('header' => $header, 'rows' => $rows);
  $pager = $base_pager + array('element' => $actions_pager_element, 'quantity' => $actions_total);
  $vars['actions'] = theme_table($table) . theme_pager($pager);

}

/**
 * Implements hook_preprocess_culturefeed_uitpas_profile_advantages().
 */
function culturefeed_uitpas_preprocess_culturefeed_uitpas_profile_advantages(&$vars) {

  $advantages = $vars['advantages'];
  $advantages_total = $vars['advantages_total'];
  $advantages_pager_element = $vars['advantages_pager_element'];

  // @var CultureFeed_PointsPromotion[] $promotions
  $promotions = $vars['promotions'];
  $promotions_total = $vars['promotions_total'];
  $promotions_pager_element = $vars['promotions_pager_element'];

  $upcoming_promotions = $vars['upcoming_promotions'];
  $upcoming_promotions_total = $vars['upcoming_promotions_total'];
  $upcoming_promotions_pager_element = $vars['upcoming_promotions_pager_element'];

  $base_table = array(
    'attributes' => array(),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => '',
    'empty' => '',
  );

  $base_pager = array('tags' => array(), 'parameters' => array());

  // Promotions.
  $header = array(
    '',
    array('data' => t('Trade-in options'), 'colspan' => 2),
    array('data' => t('Valid till')),
    array('data' => t('Description')),
  );
  $rows = array();
  if (count($promotions)) {

    foreach ($promotions as $promotion) {

      $singular = t('1 point');
      $plural = t('!points points', array('!points' => $promotion->points));
      $points = format_plural($promotion->points, $singular, $plural);
      $promotion_path = 'promotion/' . culturefeed_search_slug($promotion->title) . '/' . $promotion->id;
      $picture = '';
      if (isset($promotion->pictures[0])) {

        $picture = theme('culturefeed_uitpas_promotion_image', array('path' => $promotion->pictures[0], 'attributes' => array()));
        $picture = l($picture, $promotion_path, array('html' => TRUE));

      }

      $rows[] = array(
        $picture,
        array('data' => $points, 'class' => array('uitpas_ui_points')),
        l($promotion->title, $promotion_path),
        ($promotion->cashingPeriodEnd) ? date('j/n/Y', $promotion->cashingPeriodEnd) : t('end of stock'),
        ($promotion->description1) ? $promotion->description1 : '',
      );

    }

  }
  else {
    $rows[] = array(array('data' => t('No results found.'), 'colspan' => 5));
  }
  $table = $base_table + array('header' => $header, 'rows' => $rows);
  $pager = $base_pager + array('element' => $promotions_pager_element, 'quantity' => $promotions_total);
  $vars['promotions'] = theme_table($table) . theme_pager($pager);

  // Upcoming promotions.
  $header = array(
    '',
    array('data' => t('Coming trade-in options'), 'colspan' => 2),
    array('data' => t('Valid till')),
    array('data' => t('Description')),
  );
  $rows = array();
  if (count($upcoming_promotions)) {

    foreach ($upcoming_promotions as $promotion) {

      $singular = t('1 point');
      $plural = t('!points points', array('!points' => $promotion->points));
      $points = format_plural($promotion->points, $singular, $plural);
      $promotion_path = 'promotion/' . culturefeed_search_slug($promotion->title) . '/' . $promotion->id;
      $picture = '';
      if (isset($promotion->pictures[0])) {

        $picture = theme('culturefeed_uitpas_promotion_image', array('path' => $promotion->pictures[0], 'attributes' => array()));
        $picture = l($picture, $promotion_path, array('html' => TRUE));

      }

      $rows[] = array(
        $picture,
        array('data' => $points, 'class' => array('uitpas_ui_points')),
        l($promotion->title, $promotion_path),
        ($promotion->cashingPeriodEnd) ? date('j/n/Y', $promotion->cashingPeriodEnd) : t('end of stock'),
        ($promotion->description1) ? $promotion->description1 : '',
      );

    }

  }
  else {
    $rows[] = array(array('data' => t('No results found.'), 'colspan' => 5));
  }

  $table = $base_table + array('header' => $header, 'rows' => $rows);
  $pager = $base_pager + array('element' => $upcoming_promotions_pager_element, 'quantity' => $upcoming_promotions_total);
  $vars['upcoming_promotions'] = theme_table($table) . theme_pager($pager);

  // Advantages.
  $header = array(
    '',
    array('data' => t('Advantages')),
    array('data' => t('Valid till')),
    array('data' => t('Description')),
  );
  $rows = array();
  if (count($advantages)) {

    foreach ($advantages as $advantage) {

      $advantage_path = 'advantage/' . culturefeed_search_slug($advantage->title) . '/' . $advantage->id;
      $picture = '';
      if (isset($advantage->pictures[0])) {

        $picture = theme('culturefeed_uitpas_promotion_image', array('path' => $advantage->pictures[0], 'attributes' => array()));

        $picture = l($picture, $advantage_path, array('html' => TRUE));
      }

      $rows[] = array(
        $picture,
        l($advantage->title, $advantage_path),
        ($advantage->cashingPeriodEnd) ? date('j/n/Y', $advantage->cashingPeriodEnd) : t('end of stock'),
        ($advantage->description1) ? $advantage->description1 : '',
      );

    }

  }
  else {
    $rows[] = array(array('data' => t('No results found.'), 'colspan' => 4));
  }

  $table = $base_table + array('header' => $header, 'rows' => $rows);
  $pager = $base_pager + array('element' => $advantages_pager_element, 'quantity' => $advantages_total);
  $vars['advantages'] = theme_table($table) . theme_pager($pager);

}

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
  $vars['gender'] = '<label>' . t('Gender') . ':</label> ' . ($passholder->gender == 'MALE') ? t('Male') : t('Female');
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
 * Implements hook_preprocess_culturefeed_uitpas_profile_summary().
 */
function culturefeed_uitpas_preprocess_culturefeed_uitpas_profile_summary(&$vars) {

  $uitpas_user = $vars['uitpas_user'];
  $passholder = $vars['passholder'];
  $advantages = $vars['advantages'];
  $promotions = $vars['promotions'];
  $upcoming_promotions = $vars['upcoming_promotions'];

  if (isset($uitpas_user->depiction)) {
    $vars['image'] = theme_image(array('path' => $uitpas_user->depiction, 'attributes' => array()));
  }
  else {
    $vars['image'] = '';
  }

  $vars['name'] = l($uitpas_user->nick, 'user');
  $singular = t('1 point saved');
  $plural = t('%points points saved', array('%points' => $passholder->points));
  $vars['points'] = format_plural($passholder->points, $singular, $plural);

  // Advantages.
  $links = array(
    'links' => array(),
    'heading' => array(),
    'attributes' => array(),
  );
  foreach ($advantages as $advantage) {

    $links['links'][] = array(
      'title' => $advantage->title,
      'href' => 'advantage/' . culturefeed_search_slug($advantage->title) . '/' . $advantage->id,
    );

  }
  $vars['advantages'] = theme_links($links);

  // Promotions.
  $vars['promotions_title'] = variable_get('culturefeed_uitpas_profile_summary_promotions_title', t('Your UiTPAS advantages'));
  $links = array(
    'links' => array(),
    'heading' => array(),
    'attributes' => array(),
  );
  foreach ($promotions as $promotion) {

    $links['links'][] = array(
      'title' => $promotion->title . ' (' . t('!points points', array('!points' => $promotion->points)) . ')',
      'href' => 'promotion/' . culturefeed_search_slug($promotion->title) . '/' . $promotion->id,
    );

  }
  $vars['promotions'] = theme_links($links);

  // Upcoming promotions.
  $vars['upcoming_promotions_title'] = variable_get('culturefeed_uitpas_profile_summary_upcoming_promotions_title', t('Just a few points more for'));
  $links = array(
    'links' => array(),
    'heading' => array(),
    'attributes' => array(),
  );
  foreach ($upcoming_promotions as $promotion) {

    $links['links'][] = array(
      'title' => $promotion->title . ' (' . t('!points points', array('!points' => $promotion->points)) . ')',
      'href' => 'promotion/' . culturefeed_search_slug($promotion->title) . '/' . $promotion->id,
    );

  }
  $vars['upcoming_promotions'] = (count($upcoming_promotions)) ? theme_links($links) : '';

  // All promotions.
  $vars['all_promotions'] = l(variable_get('culturefeed_uitpas_profile_summary_more_link_text', t('Show all my promotions')), 'culturefeed/profile/uitpas/advantages');

}

/**
 * Implements hook_preprocess_culturefeed_uitpas_promotion().
 */
function culturefeed_uitpas_preprocess_culturefeed_uitpas_promotion(&$vars) {

  $promotion = $vars['promotion'];

  // Image.
  $vars['image'] = '';
  if (isset($promotion->pictures[0])) {
    $vars['image'] = theme_image(array('path' => $promotion->pictures[0] . '?maxwidth=300&max-height=300', 'attributes' => array()));
  }

  // Points.
  $singular = 'Needed points: !points point';
  $plural = 'Needed points: !points points';
  $vars['points'] = format_plural($promotion->points, $singular, $plural, array('!points' => $promotion->points));

  // Period.
  $vars['period'] = '';
  if ($promotion->cashingPeriodBegin) {

    $period = t('From !date', array('!date' => date('j/m/Y', $promotion->cashingPeriodBegin)));
    if ($promotion->cashingPeriodEnd) {
      $period .= ' ' . t('to !date', array('!date' => date('j/m/Y', $promotion->cashingPeriodEnd)));
    }
    $vars['period'] = $period;

  }

  // Location.
  $location = array();
  foreach ($promotion->counters as $counter) {
    $location[] = $counter->name;
  }
  $vars['location'] = t('At') . " " . implode(', ', $location);

  // Provider.
  $vars['provider'] = '';
  if (isset($promotion->owningCardSystem->name)) {
    $vars['provider'] = t('Provided by @provider', array('@provider' => culturefeed_uitpas_cardsystem_name($promotion->owningCardSystem)));
  }

  // Available.
  $vars['available'] = '';
  if (count($promotion->validForCities)) {

    $valid_for_cities = $promotion->validForCities;
    asort($valid_for_cities);
    $list = array(
      'title' => t('Only available for'),
      'attributes' => array(),
      'type' => 'ul',
      'items' => $valid_for_cities,
    );
    $vars['available'] = theme_item_list($list);

  }

  // Description1.
  $vars['description1'] = '';
  if ($promotion->description1) {
    $vars['description1'] = $promotion->description1;
  }

  // Description2.
  $vars['description2'] = '';
  if ($promotion->description2) {
    $vars['description2'] = $promotion->description2;
  }

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
      l($promotion->title, 'promotion/' . culturefeed_search_slug($promotion->title) . '/' . $promotion->id),

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
    $url = 'agenda/e/' . culturefeed_search_slug($action->nodeTitle) . '/' . $action->nodeId;
    $subject = l($action->nodeTitle, $url);

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
    $image = ($action->depiction) ? $action->depiction . '?maxwidth=50&maxheight=50&crop=auto' : variable_get('culturefeed_uitpas_user_default_image');
    if ($image) {
      $image = theme_image(array('path' => $image, 'attributes' => array('class' => 'thumbnail')));
    }

    $list['items'][] = $image . format_plural($action->points, $singular, $plural, $args);

  }

  $vars['actions'] = theme_item_list($list);

}

/**
 * Theme function.
 */
function theme_culturefeed_uitpas_promotion_image($vars) {

  $vars['path'] .= '?maxheight=' . $vars['maxheight'] . '&maxwidth=' . $vars['maxwidth'];
  return theme('image', $vars);

}
