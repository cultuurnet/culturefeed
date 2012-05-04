<?php

/**
 * @file
 * Plugin to provide a content type for uitpas user profile advantages.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Uitpas: user profile advantages'),
  'description' => t('Uitpas: user profile advantages.'),
  'category' => t('Uitpas'),
  'required context' => new ctools_context_required(t('Uitpas user'), 'uitpas_user'),
);

function uitpas_ui_uitpas_user_profile_advantages_content_type_edit_form($form, &$form_state) {
  return $form;
}

function uitpas_ui_uitpas_user_profile_advantages_content_type_render($subtype, $conf, $args, $context) {
  if ($context->data) {
    $uitpas_user = $context->data->user;
    $passholder = $context->data->passholder;
    $advantages = array();
    $promotions = array();
    $cashed_in_promotions = array();
    $cashed_in_advantages = array();
    $cf = $context->data->cf;
    //welcome advantages
    try {
      $query = new CultureFeed_Uitpas_Passholder_Query_WelcomeAdvantagesOptions();
      $query->uitpas_number = $passholder->uitpasNumber;
      $result = $cf->uitpas()->getWelcomeAdvantagesForPassholder($query);
      if ($result->total) {
        $advantages = $result->objects;
      }
    } 
    catch (Exception $e) {
      watchdog_exception('uitpas_ui_content_type_user_profile_advantages', $e);
    }
    //promotions
    try {
      $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
      $result = $cf->uitpas()->getPromotionPoints($query);
      if ($result->total) {
        $promotions = $result->objects;
      }
    }
    catch (Exception $e) {
      watchdog_exception('uitpas_ui_content_type_user_profile_promotions', $e);
    }
    //cashed in advantages
    try {
      $query = new CultureFeed_Uitpas_Passholder_Query_WelcomeAdvantagesOptions();
      $query->uitpas_number = $passholder->uitpasNumber;
      $query->cashedIn = TRUE;
      $result = $cf->uitpas()->getWelcomeAdvantagesForPassholder($query);
      if ($result->total) {
        $cashed_in_advantages = $result->objects;
      }
    } 
    catch (Exception $e) {
      watchdog_exception('uitpas_ui_content_type_user_profile_promotions', $e);
    }
    //cashed in promotions
    try {
      $query = new CultureFeed_Uitpas_Passholder_Query_SearchCashedInPromotionPointsOptions();
      $result = $cf->uitpas()->getCashedInPromotionPoints($query);
      if ($result->total) {
        $cashed_in_promotions = $result->objects;
      }
    }
    catch (Exception $e) {
      watchdog_exception('uitpas_ui_content_type_user_profile_promotions_cashed_in', $e);
    }
    //ctools block
    $block = new stdClass();
    $block->title = "";
    $block->content = theme('uitpas_ui_user_profile_advantages', array(
      'uitpas_user' => $uitpas_user,
      'passholder' => $passholder,
      'advantages' => $advantages,
      'promotions' => $promotions,
      'cashed_in_advantages' => $cashed_in_advantages,
      'cashed_in_promotions' => $cashed_in_promotions,
    ));
    return $block;
  }
}