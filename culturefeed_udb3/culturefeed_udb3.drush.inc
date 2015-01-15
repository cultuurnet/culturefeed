<?php

/**
 * @file
 * Contains culturefeed_udb3.drush.inc.
 */

use Drupal\Core\DrupalKernel;
use Drupal\Core\Site\Settings;
use Symfony\Component\HttpFoundation\Request;

/**
 * Implements hook_drush_command().
 */
function culturefeed_udb3_drush_command() {
  $items = array();

  $items['udb3-worker'] = array(
    'description' => 'The culturefeed udb3 worker.',
    'options' => array(),
    'arguments' => array(),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_SITE,
    'core' => array('8+'),
    'aliases' => array('udb3w'),
  );

  $items['udb3-delete-data'] = array(
    'description' => 'Delete all culturefeed udb3 data: document store and repository.',
    'options' => array(),
    'arguments' => array(),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_SITE,
    'core' => array('8+'),
    'aliases' => array('udb3d'),
  );

  return $items;
}

/**
 * Deletes all culturefeed udb3 data.
 */
function drush_culturefeed_udb3_udb3_delete_data() {

  // Delete the domain message implementations.
  $entities = array(
    'event_document_repository',
    'event_store',
    'organizer_document_repository',
    'organizer_store',
    'place_document_repository',
    'place_store',
    'used_keywords_memory_event_store',
    'event_relations',
  );

  foreach ($entities as $entity_type) {
    $results = entity_load_multiple($entity_type);
    $entity_ids = array_keys($results);
    entity_delete_multiple($entity_type, $entity_ids);
  }

}

/**
 * Starts the culturefeed udb3 worker.
 */
function drush_culturefeed_udb3_udb3_worker() {

  $path = drupal_get_path('module', 'culturefeed_udb3');
  $command = $path . '/culturefeed_udb3.worker.php';
  $command .= " --drupal_root=" . DRUPAL_ROOT;
  drush_shell_proc_open($command);

}
