<?php

/**
 * @file
 * Culturefeed uitpas preprocess functions.
 */

/**
 * Implements hook_preprocess_culturefeed_uitpas_profile_advantages().
 */
function culturefeed_uitpas_preprocess_culturefeed_uitpas_profile_advantages(&$vars) {

  $advantages = $vars['advantages'];
  $promotions = $vars['promotions'];
  $upcoming_promotions = $vars['upcoming_promotions'];

  $base_table = array(
    'attributes' => array(),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => '',
    'empty' => '',
  );

  // Promotions.
  $header = array(
    array('data' => t('Trade-in options'), 'colspan' => 2),
    array('data' => t('valid till')),
  );
  $rows = array();
  if (count($promotions)) {

    foreach ($promotions as $promotion) {

      $singular = t('1 point');
      $plural = t('!points points', array('!points' => $promotion->points));
      $points = format_plural($promotion->points, $singular, $plural);
      $rows[] = array(
        array('data' => $points, 'class' => array('uitpas_ui_points')),
        l($promotion->title, 'promotion/' . culturefeed_search_slug($promotion->title) . '/' . $promotion->id),
        ($promotion->cashingPeriodEnd) ? date('j/n/Y', $promotion->cashingPeriodEnd) : t('end of stock'),
      );

    }

  }
  else {
    $rows[] = array(array('data' => t('No results found.'), 'colspan' => 3));
  }
  $table = $base_table + array('header' => $header, 'rows' => $rows);
  $vars['promotions'] = theme_table($table);

  // Upcoming promotions.
  $header = array(
    array('data' => t('Coming trade-in options'), 'colspan' => 2),
    array('data' => t('valid till')),
  );
  $rows = array();
  if (count($upcoming_promotions)) {

    foreach ($upcoming_promotions as $promotion) {

      $singular = t('1 point');
      $plural = t('!points points', array('!points' => $promotion->points));
      $points = format_plural($promotion->points, $singular, $plural);
      $rows[] = array(
        array('data' => $points, 'class' => array('uitpas_ui_points')),
        l($promotion->title, 'promotion/' . culturefeed_search_slug($promotion->title) . '/' . $promotion->id),
        ($promotion->cashingPeriodEnd) ? date('j/n/Y', $promotion->cashingPeriodEnd) : t('end of stock'),
      );

    }

  }
  else {
    $rows[] = array(array('data' => t('No results found.'), 'colspan' => 3));
  }
  $table = $base_table + array('header' => $header, 'rows' => $rows);
  $vars['upcoming_promotions'] = theme_table($table);

  // Advantages.
  $header = array(
    array('data' => t('Advantages')),
    array('data' => t('valid till')),
  );
  $rows = array();
  if (count($advantages)) {

    foreach ($advantages as $advantage) {

      $rows[] = array(
        l($advantage->title, 'advantage/' . culturefeed_search_slug($advantage->title) . '/' . $advantage->id),
        ($advantage->cashingPeriodEnd) ? date('j/n/Y', $advantage->cashingPeriodEnd) : t('end of stock'),
      );

    }

  }
  else {
    $rows[] = array(array('data' => t('No results found.'), 'colspan' => 3));
  }
  $table = $base_table + array('header' => $header, 'rows' => $rows);
  $vars['advantages'] = theme_table($table);

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
