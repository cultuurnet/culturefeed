<?php
/**
 * @file
 * Helper functions to calculate and handle user points.
 */

/**
 * Helper to create a list / form of all available points promotions.
 * 
 * @param array $form
 * @param array $form_state
 * @param array $promotions
 * @return array
 */
function culturefeed_userpoints_ui_promotions_list_form($form, $form_state, $promotions) {

  if ($promotions->total < 0) {
    return array();
  }
  
  $objects = array();
  foreach ($promotions->objects as $object) {
    $objects[] = $object;
  }
  
  $form['list'] = array(
    '#theme' => 'culturefeed_userpoints_ui_promotions_list',
    '#promotions' => $objects,
    '#total' => $promotions->total,
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'culturefeed_userpoints_ui') . '/js/promotions.js',
      ),
    ),
    '#prefix' => '<div id="culturefeed-userpoints-promotions-list">',
    '#suffix' => '</div>',
  );
  
  foreach ($promotions->objects as $promotion) {

    $form['list'][$promotion->id] = array(
      '#theme' => 'culturefeed_userpoints_ui_promotions_list_item',
      '#promotion' => $promotion,
      '#tree' => TRUE,
    );
    
    $options = culturefeed_userpoints_ui_get_max_available_units_options($promotion);
    $form['list'][$promotion->id]['select_id'] = array(
      '#name' => 'select_id_' . $promotion->id,
      '#type' => 'hidden',
      '#default_value' => $promotion->id,
    );
    
    $form['list'][$promotion->id]['select_count'] = array(
      '#name' => 'select_count_' . $promotion->id,
      '#type' => 'select',
      '#title' => '',
      '#default_value' => 1,
      '#options' => $options,
    );
    
    $form['list'][$promotion->id]['select_btn'] = array(
      '#name' => 'select_btn_' . $promotion->id,
      '#type' => 'button',
      '#value' => 'Selecteer',
      '#ajax' => array(
        'callback' => 'culturefeed_userpoints_ui_promotions_list_form_callback',
        'wrapper' => 'culturefeed-userpoints-promotions-list',
      ),
    );
    
  }
  
  return $form;
  
}

/**
 * Submit handler to add/remove points promotions.
 * 
 * @param array $form
 * @param array $form_state
 */
function culturefeed_userpoints_ui_promotions_list_form_submit($form, &$form_state) {

  $selected_button = $form_state['clicked_button']['#name'];
  $selected_id = str_replace("select_btn_", "", $selected_button);
  $selected_count = $form_state['values'][$selected_id]['select_count'];
  
  culturefeed_userpoints_ui_promotion_select_page($selected_id, $selected_count, 'nojs', 'add');
}

/**
 * Ajax Callback handler to add/remove points promotions.
 * 
 * @param array $form
 * @param array $form_state
 */
function culturefeed_userpoints_ui_promotions_list_form_callback($form, &$form_state) {

  $selected_button = $form_state['clicked_button']['#name'];
  $selected_id = str_replace("select_btn_", "", $selected_button);
  $selected_count = $form_state['values'][$selected_id]['select_count'];

  culturefeed_userpoints_ui_promotion_select_page($selected_id, $selected_count, 'ajax', 'add');
  
  return $form;
}

/**
 * Page callback to select a promotion to exchange for userpoints.
 */
function culturefeed_userpoints_ui_promotion_select_page($promotion_id, $promotion_count = 1, $request_type = 'ajax', $operation = 'add') {

  // We need this to be able to retrieve 1 item.
  // @todo: remove/refactor this if the api method exists.
  $pointsPromotions = culturefeed_userpoints_ui_get_pointspromotions();
  // Change the selection in the cart.
  if ($operation == 'add') {
    $promotion = NULL;
    foreach ($pointsPromotions->objects as $object) {
      if ($object->id == $promotion_id) {
        $promotion = $object;
      }
    }
    $points_diff = CultureFeedUserpointsWishlist::add($promotion_id, $promotion);
    $message = 'Het voordeel werd toegevoegd aan uw promotie mandje.';
  }
  elseif ($operation == 'remove') {
    $points_diff = CultureFeedUserpointsWishlist::remove($promotion_id);
    $message = 'Het voordeel werd verwijderd uit uw promotie mandje.';
  }

  if ($request_type != "ajax") {
    
    drupal_set_message($message);
    drupal_goto();
    
  }
  else {

    // Update the current userpoints in all places where the points are displayed.
    $user_points = DrupalCultureFeed::getLoggedInUser()->points;
    $commands[] = ajax_command_invoke('.userpoints-points', 'html', array(
      CultureFeedUserpointsWishlist::getPointsLeft($user_points)));

    // Update the wishlist.
    $wishlist_build = culturefeed_userpoints_ui_get_wishlist($pointsPromotions->objects);
    $wishlist = render($wishlist_build);
    $commands[] = ajax_command_html('#culturefeed-userpoints-wishlist', $wishlist);
    
    // Update the list.
    //$list_build = culturefeed_userpoints_ui_promotions_list($pointsPromotions);
    $build = drupal_get_form('culturefeed_userpoints_ui_promotions_list_form', $pointsPromotions);
    $list = render($build);
    $commands[] = ajax_command_html('#culturefeed-userpoints-promotions-list', $list);
    //$commands[] = ajax_command_replace(NULL, $list);
    
    ajax_deliver(array('#type' => 'ajax', '#commands' => $commands));
    exit();
    
  }
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
