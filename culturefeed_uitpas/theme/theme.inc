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

  // Advantages.
  $vars['advantages_table'] = theme('culturefeed_uitpas_advantages', $vars);

  // Promotions.
  $vars['promotions_table'] = theme('culturefeed_uitpas_promotions', $vars);

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

  $table = array(
    'attributes' => array(),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => '',
    'empty' => '',
  );

  // Intro.
  $vars['intro'] = variable_get('culturefeed_uitpas_profile_actions_intro');

  // Actions.
  $header = array(t('Date'), t('Activity name'), t('Location'), t('Points'));
  $rows = array();
  if (count($actions)) {

    foreach ($actions as $action) {

      $singular = '@count point';
      $plural = '@count points';
      $rows[] = array(
        date('j/m/Y', $action->creationDate),
        $action->nodeTitle,
        $action->location,
        format_plural($action->points, $singular, $plural),
      );

    }

  }
  else {
    $rows[] = array(array('data' => t('No results found.'), 'colspan' => 4));
  }

  $table += array('header' => $header, 'rows' => $rows);
  $pager = array(
    'element' => $actions_pager_element,
    'quantity' => $actions_total,
  );

  $vars['actions_table'] = theme_table($table) . theme('pager', $pager);

  // Publish actions form.
  $vars['activity_preferences_form'] = '';
  $uitpas_user = culturefeed_uitpas_get_uitpas_user();
  if ($uitpas_user->user) {
    $form = drupal_get_form('culturefeed_uitpas_activity_preferences_form');
    $vars['activity_preferences_form'] = drupal_render($form);
  }

}

/**
 * Implements hook_preprocess_culturefeed_uitpas_profile_advantages().
 */
function culturefeed_uitpas_preprocess_culturefeed_uitpas_profile_advantages(&$vars) {

  // Promotions.
  $vars['promotions_table'] = theme('culturefeed_uitpas_promotions', $vars);

  // Upcoming promotions.
  $vars['upcoming_promotions_table'] = theme('culturefeed_uitpas_promotions', array(
    'title' => t('Upcoming promotions'),
    'promotions' => $vars['upcoming_promotions'],
    'promotions_total' => $vars['upcoming_promotions_total'],
    'promotions_pager_element' => $vars['upcoming_promotions_pager_element'],
  ));

  // Advantages.
  $vars['advantages_table'] = theme('culturefeed_uitpas_advantages', $vars);

}

/**
 * Implements hook_preprocess_preprocess_culturefeed_uitpas_profile_details().
 */
