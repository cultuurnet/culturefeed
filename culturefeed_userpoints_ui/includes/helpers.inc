<?php
/**
 * @file
 * Helper functions to calculate and handle user points.
 */

/**
 * Refresh the promotions list to the latest status.
 * @param array $form
 *   Promotions list form.
 */
function culturefeed_userpoints_ui_promotions_list_ajax_refresh($form, $promotion_points) {

  // Update the current userpoints in all places where the points are displayed.
  $user_points = DrupalCultureFeed::getLoggedInUser()->points;
  $commands[] = ajax_command_invoke('.userpoints-points', 'html', array(
    CultureFeedUserpointsWishlist::getPointsLeft($user_points)));

  // Update the wishlist.
  $wishlist_build = culturefeed_userpoints_ui_get_wishlist($promotion_points->objects);
  $wishlist = render($wishlist_build);
  $commands[] = ajax_command_html('#culturefeed-userpoints-wishlist', $wishlist);

  // Update the list.
  $commands[] = ajax_command_replace('#culturefeed-userpoints-promotions-list', render($form['list']));

  // Add status messages.
  $commands[] = ajax_command_prepend(NULL, theme('status_messages'));

  return (array('#type' => 'ajax', '#commands' => $commands));

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

  $params = array('sort' => 'POINTS', 'order' => 'ASC');
  $pointsPromotions = DrupalCultureFeed::getActivityPointsPromotions($params);

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

/**
 * Helper function to get options for the maximum available units.
 * @param CultureFeed_PointsPromotion $pointsPromotion
 * @return array
 */
function culturefeed_userpoints_ui_get_max_available_units_options(CultureFeed_PointsPromotion $pointsPromotion) {
  $max_units = $pointsPromotion->maxAvailableUnits;
  $options = array();
  for ($i = 1; $i < $max_units; $i++) {
    $options[$i] = $i;
  }

  return $options;

}
