<?php

/**
 * @file
 * Helper functions for uitpas_ui
 */

/**
 * Helper function to link culturefeed and cnapi objects
 */
function uitpas_ui_helpers_link_object($type, $object) {
  $link = uitpas_ui_helpers_object_info($type, $object, 'link');
  return ($link) ? $link : '<front>';
}

/**
 * Helper function to get properties from culturefeed and cnapi objects
 */
function uitpas_ui_helpers_object_info($type, $object, $property = NULL) {
  $types = uitpas_ui_object_types();
  $info = $types[$type];
  if ($info) {
    $id = '';
    $title = '';
    switch ($info['object']) {
      case 'culturefeed':
        $id = $object->id;
        $title = $object->title;
        break;
      case 'cnapi':
        $id = $object['cdbid'];
        $title = (isset($object['title'])) ? $object['title'] : $object['detail']['nl']['title'];
        break;
    }
    if ($id && $title) {
      $properties = array(
        'id' => $id,
        'title' => $title,
        'link' => $info['base_path'] . "/" . cnapi_ui_slug($title) . "/" . $id,
      );
      return (!is_null($property) && $properties[$property]) ? $properties[$property] : $properties;
    }
  }
  return FALSE;
}

/**
 * Helper function to get properties from culturefeed and cnapi objects by their corresponding context type
 */
function uitpas_ui_helpers_object_info_by_context_type($context_type, $object, $property = NULL) {
  $types = uitpas_ui_object_types();
  foreach ($types as $key => $info) {
    if ($info['context_type'] == $context_type) {
      return uitpas_ui_helpers_object_info($key, $object, $property);
      break;
    }
  }
  return FALSE;
}

/**
 * Helper function to link culturefeed counters to cnapi actors if available
 */
function uitpas_ui_helpers_process_counters(&$objects) {
  foreach ($objects as $object) {
    if (isset($object->counters)) {
      foreach ($object->counters as $ind => $counter) {
        $id = $counter->id;
        $actor = cnapi_get_actor($id);
        if ($actor) {
          $object->counters[$ind]->name = l($counter->name, uitpas_ui_helpers_link_object('actor', $actor));
        }
      }
    }
  }
}