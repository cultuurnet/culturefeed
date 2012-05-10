<?php

/**
 * @file
 * Plugin to provide access control/visibility uitpas slug.
 */

$plugin = array(
  'title' => t("Uitpas: slug"),
  'description' => t('Control access by checking slug.'),
  'callback' => 'uitpas_ui_uitpas_slug_access_check',
  'required context' => array(
    new ctools_context_required(t('Uitpas: object'), array(
      'uitpas_activity', 
      'uitpas_actor', 
      'uitpas_advantage',
      'uitpas_promotion',
    )),
    new ctools_context_required(t('Uitpas: slug'), 'string'),
   ),
);

function uitpas_ui_uitpas_slug_access_check($conf, $context) {
  if (empty($context)) {
    return FALSE;
  }
  else {
    $object = $context[0]->data;
    $slug = $context[1]->data;
    $info = uitpas_ui_helpers_context_get_uitpas_object_type($context[0]->type);
    if ($info) {
      $base_path = $info['base_path'];
      switch ($info['type']) {
        case 'culturefeed':
          $title = $object->title;
          $id = $object->id;
          break;
        case 'cnapi':
          $title = (isset($object['title'])) ? $object['title'] : $object['detail']['nl']['title'];
          $id = $object['cdbid'];
          break;
      }
      //our check and a redirect if required
      if ($slug == cnapi_ui_slug($title)) {
        return TRUE;
      }
      else {
        drupal_goto($base_path . "/" . cnapi_ui_slug($title) . "/" . $id, array(), 302);
      }
    }
  }
  return FALSE;
}

/**
 * Helper function to provide slug
 */
function uitpas_ui_helpers_context_get_uitpas_object_type($context_type) {
  $types = uitpas_ui_object_types();
  foreach ($types as $info) {
    if ($info['context_type'] == $context_type) {
      return $info;
    }
  }
  return FALSE;
}

