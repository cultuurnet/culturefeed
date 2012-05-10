<?php

/**
 * @file
 * Helper functions for uitpas_ui
 */

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
          $object->counters[$ind]->name = l($counter->name, 'actor/' . $counter->id);
        }
      }
    }
  }
}