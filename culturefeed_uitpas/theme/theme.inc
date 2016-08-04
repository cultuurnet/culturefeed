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

  // Ensure images are protocol-relative
  if (isset($advantage->pictures)) {
    foreach ($advantage->pictures as &$picture) {
      $picture = str_replace('http://', '//', $picture);
    }
  }

  // Image.
  $vars['image'] = '';
  $vars['image2'] = '';
  $vars['images_list'] = '';
  $images = array();
  if (isset($advantage->pictures[0])) {
    $vars['image'] = theme_image(array('path' => $advantage->pictures[0], 'alt' => $advantage->title, 'title' => $advantage->title, 'attributes' => array()));
    foreach ($advantage->pictures as $key => $picture) {
      $images[] = l(
        theme('image', array('path' => $advantage->pictures[$key] . '?maxwidth=300&max-height=300', 'alt' => $advantage->title, 'title' => $advantage->title, 'attributes' => array())),
        $advantage->pictures[$key],
        array('html' => TRUE)
      );
    }
    $vars['images_list'] = theme('item_list', array('items' => $images));
  }
  if (isset($advantage->pictures[1])) {
    $vars['image2'] = theme_image(array('path' => $advantage->pictures[1], 'alt' => $advantage->title, 'title' => $advantage->title, 'attributes' => array()));
  }

  // Get the counters information.
  $counters = array(
    '#theme' => 'item_list',
    '#type' => 'ul',
    '#items' => array(),
  );
  foreach ($advantage->counters as $counter) {
    $counters['#items'][] = l($counter->name, 'agenda/a/' . culturefeed_search_slug($counter->name) . '/' . $counter->id);
  }
  $vars['counters'] = count($counters['#items']) ? drupal_render($counters) : '';

  // Period.
  $period = '';
  $now = time();
  if ($advantage->cashingPeriodBegin && $advantage->cashingPeriodBegin > $now) {
    $period = t('From !date', array('!date' => date('j/m/Y', $advantage->cashingPeriodBegin))) . ' ';
    if ($advantage->cashingPeriodEnd) {
      $period .= t('until !date', array('!date' => date('j/m/Y', $advantage->cashingPeriodEnd)));
    }
  }
  else if ($advantage->cashingPeriodEnd) {
    $period .= t('Until !date', array('!date' => date('j/m/Y', $advantage->cashingPeriodEnd)));
  }
  else {
    $period = t('Until end of stock');
  }
  $vars['period'] = $period;

  // Provider.
  $vars['provider'] = '';
  if (isset($advantage->owningCardSystem->name)) {
    $vars['provider'] = t('Provided by @provider', array('@provider' => culturefeed_uitpas_cardsystem_name($advantage->owningCardSystem)));
    $vars['provider_raw'] = culturefeed_uitpas_cardsystem_name($advantage->owningCardSystem);
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

  drupal_add_css(drupal_get_path('module', 'culturefeed_uitpas') . '/css/promotion_advantage.css');
  drupal_add_js(drupal_get_path('module', 'culturefeed_uitpas') . '/js/script.js');
}

/**
 * Implements hook_preprocess_culturefeed_uitpas_advantages().
 */
function culturefeed_uitpas_preprocess_culturefeed_uitpas_advantages(&$vars) {

  // Advantages.
  $vars['advantages_table'] = theme('culturefeed_uitpas_advantages_table', $vars);

  // Info.
  $vars['info'] = t('This page shows all advantages for which UiTPAS holders can save points.');
}

/**
 * Implements hook_preprocess_culturefeed_uitpas_profile_advantages().
 */
function culturefeed_uitpas_preprocess_culturefeed_uitpas_profile_advantages(&$vars) {

  // Advantages.
  $total = $vars['advantages_total'];
  $vars['page_elements'] = t('Found %count advantages', array('%count' => $total));
  $vars['profile_advantages_table'] = theme('culturefeed_uitpas_advantages_table', $vars);

  // Info.
  $vars['info'] = t('This page only shows the advantages that you didn\'t receive yet.') . '<br>';
  $vars['info'] .= t('Discover <a href="@all-advantages">all advantages</a> that are currently available for new UiTPAS holders.', array('@all-advantages' => url('advantages')));

}

