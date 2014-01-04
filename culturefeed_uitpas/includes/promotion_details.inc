<?php

/**
 * @file
 * Helper functions for UiTPAS promotion details.
 */

/**
 * Returns promotion details.
 */
function culturefeed_uitpas_promotion_details_get($promotion, $slug) {

  if ($slug != culturefeed_search_slug($promotion->title)) {
    drupal_goto('promotion/' . culturefeed_search_slug($promotion->title) . '/' . $promotion->id, array(), 301);
  }

  return array(
    '#promotion' => $promotion,
    '#theme' => 'culturefeed_uitpas_promotion',
  );

}

/**
 * Returns promotion details title.
 */
function culturefeed_uitpas_promotion_details_title_get($promotion) {

  return $promotion->title;

}
