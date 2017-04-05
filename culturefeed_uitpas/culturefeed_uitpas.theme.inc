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
        'items' => array(),
        'pager' => FALSE,
      ),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'template' => 'culturefeed-uitpas-advantages-table',
      'file' => 'theme.inc',
    ),
    'culturefeed_uitpas_event_details_advantages' => array(
      'file' => 'theme.inc',
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'template' => 'culturefeed-uitpas-event-details-advantages',
      'variables' => array(
        'title' => '',
        'show_all_link_title' => '',
        'show_all_link_path' => '',
        'show_all_link_default_render' => '',
        'promotions' => array(),
        'promotions_array' => array(),
        'promotions_default_render' => '',
      ),
    ),
    'culturefeed_uitpas_profile_section' => array(
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'template' => 'culturefeed-uitpas-profile-section',
    ),
    'culturefeed_uitpas_profile_section_register' => array(
      'variables' => array(
        'intro_title' => NULL,
        'intro_text' => NULL,
        'cta_link' => NULL,
      ),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'template' => 'culturefeed-uitpas-profile-section-register',
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
    'culturefeed_uitpas_promotions' => array(
      'variables' => array(
        'promotions' => array(),
        'promotions_total' => 0,
        'promotions_pager_element' => 0,
        'promotions_filter' => array(),
        'promotions_pager_min' => 0,
        'promotions_pager_max' => 0,
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
        'items' => array(),
        'pager' => FALSE,
      ),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'template' => 'culturefeed-uitpas-promotions-table',
      'file' => 'theme.inc',
    ),
    'culturefeed_uitpas_promotions_highlight' => array(
      'variables' => array('promotions' => array()),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
      'template' => 'culturefeed-uitpas-promotions-highlight',
    ),
    'culturefeed_uitpas_profile_actions' => array(
      'variables' => array(
        'activities' => array(),
      ),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
      'template' => 'culturefeed-uitpas-profile-actions',
    ),
    'culturefeed_uitpas_profile_promotions' => array(
      'variables' => array(
        'promotions' => array(),
        'promotions_total' => 0,
        'promotions_pager_min' => 0,
        'promotions_pager_max' => 0,
      ),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
      'template' => 'culturefeed-uitpas-profile-promotions',
    ),
    'culturefeed_uitpas_profile_advantages' => array(
      'variables' => array(
        'advantages' => array(),
        'advantages_total' => 0,
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
    'culturefeed_uitpas_profile_details_form' => array(
      'render element' => 'form',
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
      'template' => 'culturefeed-uitpas-profile-details-form',
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
    'culturefeed_uitpas_filter_labels' => array(
      'render element' => 'element',
      'template' => 'culturefeed-uitpas-filter-labels',
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
    ),
    'culturefeed_uitpas_register_cta' => array(
      'variables' => array(
        'intro' => NULL,
        'register_link' => NULL,
        'get_title' => NULL,
        'get_link' => NULL,
      ),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'template' => 'culturefeed-uitpas-register-cta',
    ),
  );
}

function culturefeed_uitpas_preprocess_culturefeed_actor(&$vars) {

  // Get promotions for known uitpas locations.
  if (isset($vars['uitpas'])) { 
    // Promotions.
    module_load_include('inc', 'culturefeed_uitpas', 'includes/event_details_advantages');
    $promotions = culturefeed_uitpas_event_details_advantages_get();
    $vars['uitpas_promotions'] = drupal_render($promotions);
  }

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
              $prices[] = t('Opportunities tariff with @system: &euro; !tariff', array('@system' => culturefeed_uitpas_cardsystem_name($card_system), '!tariff' => $key->tariff));
            }

            if ($condition->value == $condition::VALUE_AT_LEAST_ONE_CARDSYSTEM) {
              $prices[] = t('Opportunities tariff for pass holders from another region: &euro; !tariff', array('!tariff' => $key->tariff));
            }

          }

        }

      }

    }

    foreach ($prices as $key => $price) {
      $prices[$key] = str_replace('.', ',', $price);
    }

    if (count($prices)) {
      $vars['price'] = '<p>' . implode('</p><p>', array_unique($prices)) . '</p>';
    }

    // Promotions.
    module_load_include('inc', 'culturefeed_uitpas', 'includes/event_details_advantages');
    $promotions = culturefeed_uitpas_event_details_advantages_get();
    $vars['uitpas_promotions'] = drupal_render($promotions);

  }

}

/**
 * Preprocess function to add classes to radio input elements.
 */
function culturefeed_uitpas_preprocess_culturefeed_uitpas_filter_labels(&$variables) {
  $attributes = array();
  if (isset($variables['element']['#id'])) {
    $attributes['id'] = $variables['element']['#id'];
  }
  $attributes['class'] = 'form-radios';
  if (!empty($variables['element']['#attributes']['class'])) {
    $attributes['class'] .= ' ' . implode(' ', $variables['element']['#attributes']['class']);
  }
  if (isset($variables['element']['#attributes']['title'])) {
    $attributes['title'] = $variables['element']['#attributes']['title'];
  }
  foreach (element_children($variables['element']) as $child) {
    $variables['element'][$child]['#prefix'] = '<span class="' . drupal_html_class($variables['element'][$child]['#title']) . '">';
    $variables['element'][$child]['#suffix'] = '</span>';
    $variables['element'][$child]['#attributes']['data-counter'] = drupal_html_class($variables['element'][$child]['#title']);
    $rendered_child = drupal_render($variables['element'][$child]);

    $variables['element']['#children'] .= $rendered_child;
  }
  $variables['#attributes'] = $attributes;
}

/**
 * Implements hook_preprocess_culturefeed_uitpas_register_cta().
 */
function culturefeed_uitpas_preprocess_culturefeed_uitpas_register_cta(&$vars) {
  $vars['intro'] = t('Register here, so you can follow your UiTPAS advantages and points balance online.');
  $vars['register_link'] = l(t('Register your UiTPAS'), 'culturefeed/oauth/connect', array('query' => array('destination' => 'register_uitpas')));
  $vars['get_title'] = t('No UiTPAS yet?');
  $vars['get_link'] = l(t('Get an UiTPAS'), 'register_where');
}

/**
 * Implements hook_preprocess_culturefeed_uitpas_profile_section_register().
 */
function culturefeed_uitpas_preprocess_culturefeed_uitpas_profile_section_register(&$vars) {

  if(culturefeed_uitpas_not_yet_registered()) {
    $vars['intro_title'] = t('You did not register your UiTPAS yet.');
    $vars['intro_text'] = t('Register here, so you can follow your UiTPAS advantages and points balance online.');
    $vars['cta_link'] = l(t('Register your UiTPAS'), 'register_uitpas');
  }

  else {
    $vars['intro_title'] = t('No UiTPAS yet?');
    $vars['intro_text'] = t('Holders of an UiTPAS can earn points by participating in leisure activities and exchange them for') . ' ' . l(t('nice benefits'), 'promotions') . '.';
    $vars['cta_link'] = l(t('Get an UiTPAS'), 'register_where');
  }
}
