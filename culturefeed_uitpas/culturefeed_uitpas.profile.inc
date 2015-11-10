<?php

/**
 * @file
 * Profile implementations for Culturefeed uitpas.
 */

/**
 * Implements hook_culturefeed_ui_profile_sections().
 */
function culturefeed_uitpas_culturefeed_ui_profile_sections() {

  if (culturefeed_uitpas_is_passholder()) {

    // Points.
    $user_info = culturefeed_uitpas_get_uitpas_user();
    /* @var CultureFeed_Uitpas_Passholder $passholder */
    $passholder = $user_info->passholder;

    $links = array(
      array(
        'title' => variable_get('culturefeed_uitpas_profile_details_title', t('My UiTPAS')),
        'href' => 'culturefeed/profile/uitpas',
      ),
      array(
        'title' => variable_get('culturefeed_uitpas_profile_details_menu_advantages_title', t('My advantages')),
        'href' => 'culturefeed/profile/uitpas/promotions',
        'attributes' => array(
          'title' => variable_get('culturefeed_uitpas_profile_details_menu_advantages_description', t('Discover which promotions and advantages you are entitled to.')),
        ),
      ),
      array(
        'title' => variable_get('culturefeed_uitpas_profile_details_menu_actions_title', t('Points history')),
        'href' => 'culturefeed/profile/uitpas/actions',
        'attributes' => array(
          'title' => variable_get('culturefeed_uitpas_profile_details_menu_actions_description', t('An overview of all your UiTPAS actions.')),
        ),
      ),
    );

    return array(
      'classes' => array('uitpas'),
      'content' => theme('culturefeed_uitpas_profile_section', array(
        'links' => theme('links', array('links' => $links)),
        'points_label' => t('Points'),
        'points_value' => $passholder->points,
      )),
      'title' => variable_get('culturefeed_uitpas_card_name', 'UiTPas'),
    );

  }

}