function culturefeed_uitpas_preprocess_culturefeed_uitpas_profile_details(&$vars) {

  $uitpas_user = $vars['uitpas_user'];
  // @codingStandardsIgnoreStart
  /** @var CultureFeed_Uitpas_Passholder $passholder */
  // @codingStandardsIgnoreEnd
  $passholder = $uitpas_user->passholder;
  // @codingStandardsIgnoreStart
  /** @var CultureFeed_Uitpas_Passholder_CardSystemSpecific $card_system */
  // @codingStandardsIgnoreEnd
  $card_system = $uitpas_user->card_system;

  // Details.
  $vars['uitpas_title'] = variable_get('culturefeed_uitpas_profile_details_title', t('My UiTPAS'));
  $uitpas_numbers = array(
    'items' => array(),
    'type' => 'ul',
    'attributes' => array(),
    'title' => '',
  );
  foreach ($passholder->cardSystemSpecific as $card_system_specific) {

    if ($card_system_specific->currentCard->uitpasNumber) {
      $uitpas_numbers['items'][] = $card_system_specific->currentCard->uitpasNumber . ' (' . $card_system_specific->cardSystem->name . ')';
    }

  }
  $vars['uitpas_numbers'] = '<label>' . variable_get('culturefeed_uitpas_profile_details_uitpas_number', t('UiTPAS number(s)')) . ':</label> ' . theme_item_list($uitpas_numbers);
  $vars['intro'] = variable_get('culturefeed_uitpas_profile_details_intro');
  $vars['first_name'] = '<label>' . t('First name') . ':</label> ' . $passholder->firstName;
  $vars['last_name'] = '<label>' . t('Name') . ':</label> ' . $passholder->name;
  $vars['dob'] = '<label>' . t('Date of birth') . ':</label> ' . date('j/m/Y', $passholder->dateOfBirth);
  $vars['pob'] = '<label>' . t('Place of birth') . ':</label> ' . $passholder->placeOfBirth;
  $gender = ($passholder->gender == 'MALE') ? t('Male') : t('Female');
  $vars['gender'] = '<label>' . t('Gender') . ':</label> ' . $gender;
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
  $vars['kansen_statuut'] = '';
  $vars['kansen_statuut_valid_end_date'] = '';
  $vars['status_valid_till'] = '';

  if ($card_system) {

    $vars['kansen_statuut'] = $card_system->kansenStatuut;
    $vars['kansen_statuut_valid_end_date'] = (time() < $card_system->kansenStatuutEndDate);
    $status_end_date = t('valid till !date', array('!date' => date('j/m/Y', $card_system->kansenStatuutEndDate)));
    $vars['status_valid_till'] = '<label>' . t('Opportunity status') . ':</label> ' . $status_end_date;

  }

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
    $vars['image'] = theme_image(array('path' => $uitpas_user->depiction . '?maxwidth=50&amp;maxheight=50&amp;crop=auto', 'attributes' => array()));
  }
  else {
    $vars['image'] = '';
  }

  $vars['name'] = l($uitpas_user->nick, 'user');
  $singular = '@count ' . t('point');
  $plural = '@count ' . t('points');
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
  $vars['advantages_links'] = theme_links($links);

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
  $vars['promotions_links'] = theme_links($links);

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
  $vars['upcoming_promotions_links'] = (count($upcoming_promotions)) ? theme_links($links) : '';

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
  $singular = t('Needed points') . ': @count ' . t('point');
  $plural = t('Needed points') . ': @count ' . t('points');
  $vars['points'] = format_plural($promotion->points, $singular, $plural);

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
    $vars['available'] = t('Only available for') . " " . implode(', ', $valid_for_cities);

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

    $singular = '@count point';
    $plural = '@count points';
    $rows[] = array(
      array(
        'data' => format_plural($promotion->points, $singular, $plural),
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

  $vars['promotions_table'] = theme_table($table);
  $more_text = variable_get('culturefeed_uitpas_promotions_highlight_more_link_text', t('Show all promotions'));
  $vars['more'] = l($more_text, 'advantages_promotions');

}

/**
 * Implements hook_preprocess_culturefeed_uitpas_register_where().
 */
function culturefeed_uitpas_preprocess_culturefeed_uitpas_register_where(&$vars) {

  $table = array(
    'header' => array(),
    'rows' => array(),
    'attributes' => array(),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => '',
    'empty' => '',
  );

  if (count($vars['pos'])) {

    foreach ($vars['pos'] as $pos) {

      $address = array();

      if (isset($vars['actors'][$pos->id])) {

        // @codingStandardsIgnoreStart
        /** @var CultureFeed_Cdb_Item_Actor $actor */
        // @codingStandardsIgnoreEnd
        $actor = $vars['actors'][$pos->id]->getEntity();
        $contact_info = $actor->getContactInfo();
        // @codingStandardsIgnoreStart
        /** @var CultureFeed_Cdb_Data_Address[] $addresses */
        // @codingStandardsIgnoreEnd
        $addresses = $contact_info->getAddresses();
        if ($addresses[0]) {

          if ($addresses[0]->getPhysicalAddress()->getZip()) {
            $address[] = $addresses[0]->getPhysicalAddress()->getZip();
          }
          if ($addresses[0]->getPhysicalAddress()->getCity()) {
            $address[] = $addresses[0]->getPhysicalAddress()->getCity();
          }
        }

      }

      elseif ($pos->city && !count($address)) {

        if ($pos->postalCode) {
          $address[] = $pos->postalCode;
        }
        if ($pos->city) {
          $address[] = $pos->city;
        }

      }

      $table['rows'][] = array(
        l($pos->name, 'agenda/a/' . culturefeed_search_slug($pos->name) . '/' . $pos->id),
        implode(' ', $address),
      );

    }

  }
  else {
    $table['rows'][] = array(array('data' => t('No results found.'), 'colspan' => 2));
  }

  $pager = array(
    'element' => $vars['pos_pager_element'],
    'quantity' => $vars['pos_total'],
  );

  $vars['intro'] = variable_get('culturefeed_uitpas_register_where_intro');
  $vars['pos_table'] = theme_table($table) . theme('pager', $pager);
  $vars['outro'] = variable_get('culturefeed_uitpas_register_where_outro');

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
      '!location' => $action->createdVia,
      '!time_ago' => format_interval(time() - $action->creationDate, 1),
      '!subject' => $subject,
    );

    $singular = "<strong>!name</strong> " .  t('saved') . " @count " . t('point') . " " . t('at') . " !subject, <span class=\"time-ago\"> !time_ago " .  t('ago') . "</span>";
    $plural = "<strong>!name</strong> " .  t('saved') . " @count " . t('points') . " " . t('at') . " !subject, <span class=\"time-ago\"> !time_ago " .  t('ago') . "</span>";
    $image = ($action->depiction) ? $action->depiction . '?maxwidth=50&maxheight=50&crop=auto' : variable_get('culturefeed_uitpas_user_default_image');
    if ($image) {
      $image = theme_image(array('path' => $image, 'attributes' => array('class' => 'thumbnail')));
    }

    $list['items'][] = $image . format_plural($action->points, $singular, $plural, $args);

  }

  // Publish actions form.
  $vars['activity_preferences_form'] = '';
  $uitpas_user = culturefeed_uitpas_get_uitpas_user();
  if ($uitpas_user->user) {
    $form = drupal_get_form('culturefeed_uitpas_activity_preferences_form');
    $vars['activity_preferences_form'] = drupal_render($form);
  }

  $vars['actions_list'] = theme_item_list($list);

}

