<?php

/**
 * @file
 * Culturefeed uitpas preprocess functions.
 */

/**
 * Theme function.
 */
function theme_culturefeed_uitpas_promotion_picture($vars) {
  $vars['path'] .= '?maxheight=120&maxwidth=120';

  return theme('image', $vars);
}

/**
 * Implements hook_preprocess_culturefeed_uitpas_profile_advantages().
 */
function culturefeed_uitpas_preprocess_culturefeed_uitpas_profile_advantages(&$vars) {

  $advantages = $vars['advantages'];
  $advantages_total = $vars['advantages_total'];
  $advantages_pager_element = $vars['advantages_pager_element'];

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
  /*$pager = $base_pager + array('element' => $advantages_pager_element, 'quantity' => $advantage_total);*/
  $vars['advantages'] = theme_table($table); /* . theme_pager($pager);*/

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
