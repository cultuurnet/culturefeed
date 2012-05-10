<?php

/**
 * @file
 * Helper functions for uitpas_ui
 */

/**
 * Helper function to link culturefeed and cnapi objects
 */
function uitpas_ui_helpers_link_object($type, $object) {
  $types = uitpas_ui_object_types();
  $info = $types[$type];
  if ($info['type'] == 'culturefeed') {
    return $info['base_path'] . "/" . cnapi_ui_slug($object->title) . "/" . $object->id;
  }
  else if ($info['type'] == 'cnapi') {
    $title = (isset($object['title'])) ? $object['title'] : $object['detail']['nl']['title'];
    return $info['base_path'] . "/" . cnapi_ui_slug($title) . "/" . $object['cdbid'];
  }
  return '<front>';
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