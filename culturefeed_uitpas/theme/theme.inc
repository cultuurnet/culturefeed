<?php

/**
 * @file
 * Culturefeed uitpas preprocess functions.
 */

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
    '',
    t('Welcome advantages'),
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
        (isset($advantage->pictures[0])) ? theme_image(array('path' => $advantage->pictures[0], 'attributes' => array())) : '',
        l($advantage->title, 'advantage/' . culturefeed_search_slug($advantage->title) . '/' . $advantage->id),
        ($advantage->cashingPeriodEnd) ? date('j/n/Y', $advantage->cashingPeriodEnd) : t('end of stock'),
        ($advantage->description1) ? $advantage->description1 : '',
        implode(', ', $counters),
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
    '',
    array('data' => t('Promotions'), 'colspan' => 2),
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
        (isset($promotion->pictures[0])) ? theme_image(array('path' => $promotion->pictures[0], 'attributes' => array())) : '',
        array('data' => $points, 'class' => array('uitpas_ui_points')),
        l($promotion->title, 'promotion/' . culturefeed_search_slug($promotion->title) . '/' . $promotion->id),
        ($promotion->cashingPeriodEnd) ? date('j/n/Y', $promotion->cashingPeriodEnd) : t('end of stock'),
        ($promotion->description1) ? $promotion->description1 : '',
        implode(', ', $counters),
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
    $image = ($action->depiction) ? $action->depiction : variable_get('culturefeed_uitpas_user_default_image');
    if ($image) {
      $image = theme_image(array('path' => $image, 'attributes' => array()));
    }

    $list['items'][] = $image . format_plural($action->points, $singular, $plural, $args);

  }

  $vars['actions'] = theme_item_list($list);

}