/**
 * Returns HTML for UiTPAS advantages.
 *
 * @param array $vars
 *   An associative array.
 *
 * @return string
 *   The HTML.
 */
function theme_culturefeed_uitpas_advantages(&$vars) {

  // @codingStandardsIgnoreStart
  /** @var CultureFeed_Uitpas_Passholder_WelcomeAdvantage[] $advantages */
  // @codingStandardsIgnoreEnd
  $advantages = $vars['advantages'];
  $advantages_total = $vars['advantages_total'];
  $advantages_pager_element = $vars['advantages_pager_element'];

  $table = array(
    'attributes' => array(),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => '',
    'empty' => '',
  );

  $header = array(
    t('Welcome advantages'),
    t('Valid till'),
    t('Description'),
    t('Counters'),
    '',
  );
  $rows = array();

  if (count($advantages)) {

    foreach ($advantages as $advantage) {

      $advantage_path = 'advantage/' . culturefeed_search_slug($advantage->title) . '/' . $advantage->id;

      // Counters.
      $counters = array();
      foreach ($advantage->counters as $counter) {
        $counters[] = $counter->name;
      }

      // Picture.
      $picture = '';
      if (isset($advantage->pictures[0])) {

        $picture = theme('culturefeed_uitpas_promotion_image', array(
          'path' => $advantage->pictures[0],
          'maxwidth' => 100,
          'maxheight' => 100,
          'attributes' => array(),
        ));
        $picture = l($picture, $advantage_path, array('html' => TRUE));

      }

      // Rows.
      $rows[] = array(
        l($advantage->title, $advantage_path),
        ($advantage->cashingPeriodEnd) ? date('j/n/Y', $advantage->cashingPeriodEnd) : t('end of stock'),
        ($advantage->description1) ? $advantage->description1 : '',
        implode(', ', $counters),
        $picture,
      );

    }

  }
  else {
    $rows[] = array(array('data' => t('No results found'), 'colspan' => 5));
  }

  $table += array('header' => $header, 'rows' => $rows);
  $pager = array(
    'element' => $advantages_pager_element,
    'quantity' => $advantages_total,
  );

  return theme_table($table) . theme('pager', $pager);

}

