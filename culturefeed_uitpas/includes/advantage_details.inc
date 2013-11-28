<?php

/**
 * @file
 * Helper functions for UiTPAS advantage details.
 */

/**
 * Returns advantage details.
 */
function culturefeed_uitpas_advantage_details_get($advantage, $slug) {

  if ($slug != culturefeed_search_slug($advantage->title)) {
    drupal_goto('advantage/' . culturefeed_search_slug($advantage->title) . '/' . $advantage->id, array(), 301);
  }

  return array(
    '#advantage' => $advantage,
    '#theme' => 'culturefeed_uitpas_advantage',
  );

}

/**
 * Returns advantage details title.
 */
function culturefeed_uitpas_advantage_details_title_get($advantage) {

  return $advantage->title;

}
