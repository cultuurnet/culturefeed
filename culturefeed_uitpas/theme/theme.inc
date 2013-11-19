<?php

/**
 * @file
 * Culturefeed uitpas preprocess functions.
 */

/**
 * Implements hook_preprocess_culturefeed_uitpas_promotion().
 */
function culturefeed_uitpas_preprocess_culturefeed_uitpas_promotion(&$vars) {

  $promotion = $vars['promotion'];

  // Image.
  $vars['image'] = '';
  if (isset($promotion->pictures[0])) {
    $vars['image'] = theme_image(array('path' => $promotion->pictures[0], 'attributes' => array()));
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
    $vars['provider'] = t('Provided by !provider', array('!provider' => $promotion->owningCardSystem->name));
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
