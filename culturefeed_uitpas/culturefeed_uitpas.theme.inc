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
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
      'template' => 'culturefeed-uitpas-advantage',
    ),
    /*'culturefeed_uitpas_advantages_promotions' => array(
      'variables' => array(
        'advantages' => array(),
        'advantages_total' => 0,
        'advantages_pager_element' => 0,
        'promotions' => array(),
        'promotions_total' => 0,
        'promotions_pager_element' => 0,
      ),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
      'template' => 'culturefeed-uitpas-advantages-promotions',
    ),*/
    'culturefeed_uitpas_advantages' => array(
      'variables' => array(
        'advantages' => array(),
        'advantages_total' => 0,
        'advantages_pager_element' => 0,
      ),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
      'template' => 'culturefeed-uitpas-advantages',
    ),
      'culturefeed_uitpas_advantages_table' => array(
          'variables' => array(
              'title' => t('Advantages'),
              'advantages' => array(),
              'advantages_total' => 0,
              'advantages_pager_element' => 0,
          ),
          'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
          'file' => 'theme.inc',
      ),
    'culturefeed_uitpas_promotions' => array(
      'variables' => array(
        'promotions' => array(),
        'promotions_total' => 0,
        'promotions_pager_element' => 0,
        'promotions_filter' => array(),
      ),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
      'template' => 'culturefeed-uitpas-promotions',
    ),
      'culturefeed_uitpas_promotions_table' => array(
          'variables' => array(
              'title' => t('Promotions'),
              'promotions' => array(),
              'promotions_total' => 0,
              'promotions_pager_element' => 0,
          ),
          'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
          'file' => 'theme.inc',
      ),
    'culturefeed_uitpas_profile_actions' => array(
      'variables' => array(
        'actions' => array(),
        'actions_total' => 0,
        'actions_pager_element' => 0,
      ),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
      'template' => 'culturefeed-uitpas-profile-actions',
    ),
    'culturefeed_uitpas_profile_advantages' => array(
      'variables' => array(
        'advantages' => array(),
        'advantages_total' => 0,
        'advantages_pager_element' => 0,
        'promotions' => array(),
        'promotions_total' => 0,
        'promotions_pager_element' => 0,
        'upcoming_promotions' => array(),
        'upcoming_promotions_total' => 0,
        'upcoming_promotions_pager_element' => 0,
      ),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
      'template' => 'culturefeed-uitpas-profile-advantages',
    ),
    'culturefeed_uitpas_profile_details' => array(
      'variables' => array(
        'uitpas_user' => NULL,
        'passholder' => NULL,
      ),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
      'template' => 'culturefeed-uitpas-profile-details',
    ),
    'culturefeed_uitpas_profile_summary' => array(
      'variables' => array(
        'uitpas_user' => NULL,
        'passholder' => NULL,
        'advantages' => array(),
        'promotions' => array(),
        'upcoming_promotions' => array(),
      ),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
      'template' => 'culturefeed-uitpas-profile-summary',
    ),
    'culturefeed_uitpas_promotion' => array(
      'variables' => array('promotion' => NULL),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
      'template' => 'culturefeed-uitpas-promotion',
    ),
    'culturefeed_uitpas_promotion_image' => array(
      'variables' => array(
        'path' => NULL,
        'maxheight' => 120,
        'maxwidth' => 120,
      ),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
    ),
    /*'culturefeed_uitpas_promotions' => array(
      'variables' => array(
        'title' => t('Promotions'),
        'promotions' => array(),
        'promotions_total' => 0,
        'promotions_pager_element' => 0,
      ),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
    ),*/
    'culturefeed_uitpas_promotions_highlight' => array(
      'variables' => array('promotions' => array()),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
      'template' => 'culturefeed-uitpas-promotions-highlight',
    ),
    'culturefeed_uitpas_recent_actions' => array(
      'variables' => array('actions' => array()),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
      'template' => 'culturefeed-uitpas-recent-actions',
    ),
    'culturefeed_uitpas_register_where' => array(
      'variables' => array(
        'pos' => array(),
        'pos_total' => 0,
        'pos_pager_element' => 0,
        'filters' => array(),
        'actors' => array(),
      ),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
      'template' => 'culturefeed-uitpas-register-where',
    ),

  );

}

/**
 * Preprocess function for culturefeed_agenda_event.
 */
function culturefeed_uitpas_preprocess_culturefeed_event(&$vars) {

  $event = culturefeed_uitpas_get_uitpas_event($vars['item']);
  if (isset($vars['price'])) {
    $price = $vars['price'];
  }

  if ($event) {

    $prices = array();

    if (isset($price)) {
      $prices[] = $price;
    }

    foreach ($event->cardSystems as $card_system) {
      foreach ($card_system->distributionKeys as $key) {
        foreach ($key->conditions as $condition) {

          if ($condition->definition == $condition::DEFINITION_KANSARM && $key->tariff > 0) {

            if ($condition->value == $condition::VALUE_MY_CARDSYSTEM) {
              $prices[] = t('Opportunities tariff with @system: !tariff', array('@system' => culturefeed_uitpas_cardsystem_name($card_system), '!tariff' => $key->tariff));
            }

            if ($condition->value == $condition::VALUE_AT_LEAST_ONE_CARDSYSTEM) {
              $prices[] = t('Opportunities tariff for pass holders from another region: !tariff', array('!tariff' => $key->tariff));
            }

          }

        }

      }

    }

    if (count($prices)) {
      $vars['price'] = implode('<br />', array_unique($prices));
    }

  }

}
