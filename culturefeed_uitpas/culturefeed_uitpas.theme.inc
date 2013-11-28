<?php

/**
 * @file
 * Theme implementations for Culturefeed uitpas.
 */

/**
 * Implements hook_theme().
 */
function culturefeed_uitpas_theme() {

  return array(
    'culturefeed_uitpas_advantage' => array(
      'variables' => array('advantage' => NULL),
      'template' => 'culturefeed-uitpas-advantage',
      'file' => 'theme.inc',
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
    ),
    'culturefeed_uitpas_advantages_promotions' => array(
      'variables' => array(
        'advantages' => array(),
        'advantages_total' => 0,
        'advantages_pager_element' => 0,
        'promotions' => array(),
        'promotions_total' => 0,
        'promotions_pager_element' => 0,
      ),
      'file' => 'theme.inc',
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'template' => 'culturefeed-uitpas-advantages-promotions',
    ),
    'culturefeed_uitpas_profile_summary' => array(
      'variables' => array(
        'uitpas_user' => NULL,
        'passholder' => NULL,
        'advantages' => array(),
        'promotions' => array(),
        'upcoming_promotions' => array(),
      ),
      'template' => 'culturefeed-uitpas-profile-summary',
      'file' => 'theme.inc',
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
    ),
    'culturefeed_uitpas_promotion' => array(
      'variables' => array('promotion' => NULL),
      'template' => 'culturefeed-uitpas-promotion',
      'file' => 'theme.inc',
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
    ),
    'culturefeed_uitpas_promotions_highlight' => array(
      'variables' => array('promotions' => array()),
      'template' => 'culturefeed-uitpas-promotions-highlight',
      'file' => 'theme.inc',
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
    ),
    'culturefeed_uitpas_recent_actions' => array(
      'variables' => array('actions' => array()),
      'template' => 'culturefeed-uitpas-recent-actions',
      'file' => 'theme.inc',
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
    ),
  );

}

/**
 * Preprocess function for culturefeed_agenda_event.
 */
function culturefeed_uitpas_preprocess_culturefeed_event(&$vars) {

  $event = culturefeed_uitpas_get_uitpas_event($vars['item']);

  if ($event) {

    $prices = array();

    foreach ($event->cardSystems as $card_system) {
      foreach ($card_system->distributionKeys as $key) {
        foreach ($key->conditions as $condition) {

          if ($condition->definition == $condition::DEFINITION_KANSARM) {

            if ($condition->value == $condition::VALUE_MY_CARDSYSTEM) {
              $prices[] = t('Opportunities tariff with !system: !tariff', array('!system' => $card_system->name, '!tariff' => $key->tariff));
            }

            if ($condition->value == $condition::VALUE_AT_LEAST_ONE_CARDSYSTEM) {
              $prices[] = t('Opportunities tariff for pass holders uit other region: !tariff', array('!tariff' => $key->tariff));
            }

          }

        }

      }

    }

    if (count($prices)) {
      $vars['price'] = implode('<br />', $prices);
    }

  }

}