/**
 * Implements hook_preprocess_culturefeed_uitpas_promotions().
 */
function culturefeed_uitpas_preprocess_culturefeed_uitpas_promotions(&$vars) {

  // Page elements.
  $vars['page_elements'] = t('@min - @max of @total promotions',
    array(
      '@min' => $vars['promotions_pager_min'],
      '@max' => $vars['promotions_pager_max'],
      '@total' => $vars['promotions_total']
    )
  );

  // Promotions.
  $vars['promotions_table'] = theme('culturefeed_uitpas_promotions_table', $vars);

  // Info.
  $vars['info'] = t('This page shows all promotions for which UiTPAS holders can save points.');
}

/**
 * Implements hook_preprocess_culturefeed_uitpas_profile_promotions().
 */
function culturefeed_uitpas_preprocess_culturefeed_uitpas_profile_promotions(&$vars) {

  // Page elements.
  $min = $vars['promotions_pager_min'];
  $max = $vars['promotions_pager_max'];
  $total = $vars['promotions_total'];
  $vars['page_elements'] = t('Found %count promotions', array('%count' => $total));

  // Promotions.
  $vars['profile_promotions_table'] = theme('culturefeed_uitpas_promotions_table', $vars);

  // Info.
  $vars['info'] = t('This page only shows the promotions you can get with your current points balance') . '<br>';
  $vars['info'] .= t('Discover <a href="@all-promotions">all promotions</a> you can save for.', array('@all-promotions' => url('promotions')));

  // User points.
  $uitpas_user = culturefeed_uitpas_get_uitpas_user();
  $vars['passholder'] = $uitpas_user->passholder;
}

/**
 * Implements hook_preprocess_culturefeed_uitpas_profile_actions().
 */
