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
        'title' => t('My UiTPAS'),
        'href' => 'culturefeed/profile/uitpas',
        'attributes' => array(
          'title' => t('Check your personal UiTPAS data and cardnumber.'),
        ),
      ),
      array(
        'title' => t('My promotions'),
        'href' => 'culturefeed/profile/uitpas/promotions',
        'attributes' => array(
          'title' => t('Find out which promotions and advantages you are entitled to.'),
        ),
      ),
      array(
        'title' => t('Points history'),
        'href' => 'culturefeed/profile/uitpas/actions',
        'attributes' => array(
          'title' => t('An overview of all your UiTPAS actions.'),
        ),
      ),
    );

    return array(
      'classes' => array('uitpas'),
      'content' => theme('culturefeed_uitpas_profile_section', array(
        'links' => theme('links', array('links' => $links)),
        'points_label' => t('Points balance'),
        'points_value' => $passholder->points,
      )),
      'title' => variable_get('culturefeed_uitpas_card_name', 'UiTPAS'),
    );

  }

  else {
    return array(
      'classes' => array('uitpas'),
      'content' => theme('culturefeed_uitpas_profile_section_register'),
      'title' => variable_get('culturefeed_uitpas_card_name', 'UiTPAS'),
    );
  }

}
