<?php
/**
 * @file
 * Helper functions to calculate and handle user points.
 */

/**
 * Helper function to render a list of promotions.
 * @param array $promotions
 * @param double $points 
 *   The points of the user in respect to the wishlist.
 */
function culturefeed_userpoints_ui_promotions_list($promotions) {

  if ($promotions->total < 0) {
    return array();
  }
  
  $objects = array();
  foreach ($promotions->objects as $object) {
    $objects[] = $object;
  }
  
  return array(
    '#theme' => 'culturefeed_userpoints_ui_promotions_list',
    '#promotions' => $objects,
    '#total' => $promotions->total,
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'culturefeed_userpoints_ui') . '/js/promotions.js',
      ),
    )
  );
  
}

/**
 * Helper function to render the wishlist of selected promotions.
 * @param array $promotions
 */
function culturefeed_userpoints_ui_get_wishlist($promotions) {

  $selected_promotions = CultureFeedUserpointsWishlist::getItems($promotions);

  $build = array(
    '#theme' => 'culturefeed_userpoints_ui_wishlist',
    '#promotions' => $selected_promotions,
  );
  
  return $build;
  
}

/**
 * Utility function + facade method to ease the request.
 * @return array of PointsPromotions.
 */
function culturefeed_userpoints_ui_get_pointspromotions() {

  $pointsPromotions = DrupalCultureFeed::getActivityPointsPromotions();
  
  $list = array();
  
  foreach ($pointsPromotions->objects as $pointsPromotion) {
    
    // Every page load we need to filter some items out of the list. Note that
    // this list does not follow the state of the promotion wishlist. 
    if ($pointsPromotion->cashInState != CultureFeed_PointsPromotion::NOT_POSSIBLE_POINTS_CONSTRAINT
      && $pointsPromotion->cashInState != CultureFeed_PointsPromotion::NOT_POSSIBLE_USER_VOLUME_CONSTRAINT
      && ($pointsPromotion->unitsTaken < $pointsPromotion->maxAvailableUnits 
        || empty($pointsPromotion->maxAvailableUnits))) {
      
      $list[] = $pointsPromotion;
      
    }
  }
  $pointsPromotions->objects = $list;
  
  return $pointsPromotions;
  
}
