<?php

/**
 * @file
 * Plugin to provide a content type for uitpas advantage.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Uitpas: advantage'),
  'description' => t('Uitpas: advantage.'),
  'category' => t('Uitpas'),
  'required context' => new ctools_context_required(t('Uitpas advantage'), 'string'),
);

function uitpas_ui_uitpas_advantage_content_type_edit_form($form, &$form_state) {
  return $form;
}

function uitpas_ui_uitpas_advantage_content_type_render($subtype, $conf, $args, $context) {
  $aid = $context->data;
  if ($aid) {
    try {
      $cf = DrupalCultureFeed::getConsumerInstance();
      $advantage = $cf->uitpas()->getWelcomeAdvantage($aid);
      foreach ($advantage->counters as $key => $counter) {
        //@TODO: this won't work, we need a cbid
        $actor = cnapi_get_actor($counter->id);
        if ($actor) {
          $advantage->counters[$key]->name = l($counter->name, 'actor/' . $counter->id);
        }
      }
      $block = new stdClass();
      $block->title = "";
      $block->content = theme('uitpas_ui_advantage', array('advantage' => $advantage));
      return $block;
    }
    catch (Exception $e) {
      watchdog_exception('uitpas_ui_content_type_advantage', $e);
    }
  }
}