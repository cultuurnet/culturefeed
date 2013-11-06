<?php

/**
 * @file
 * Culturefeed uitpas preprocess functions.
 */

/**
 * Implements hook_preprocess_culturefeed_uitpas_my_uitpas_summary().
 */
function culturefeed_uitpas_preprocess_culturefeed_uitpas_my_uitpas_summary(&$vars) {

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

  $vars['name'] = l($uitpas_user->nick, 'myprofile/details');
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
      'href' => uitpas_ui_helpers_link_object('advantage', $advantage),
    );
  }
  $vars['advantages'] = theme_links($links);

  // Promotions.
  $vars['promotions_title'] = variable_get('culturefeed_uitpas_my_uitpas_summary_promotions_title', t('Your UiTPAS advantages'));
  $links = array(
    'links' => array(),
    'heading' => array(),
    'attributes' => array(),
  );
  foreach ($promotions as $promotion) {
    $links['links'][] = array(
      'title' => $promotion->title . ' (' . t('!points points', array('!points' => $promotion->points)) . ')',
      'href' => '</front>', /*uitpas_ui_helpers_link_object('promotion', $promotion),*/
    );
  }
  $vars['promotions'] = theme_links($links);

  // Upcoming promotions.
  $vars['upcoming_promotions_title'] = variable_get('culturefeed_uitpas_my_uitpas_summary_upcoming_promotions_title', t('Just a few points more for'));
  $links = array(
    'links' => array(),
    'heading' => array(),
    'attributes' => array(),
  );
  foreach ($upcoming_promotions as $promotion) {
    $links['links'][] = array(
      'title' => $promotion->title . ' (' . t('!points points', array('!points' => $promotion->points)) . ')',
      'href' => '<front>', /*uitpas_ui_helpers_link_object('promotion', $promotion),*/
    );
  }
  $vars['upcoming_promotions'] = (count($upcoming_promotions)) ? theme_links($links) : '';

  // All promotions.
  $vars['all_promotions'] = l(variable_get('culturefeed_uitpas_my_uitpas_summary_more_link_text', t('Show all my promotions')), 'myprofile/advantages');

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
