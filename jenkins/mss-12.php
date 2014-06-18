<?php

$default_theme = 'bartik';;

$facets = array(
  'datetype', 'eventtype', 'theme', /*'city', 'city-only',*/
  'actortype', 'educationfield', 'educationlevel', 'facility',
  'flandersregion', 'flanderstouristregion', 'IPE', 'misc',
  'municipal', 'publicscope', 'targetaudience', 'umv', 'workingregion',
);

$default_block = array(
  'module' => 'culturefeed_search_ui',
  'delta' => '',
  'theme' => '',
  'status' => 1,
  'weight' => '',
  'region' => 'sidebar_first',
  'pages' => 'agenda/search',
  'visibility' => 1,
  'cache' => -1,
);

$delete = db_delete('block');
$delete->condition('module', 'culturefeed_search_ui');
$or = db_or();
$or->condition('theme', 'culturefeed_bootstrap');
$or->condition('theme', 'bartik');
$delete->condition($or);
$delete->condition('delta', 'facet-%', 'LIKE');
$delete->execute();

$insert = db_insert('block')->fields(array('module', 'delta', 'theme', 'status', 'weight', 'region', 'pages', 'cache', 'visibility'));
$weight = -50;
foreach ($facets as $facet) {
  $block = $default_block;
  $block['delta'] = 'facet-' . $facet;
  $block['weight'] = $weight;
  $weight++;
  // Add block for bootstrap.
  $block['theme'] = 'culturefeed_bootstrap';
  $insert->values($block);
  // Add block for bartik.
  $block['theme'] = 'bartik';
  $insert->values($block);
}
$insert->execute();