/**
 * Returns HTML for an UiTPAS promotion image.
 *
 * @param array $vars
 *   An associative array.
 *
 * @return string
 *   The HTML.
 */
function theme_culturefeed_uitpas_promotion_image($vars) {

  $vars['path'] .= '?maxheight=' . $vars['maxheight'] . '&maxwidth=' . $vars['maxwidth'];
  return theme('image', $vars);

}

/**
 * Returns HTML for UiTPAS promotions.
 *
 * @param array $vars
 *   An associative array.
 *
 * @return string
 *   The HTML.
 */
function theme_culturefeed_uitpas_promotions(&$vars) {

  // @codingStandardsIgnoreStart
  /** @var CultureFeed_Uitpas_Passholder_PointsPromotion[] $promotions */
  // @codingStandardsIgnoreEnd
  $promotions = $vars['promotions'];
  $promotions_total = $vars['promotions_total'];
  $promotions_pager_element = $vars['promotions_pager_element'];

  $table = array(
    'attributes' => array(),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => '',
    'empty' => '',
  );

  $header = array(
    array('data' => $vars['title'], 'colspan' => 2),
    t('Valid till'),
    t('Description'),
    t('Counters'),
    '',
  );
  $rows = array();

  if (count($promotions)) {

    foreach ($promotions as $promotion) {

      $promotion_path = 'promotion/' . culturefeed_search_slug($promotion->title) . '/' . $promotion->id;

      // Points.
      $singular = '@count point';
      $plural = '@count points';
      $points = format_plural($promotion->points, $singular, $plural);

      // Counters.
      $counters = array();
      foreach ($promotion->counters as $counter) {
        $counters[] = $counter->name;
      }

      // Picture.
      $picture = '';
      if (isset($promotion->pictures[0])) {

        $picture = theme('culturefeed_uitpas_promotion_image', array(
          'path' => $promotion->pictures[0],
          'maxwidth' => 100,
          'maxheight' => 100,
          'attributes' => array(),
        ));
        $picture = l($picture, $promotion_path, array('html' => TRUE));

      }

      if ($promotion->cashInState == $promotion::CASHIN_NOT_POSSIBLE_VOLUME_CONSTRAINT) {
        $validness = t('not any longer in stock');
      }
      else if ($promotion->cashingPeriodEnd) {
        $validness = date('j/n/Y', $promotion->cashingPeriodEnd);
      }
      else {
        $validness = t('end of stock');
      }

      // Rows.
      $rows[] = array(
        'class' => array(drupal_clean_css_identifier('cashin-' . drupal_strtolower($promotion->cashInState))),
        'data' => array(
          array('data' => $points, 'class' => array('points')),
          l($promotion->title, $promotion_path),
          $validness,
          ($promotion->description1) ? $promotion->description1 : '',
          implode(', ', $counters),
          $picture,
        ),
      );

    }

  }
  else {
    $rows[] = array(array('data' => t('No results found'), 'colspan' => 6));
  }

  $table += array('header' => $header, 'rows' => $rows);
  $pager = array(
    'element' => $promotions_pager_element,
    'quantity' => $promotions_total,
  );

  return theme_table($table) . theme('pager', $pager);

}
