<?php

/**
 * @file
 * Plugin to provide a content type for uitpas activity.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Uitpas: activity'),
  'description' => t('Uitpas: activity.'),
  'category' => t('Uitpas'),
  'required context' => array(
    new ctools_context_required(t('Uitpas activity'), 'string'),
    new ctools_context_required(t('Uitpas user'), 'uitpas_user'),
  ),
);

function uitpas_ui_uitpas_activity_content_type_edit_form($form, &$form_state) {
  return $form;
}

function uitpas_ui_uitpas_activity_content_type_render($subtype, $conf, $args, $context) {
  $cdbid = $context[0]->data;
  $activity = cnapi_get_event($cdbid);
  $uitpas_activity = '';
  $promotions = 0;
  try {
    $cf = DrupalCultureFeed::getConsumerInstance();
    //uitpas event
    $query = new CultureFeed_Uitpas_Event_Query_SearchEventsOptions();
    $query->cdbid = $cdbid;
    if ($context[1]->data->passholder) {
      $query->uitpasNumber = ($context[1]->data->passholder->uitpasNumber);
    }
    $result = $cf->uitpas()->searchEvents($query);
    if ($result->total) {
      $uitpas_activity = $result->objects[0];
    }
    //promotions
    try {
      $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
      $query->balieConsumerKey = $activity['location']['actor']['cdbid'];
      $result = $cf->uitpas()->getPromotionPoints($query);
      if ($result->total) {
        $promotions = $result->total;
      }
    }
    catch (Exception $e) {
      watchdog_exception('uitpas_ui_content_type_activity_actor_' . $activity['location']['actor']['cdbid'], $e);
    }
    
  }
  catch (Exception $e) {
    watchdog_exception('uitpas_ui_content_type_activity', $e);
  }
  $block = new stdClass();
  $block->title = $activity['detail']['nl']['title'];
  $block->content = theme('uitpas_ui_activity', array(
    'activity' => $activity, 
    'uitpas_activity' => $uitpas_activity, 
    'promotions' => $promotions,
  ));
  return $block;
}