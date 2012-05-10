<?php

/**
 * @file
 * Plugin to provide a content type for uitpas actor.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Uitpas: actor'),
  'description' => t('Uitpas: actor.'),
  'category' => t('Uitpas'),
  'required context' => array(
    new ctools_context_required(t('Uitpas actor'), 'uitpas_actor'),
    new ctools_context_required(t('Uitpas user'), 'uitpas_user'),
  ),
);

function uitpas_ui_uitpas_actor_content_type_edit_form($form, &$form_state) {
  return $form;
}

function uitpas_ui_uitpas_actor_content_type_render($subtype, $conf, $args, $context) {
  $actor = $context[0]->data;
  $points = FALSE;
  $promotions = array();
  try {
    $cf = DrupalCultureFeed::getConsumerInstance();
    //event
    $query = new CultureFeed_Uitpas_Event_Query_SearchEventsOptions();
    $query->locatieId = $actor['cdbid'];
    if ($context[1]->data && $context[1]->data->passholder) {
      $query->uitpasNumber = ($context[1]->data->passholder->uitpasNumber);
    }
    $result = $cf->uitpas()->searchEvents($query);
    if ($result->total) {
      foreach ($result->objects as $event) {
        if ($event->numberOfPoints) {
          $points = TRUE;
          break;
        }
      }
    }
    //promotions
    try {
      $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
      $query->balieConsumerKey = $actor['cdbid'];
      $result = $cf->uitpas()->getPromotionPoints($query);
      if ($result->total) {
        $promotions = $result->objects;
      }
    }
    catch (Exception $e) {
      watchdog_exception('uitpas_ui_content_type_actor_' . $actor['cdbid'], $e);
    }
  }
  catch (Exception $e) {
    watchdog_exception('uitpas_ui_content_type_actor', $e);
  }
  $block = new stdClass();
  $block->title = $actor['detail']['nl']['title'];
  $block->content = theme('uitpas_ui_actor', array(
    'actor' => $actor, 
    'points' => $points,
    'promotions' => $promotions,
  ));
  return $block;
}