function culturefeed_uitpas_preprocess_culturefeed_uitpas_profile_actions(&$vars) {

  $activities = $vars['activities'];

  $table = array(
    'attributes' => array(),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => '',
    'empty' => '',
  );

  // Intro.
  $vars['intro'] = t('This page shows an overview of all your UiTPAS actions');

  // Actions.
  $header = array(t('Date'), t('Action'), t('Points'));
  $rows = array();
  if (count($activities)) {

    foreach ($activities as $activity) {

      /* @var \CultureFeed_Activity $activity */
      $action = culturefeed_get_human_readable_activity_type($activity->type);
      $action .= '<br /><strong>' . $activity->nodeTitle . '</strong>';
      if ($activity->createdVia) {
        $action .= ' ' . t('at @location', array('@location' => $activity->createdVia));
      }

      // Points.
      $singular = t('@formatted_count point');
      $plural = t('@formatted_count points');
      $formatted_count = number_format($activity->points, 2, ',', '.');
      if ($activity->points > 0) {
        $formatted_count = '+' . $formatted_count;
      }
      $points = '<span>' . format_plural($activity->points, $singular, $plural, array('@formatted_count' => $formatted_count)) . '</span>';

      $rows[] = array(
        date('j/m/Y', $activity->creationDate) . '<br />' . date('H:i', $activity->creationDate),
        $action,
        $points,
      );

    }

  }
  else {
    $rows[] = array(array('data' => t('No results found.'), 'colspan' => 4));
  }

  $table += array('header' => $header, 'rows' => $rows);
  $vars['actions_table'] = theme_table($table);

  // Publish actions form.
  $vars['activity_preferences_form'] = '';
  $uitpas_user = culturefeed_uitpas_get_uitpas_user();
  if ($uitpas_user->user) {
    $form = drupal_get_form('culturefeed_uitpas_activity_preferences_form');
    $vars['activity_preferences_form'] = drupal_render($form);
  }

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

  // Intro.
  $vars['intro'] = t('This page shows the UiTPAS card(s) you own and the personal information that has been registered when buying your card.');

  // Card numbers.
  $uitpas_numbers = array(
    'items' => array(),
    'type' => 'ul',
    'attributes' => array(),
    'title' => '',
  );
  foreach ($passholder->cardSystemSpecific as $card_system_specific) {

    if (isset($card_system_specific->currentCard->uitpasNumber)) {

      $output = $card_system_specific->currentCard->uitpasNumber . ' (' . $card_system_specific->cardSystem->name . ')';
      if ($card_system_specific->kansenStatuut && time() < $card_system_specific->kansenStatuutEndDate) {
        $status_end_date = t('valid till !date', array('!date' => date('j/m/Y', $card_system_specific->kansenStatuutEndDate)));
        $output .= '<br /><label>' . t('Opportunity status') . ':</label> ' . $status_end_date;
      }
      $uitpas_numbers['items'][] = $output;

    }

  }
  $uitpas_numbers_title = t('UiTPAS number(s)');
  $uitpas_numbers_output = '<h3>' . $uitpas_numbers_title . ':</h3> ';
  $uitpas_numbers_output .= theme('item_list', $uitpas_numbers);
  $vars['uitpas_numbers_title'] = $uitpas_numbers_title;
  $vars['uitpas_numbers'] = $uitpas_numbers_output;

  $vars['form_title'] = t('My personal data');
  $vars['form_intro'] = t('This information has been read from your eID and cannot be changed. Updates can only be done at official <a href="@counters-url">UiTPAS registration counters</a>.', array('@counters-url' => url('register_where')));
  $form = drupal_get_form('culturefeed_uitpas_profile_details_form');
  $vars['form'] = drupal_render($form);

  $vars['status_title'] = t('Status');
  $vars['kansen_statuut'] = '';
  $vars['kansen_statuut_valid_end_date'] = '';
  $vars['status_valid_till'] = '';

  if (count($passholder->memberships)) {

    $memberships = array();
    foreach ($passholder->memberships as $membership) {

      if (isset($membership->association->association)) {

        $endate = t('valid till !date', array('!date' => date('j/m/Y', $membership->endDate)));
        $memberships[] = '<label>' . $membership->association . ':</label> ' . $endate;

      }

    }
    $vars['memberships'] = implode('<br />', $memberships);

  }
  else {
    $vars['memberships'] = '';
  }

}

/**
 * Implements hook_preprocess_preprocess_culturefeed_uitpas_profile_details().
 */
function culturefeed_uitpas_preprocess_culturefeed_uitpas_profile_details_form(&$vars) {

  $form = $vars['form'];

  $vars['first_name'] = drupal_render($form['first_name']);
  $vars['last_name'] = drupal_render($form['last_name']);
  $vars['dob'] = drupal_render($form['dob']);
  $vars['pob'] = drupal_render($form['pob']);
  $vars['gender'] = drupal_render($form['gender']);
  $vars['nationality'] = drupal_render($form['nationality']);
  $vars['street'] = drupal_render($form['street']);
  $vars['nr'] = drupal_render($form['nr']);
  $vars['zip'] = drupal_render($form['zip']);
  $vars['city'] = drupal_render($form['city']);
  $vars['tel'] = drupal_render($form['tel']);
  $vars['mobile'] = drupal_render($form['mobile']);
  $vars['email'] = drupal_render($form['email']);
  $vars['email_description'] = drupal_render($form['email_description']);
  $vars['actions'] = drupal_render($form['actions']);

  $vars['main_form'] = drupal_render_children($form);

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
  $vars['all_promotions'] = l(variable_get('culturefeed_uitpas_profile_summary_more_link_text', t('Show all my promotions')), 'culturefeed/profile/uitpas/promotions');

}

/**
 * Implements hook_preprocess_culturefeed_uitpas_promotion().
 */
