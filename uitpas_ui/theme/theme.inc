<?php

/**
 * @file
 * Uitpas preprocess functions.
 */
 
 /**
 * Implements hook_preprocess_uitpas_ui_advantage() for uitpas-ui-user-advantage.
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
 * Implements hook_preprocess_uitpas_ui_promotion() for uitpas-ui-user-promotion.
 */
function uitpas_ui_preprocess_uitpas_ui_promotion(&$vars) {
  $promotion = $vars['promotion'];
  $vars['title'] = $promotion->title;
  $vars['points'] = t('Points needed') . ':' . t('!points points', array('!points' => $promotion->points));
  $vars['period'] = '';
  if ($promotion->cashingPeriodBegin) {
    $period = t('From !date', array('!date' => date('j/n/Y', $promotion->cashingPeriodBegin)));
    if ($promotion->cashingPeriodEnd) {
      $period .= ' ' . t('to !date', array('!date' => date('j/n/Y', $promotion->cashingPeriodEnd)));
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
 * Implements hook_preprocess_uitpas_ui_user_profile() for uitpas-ui-user-profile.
 */
function uitpas_ui_preprocess_uitpas_ui_user_profile(&$vars) {
  $uitpas_user = $vars['uitpas_user'];
  $passholder = $vars['passholder'];
  $advantages = $vars['advantages'];
  $promotions = $vars['promotions'];
  $nearby_promotions = $vars['nearby_promotions'];
  if (isset($uitpas_user->depiction)) {
    $vars['image'] = theme_image(array('path' => $uitpas_user->depiction, 'attributes' => array()));
  }
  else {
    $vars['image'] = '';
  }
  $vars['name'] = l($uitpas_user->nick, 'myprofile');
  $vars['points'] = t('%points points aved', array('%points' => $passholder->points));
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
  //nearby promotions
  $links = array('links' => array(), 'heading' => array(), 'attributes' => array());
  foreach ($nearby_promotions as $promotion) {
    $links['links'][] = array(
      'title' => $promotion->title,
      'href' => 'promotion/' . $promotion->id,
    );
  }
  $vars['nearby_promotions'] = theme_links($links);
}