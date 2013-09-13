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
  $commands[] = ajax_command_invoke('.userpoints-points-left', 'html', array(
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
 * @param $params
 *   Array of options the remote service call supports.
 *   see
 * @return array of PointsPromotions.
 */
function culturefeed_userpoints_ui_get_pointspromotions($params = array()) {

  $default_params = array('sort' => 'POINTS', 'order' => 'ASC');
  $params += $default_params;

  // Get the available promotions for the current user from the API.
  $pointsPromotions = DrupalCultureFeed::getActivityPointsPromotions($params);

  $list = array();

  // Filter out the natural constraints.
  // Also adjust properties according to the ones currently in wishlist.
  if (!empty($pointsPromotions->objects)) {
    foreach ($pointsPromotions->objects as $pointsPromotion) {

      // Filter out the natural constraints.
      if ($pointsPromotion->cashInState == CultureFeed_PointsPromotion::NOT_POSSIBLE_POINTS_CONSTRAINT
        || $pointsPromotion->cashInState == CultureFeed_PointsPromotion::NOT_POSSIBLE_USER_VOLUME_CONSTRAINT) {
        continue;
      }

      // Filter out when the units taken exceed the maximum available units.
      if (!empty($pointsPromotion->maxAvailableUnits) && $pointsPromotion->unitsTaken >= $pointsPromotion->maxAvailableUnits) {
        continue;
      }

      // Update the item according to the one in the wishlist.
      $wishlistItem = CultureFeedUserpointsWishlist::get($pointsPromotion->id);
      if (!empty($wishlistItem)) {
        $pointsPromotion->maxAvailableUnits = $pointsPromotion->maxAvailableUnits - $wishlistItem['count'];
      }

      $list[] = $pointsPromotion;

    }
  }
  $pointsPromotions->objects = $list;

  return $pointsPromotions;

}

/**
 * Helper function to get options for the maximum available units.
 * @param Integer $maxAvailableUnits
 * @return array
 */
function culturefeed_userpoints_ui_get_max_available_units_options($maxAvailableUnits) {

  $options = array();
  for ($i = 1; $i <= $maxAvailableUnits; $i++) {
    $options[$i] = $i;
  }

  return $options;

}