function culturefeed_uitpas_preprocess_culturefeed_uitpas_promotion(&$vars) {
  $promotion = $vars['promotion'];

  // Ensure images are protocol-relative
  if (isset($promotion->pictures)) {
    foreach ($promotion->pictures as &$picture) {
      $picture = str_replace('http://', '//', $picture);
    }
  }

  // Image.
  $vars['image'] = '';
  $vars['image2'] = '';
  $vars['images_list'] = '';
  $images = array();
  if (isset($promotion->pictures[0])) {
    $vars['image'] = theme('image', array('path' => $promotion->pictures[0] . '?maxwidth=300&max-height=300', 'alt' => $promotion->title, 'title' => $promotion->title, 'attributes' => array()));
    foreach ($promotion->pictures as $key => $picture) {
      $images[] = l(
        theme('image', array('path' => $promotion->pictures[$key] . '?maxwidth=300&max-height=300', 'alt' => $promotion->title, 'title' => $promotion->title, 'attributes' => array())),
        $promotion->pictures[$key],
        array('html' => TRUE)
      );
    }
    $vars['images_list'] = theme('item_list', array('items' => $images));
  }
  if (isset($promotion->pictures[1])) {
    $vars['image2'] = theme_image(array('path' => $promotion->pictures[1] . '?maxwidth=300&max-height=300', 'alt' => $promotion->title, 'title' => $promotion->title, 'attributes' => array()));
  }

  // Points.
  $vars['points'] = format_plural($promotion->points, '1 point', '@count points');

  // Get the counters information.
  $counters = array(
    '#theme' => 'item_list',
    '#type' => 'ul',
    '#items' => array(),
  );
  foreach ($promotion->counters as $counter) {
    $counters['#items'][] = l($counter->name, 'agenda/a/' . culturefeed_search_slug($counter->name) . '/' . $counter->id);
  }
  $vars['counters'] = count($counters['#items']) ? drupal_render($counters) : '';

  // Stock.
  $vars['out_of_stock'] = FALSE;
  if ($promotion->maxAvailableUnits != NULL && $promotion->unitsTaken >= $promotion->maxAvailableUnits) {
    $vars['out_of_stock'] = TRUE;
  }

  // Period.
  $period = '';
  $now = time();
  if ($promotion->cashingPeriodBegin && $promotion->cashingPeriodBegin > $now) {
    $period = t('From !date', array('!date' => date('j/m/Y', $promotion->cashingPeriodBegin))) . ' ';
    if ($promotion->cashingPeriodEnd) {
      $period .= t('until !date', array('!date' => date('j/m/Y', $promotion->cashingPeriodEnd)));
    }
  }
  else if ($promotion->cashingPeriodEnd) {
    $period .= t('Until !date', array('!date' => date('j/m/Y', $promotion->cashingPeriodEnd)));
  }
  else {
    $period = t('Until end of stock');
  }
  $vars['period'] = $period;

  // Provider.
  $vars['provider'] = '';
  if (isset($promotion->owningCardSystem->name)) {
    $vars['provider'] = t('Provided by @provider', array('@provider' => culturefeed_uitpas_cardsystem_name($promotion->owningCardSystem)));
    $vars['provider_raw'] = culturefeed_uitpas_cardsystem_name($promotion->owningCardSystem);
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

  drupal_add_css(drupal_get_path('module', 'culturefeed_uitpas') . '/css/promotion_advantage.css');
  drupal_add_js(drupal_get_path('module', 'culturefeed_uitpas') . '/js/script.js');
}

/**
 * Implements hook_preprocess_culturefeed_uitpas_promotions_highlight().
 */
function culturefeed_uitpas_preprocess_culturefeed_uitpas_promotions_highlight(&$vars) {

  $promotions = $vars['promotions'];
  $rows = array();

  foreach ($promotions as $promotion) {

    if (isset($promotion->pictures[0])) {
      $thumbnail = theme('image', array('path' => $promotion->pictures[0] . '?width=400&height=300&crop=auto', 'alt' => $promotion->title, 'title' => $promotion->title, 'attributes' => array()));
    }
    else {
      $thumbnail = theme('image', array('path' => drupal_get_path('module', 'culturefeed_uitpas') . '/images/default.png', 'alt' => $promotion->title, 'title' => $promotion->title, 'attributes' => array()));
    }

    $singular = '@count point';
    $plural = '@count points';
    $points = array(
        'data' => format_plural($promotion->points, $singular, $plural),
        'class' => array('points'),
      );
    $url = 'promotion/' . culturefeed_search_slug($promotion->title) . '/' . $promotion->id;
    $title = l($promotion->title, $url);

    $rows[] = array(
      $points,
      $title,
    );

    $items[] = array(
      'thumbnail' => $thumbnail,
      'points' => $points['data'],
      'title' => $title,
      'url' => $url
    );

  }

  $vars['items'] = $items;

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

      // Address.
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

      // Card systems.
      $card_systems = array();
      if (!empty($pos->cardSystems)) {
        foreach ($pos->cardSystems as $card_system) {
          /* @var CultureFeed_Uitpas_CardSystem $card_system */
          $card_systems[] = $card_system->name;
        }
      }

      $table['rows'][] = array(
        l($pos->name, 'agenda/a/' . culturefeed_search_slug($pos->name) . '/' . $pos->id),
        (count($card_systems)) ? theme('item_list', array('items' => $card_systems, 'type' => 'ul')) : '',
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

  $vars['intro'] = t('You can get an UiTPAS at one of these registration counters. An UiTPAS costs € 5. Younger than 18 years? Then you\'ll pay € 2. For people with an opportunities tarrif UiTPAS is free. You\'ll need your eID to register your UiTPAS.');
  $vars['pos_table'] = theme_table($table) . theme('pager', $pager);
  $vars['outro'] = t('Important: you\'ll need your eID to register your UiTPAS. <a href="@read-more">Read more</a> about the UiTPAS project.', array('@read-more' => 'http://www.cultuurnet.be/project/uitpas'));

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
function culturefeed_uitpas_preprocess_culturefeed_uitpas_advantages_table(&$vars) {
  // @codingStandardsIgnoreStart
  /** @var CultureFeed_Uitpas_Passholder_WelcomeAdvantage[] $advantages */
  // @codingStandardsIgnoreEnd
  $advantages = $vars['advantages'];
  $advantages_total = $vars['advantages_total'];
  $advantages_pager_element = $vars['advantages_pager_element'];

  if (count($advantages)) {
    $items = array();

    foreach ($advantages as $key => $advantage) {
      $item = array();
      $advantage_path = 'advantage/' . culturefeed_search_slug($advantage->title) . '/' . $advantage->id;

      // Get the first image or fall back to a default.
      if (isset($advantage->pictures[0])) {
        $image = theme('culturefeed_uitpas_promotion_image', array(
          'path' => $advantage->pictures[0],
          'maxwidth' => 100,
          'maxheight' => 100,
          'alt' => $advantage->title,
          'title' => $advantage->title,
          'attributes' => array(),
        ));
      }
      else {
        $image = theme('image', array(
          'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/images/default.png',
          'width' => 100,
          'height' => 100,
          'alt' => $advantage->title,
          'title' => $advantage->title,
        ));
      }
      $item['image'] = l($image, $advantage_path, array('html' => TRUE));
      $item['classes'][] = ($key % 2) ? 'even' : 'odd';

      $item['cardsystem'] = array(
        'class' => drupal_html_class($advantage->owningCardSystem->name),
        'name' => $advantage->owningCardSystem->name,
      );
      $item['title'] = l($advantage->title, $advantage_path);
      $item['overlay_link'] = l('<span class="hyperspan"></span>', $advantage_path, array('html' => TRUE, 'attributes' => array('class' => 'hulk-link')));

      foreach ($advantage->counters as $counter) {
        $item['counters'][] = array(
          'class' => drupal_html_class($counter->name),
          'name' => $counter->name,
        );
      }

      $points_classes = array();
      if ($advantage->cashingPeriodEnd) {
        $item['availability'] = date('j/n/Y', $advantage->cashingPeriodEnd);
      }
      else {
        $item['availability'] = t('Until end of stock');
        $points_classes[] = 'until-end-of-stock';
      }
      $item['availability'] = ($advantage->cashingPeriodEnd) ? date('j/n/Y', $advantage->cashingPeriodEnd) : t('Until end of stock');
      $item['points'] = array(
        'value' => format_plural($advantage->points, '1 point', '@count points'),
        'remark' => t('This is a welcome advantage'),
        'classes' => implode(' ', $points_classes),
      );

      $items[] = $item;
    }

    $vars['items'] = $items;
    $pager = array(
      'element' => $advantages_pager_element,
      'quantity' => $advantages_total,
    );
    $vars['pager'] = theme('pager', $pager);
    drupal_add_css(drupal_get_path('module', 'culturefeed_uitpas') . '/css/advantages.css');
  }
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
function culturefeed_uitpas_preprocess_culturefeed_uitpas_promotions_table(&$vars) {
  // @codingStandardsIgnoreStart
  /** @var CultureFeed_Uitpas_Passholder_PointsPromotion[] $promotions */
  // @codingStandardsIgnoreEnd
  $promotions = $vars['promotions'];

  if (count($promotions)) {
    $items = array();
    $cf_account = DrupalCultureFeed::getLoggedInUser();

    foreach ($promotions as $key => $promotion) {
      $item = array();
      $promotion_path = 'promotion/' . culturefeed_search_slug($promotion->title) . '/' . $promotion->id;

      // Get the first image or fall back to a default.
      if (isset($promotion->pictures[0])) {
        $image = theme('culturefeed_uitpas_promotion_image', array(
          'path' => $promotion->pictures[0],
          'maxwidth' => 100,
          'maxheight' => 100,
          'alt' => $promotion->title,
          'title' => $promotion->title,
          'attributes' => array(),
        ));
      }
      else {
        $image = theme('image', array(
          'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/images/default.png',
          'width' => 100,
          'height' => 100,
          'alt' => $promotion->title,
          'title' => $promotion->title,
        ));
      }
      $item['image'] = l($image, $promotion_path, array('html' => TRUE));
      $item['classes'][] = ($key % 2) ? 'even' : 'odd';
      $item['classes'][] = drupal_clean_css_identifier('cashin-' . drupal_strtolower($promotion->cashInState));

      $item['cardsystem'] = array(
        'class' => drupal_html_class($promotion->owningCardSystem->name),
        'name' => $promotion->owningCardSystem->name,
      );
      $item['title'] = l($promotion->title, $promotion_path);
      $item['overlay_link'] = l('<span class="hyperspan"></span>', $promotion_path, array('html' => TRUE, 'attributes' => array('class' => 'hulk-link')));

      $item['counters'] = array();
      foreach ($promotion->counters as $counter) {
        $item['counters'][] = array(
          'class' => drupal_html_class($counter->name),
          'name' => $counter->name,
        );
      }

      $points_classes = array();
      if ($promotion->cashInState == $promotion::CASHIN_NOT_POSSIBLE_VOLUME_CONSTRAINT) {
        $item['availability'] = t('Out of stock');
        $points_classes[] = 'until-end-of-stock';
      }
      else if ($promotion->cashingPeriodEnd) {
        $item['availability'] = t('Until @date', array('@date' => date('j/n/Y', $promotion->cashingPeriodEnd)));
      }
      else if ($promotion->cashingPeriodBegin && $promotion->cashingPeriodBegin > time()) {
        $item['availability'] = t('From @date', array('@date' => date('j/n/Y', $promotion->cashingPeriodBegin)));
        $points_classes[] = 'available-from';
      }
      else {
        $item['availability'] = t('Until end of stock');
      }

      $points_remark = FALSE;
      if ($cf_account) {
        if ($cf_account->points < $promotion->points) {
          $points_remark = t('You have insufficient points');
          $points_classes[] = 'insufficient-points';
        }
      }
      $item['points'] = array(
        'value' => format_plural($promotion->points, '1 point', '@count points'),
        'remark' => $points_remark,
        'classes' => implode(' ', $points_classes),
      );

      $items[] = $item;
    }

    $vars['items'] = $items;
    drupal_add_css(drupal_get_path('module', 'culturefeed_uitpas') . '/css/promotions.css');
  }
}
