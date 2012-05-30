<?php

/**
 * @file
 * Plugin to provide a content type for uitpas user profile.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Uitpas: user profile'),
  'description' => t('Uitpas: user profile.'),
  'category' => t('Uitpas'),
  'required context' => new ctools_context_required(t('Uitpas user'), 'uitpas_user'),
);

function uitpas_ui_uitpas_user_profile_content_type_edit_form($form, &$form_state) {
  return $form;
}

function uitpas_ui_uitpas_user_profile_content_type_render($subtype, $conf, $args, $context) {
  if ($context->data && $context->data->passholder) {
    $uitpas_user = $context->data->user;
    $passholder = $context->data->passholder;
    $cf = $context->data->cf;
    $advantages = array();
    $promotions = array();
    $coming_promotions = array();
    //welcome advantages
    try {
      $query = new CultureFeed_Uitpas_Passholder_Query_WelcomeAdvantagesOptions();
      $query->uitpas_number = $passholder->uitpasNumber;
      $query->cashedIn = FALSE;
      $query->max = 2;
      $result = $cf->uitpas()->getWelcomeAdvantagesForPassholder($query);
      if ($result->total) {
        $advantages = $result->objects;
      }
    } 
    catch (Exception $e) {
      watchdog_exception('uitpas_ui_content_type_user_profile_welcome_advantages', $e);
    }
    //promotions
    try {
      $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
      $query->filterOnUserPoints = TRUE;
      $query->max = 2;
      $result = $cf->uitpas()->getPromotionPoints($query);
      if ($result->total) {
        $promotions = $result->objects;
      }
    }
    catch (Exception $e) {
      watchdog_exception('uitpas_ui_content_type_user_profile_promotions', $e);
    }
    //coming promotions
    try {
      $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
      $query->minPoints = $passholder->points + 1;
      $query->max = 2;
      $result = $cf->uitpas()->getPromotionPoints($query);
      if ($result->total) {
        $coming_promotions = $result->objects;
      }
    }
    catch (Exception $e) {
      watchdog_exception('uitpas_ui_content_type_user_profile_promotions_nearby', $e);
    }
    //ctools block
    $block = new stdClass();
    $block->title = t('My UITPAS');
    $block->content = theme('uitpas_ui_user_profile', array(
      'uitpas_user' => $uitpas_user,
      'passholder' => $passholder,
      'advantages' => $advantages,
      'promotions' => $promotions,
      'coming_promotions' => $coming_promotions,
    ));
    return $block;
  }
}