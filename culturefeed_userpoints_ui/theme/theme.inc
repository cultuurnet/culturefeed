<?php
/**
 * @file
 * Theming / preprocess functions for culturefeed userpoints ui.
 */

/**
 * Preprocess the culturefeed userpoints my balance block.
 * 
 * @see culturefeed-userpoints-ui-my-balance.tpl.php
 */
function culturefeed_userpoints_ui_preprocess_culturefeed_userpoints_ui_my_balance(&$variables) {

  _culturefeed_userpoints_ui_preprocess_userpoints($variables);
  
}

/**
 * Preprocess the culturefeed userpoints my exchange page.
 * 
 * @see culturefeed-userpoints-ui-exchange-page.tpl.php
 */
function culturefeed_userpoints_ui_preprocess_culturefeed_userpoints_ui_exchange_page(&$variables) {

  _culturefeed_userpoints_ui_preprocess_userpoints($variables);

  //$build = culturefeed_userpoints_ui_promotions_list($variables['promotions']);
  $build = drupal_get_form('culturefeed_userpoints_ui_promotions_list_form', $variables['promotions']);
  $list = drupal_render($build);
  
  $minimum_points = NULL;
  foreach ($variables['promotions']->objects as $promotion) {
    if (!isset($minimum_points) || $promotion->points < $minimum_points) {
      $minimum_points = $promotion->points;
    }
  }
  $variables['minimum_points'] = $minimum_points;
  
  $variables['promotions_list'] = $list;
  $variables['wishlist'] = '<div id="culturefeed-userpoints-wishlist">'; 
  $wishlist_build = culturefeed_userpoints_ui_get_wishlist($variables['promotions']->objects);
  $variables['wishlist'] .= drupal_render($wishlist_build);
  $variables['wishlist'] .= '</div>';
  
}

/**
 * Helper function to set the variables for the points, with and with wishlist items.
 */
function _culturefeed_userpoints_ui_preprocess_userpoints(&$variables) {

  $cf_user = DrupalCultureFeed::getLoggedInUser();
  $points_left = CultureFeedUserpointsWishlist::getPointsLeft($cf_user->points);
  
  $variables['real_points'] = $variables['points'];
  $variables['real_points_in_wishlist'] = $variables['points_in_wishlist'];
  $variables['real_points_left'] = $points_left;
  
}

/**
 * Preprocess function for the promotions list.
 * 
 * @see culturefeed-userpoints-ui-promotions-list.tpl.php
 */
function culturefeed_userpoints_ui_preprocess_culturefeed_userpoints_ui_promotions_list(&$variables) {

  // Always include the javascript for ajax behavior.
  drupal_add_library('system', 'drupal.ajax');

  $form = $variables['form'];
  $promotions = $form['#promotions'];
  
  $list = array();
  foreach (element_children($form) as $form_key) {
    $list[] = drupal_render($form[$form_key]);
  }
  
  $variables['items'] = $list;


}

/**
 * Preprocess a promotions list item.
 * 
 * @see culturefeed-userpoints-ui-promotions-list-item.tpl.php
 */
function culturefeed_userpoints_ui_preprocess_culturefeed_userpoints_ui_promotions_list_item(&$variables) {

  //$pointsPromotion = $variables['promotion'];
  $form = $variables['form'];
  $pointsPromotion = $variables['form']['#promotion'];
  
  _culturefeed_userpoints_ui_preprocess_detail($variables, $pointsPromotion);
  
  $promotion_id = $pointsPromotion->id;
  
  $variables['link'] = l($pointsPromotion->title, 'culturefeed/promotion/' . $promotion_id);
  
  $variables['select_link'] = '';
  $variables['add_link'] = '';
  $variables['remove_link'] = '';
  $variables['select_list'] = '';
  $variables['select_btn'] = '';
  $variables['select_id'] = drupal_render($form['select_id_' . $promotion_id]);
  
  /*dsm('Volume constraint: ' . $variables['volume_constraint'] . '(' . $pointsPromotion->unitsTaken . '/' . $pointsPromotion->maxAvailableUnits . ')
     Can exchange: ' . $variables['can_exchange'] . '
     Active: ' . $variables['active'], $promotion_id);*/
  
  if ($variables['can_exchange']) {
    if (!$variables['active']) {
      $variables['select_list'] = drupal_render($form['select_count_' . $promotion_id]);
      $variables['select_btn'] = drupal_render($form['select_btn_' . $promotion_id]);
    }
    else {
      $variables['classes_array'][] = 'item-selected';
      $variables['remove_link'] = l('x Verwijder', 'culturefeed/userpoints/' . $promotion_id . '/remove/nojs',
        array('attributes' => array('class' => array('use-ajax')), 'query' => drupal_get_destination()));
    }
  }
}

/**
 * Preprocess a wishlist.
 *
 * @see culturefeed-userpoints-ui-wishlist.tpl.php
 */
function culturefeed_userpoints_ui_preprocess_culturefeed_userpoints_ui_wishlist(&$variables) {

  $items = array();
  foreach ($variables['promotions'] as $promotion) {
    $items[] = theme('culturefeed_userpoints_ui_wishlist_item', array(
      'item' => $promotion,
    ));
  }
  
  $variables['items'] = $items;
  
}

/**
 * Preprocess a wishlist item.
 *
 * @see culturefeed-userpoints-ui-wishlist-item.tpl.php
 */
function culturefeed_userpoints_ui_preprocess_culturefeed_userpoints_ui_wishlist_item(&$variables) {

  $variables['title'] = $variables['item']['title'];
  $variables['count'] = $variables['item']['count'];
  $variables['id'] = $variables['item']['promotion_id'];
  $variables['remove_link'] = l('x Verwijder', 'culturefeed/userpoints/' . $variables['item']['promotion_id'] . '/remove/nojs', array('attributes' => array('class' => array('use-ajax'))));

}

/**
 * Theme the message when a user earned some points.
 */
function theme_culturefeed_userpoints_ui_points_earned_message($variables) {

  $points = CultureFeedUserpointsWishlist::getRealPoints($variables['activity']->points);
  $total_points = CultureFeedUserpointsWishlist::getRealPoints($variables['activity']->newTotalPoints);
  $message = 'Gefeliciteerd! Je verdiende ' . $points . ' met deze actie en hebt nu ' . $total_points . ' punten.';

  return $message;

}

/**
 * Theme function for a message when the userpoints were exchanged.
 * @param array $variables
 */
function theme_culturefeed_userpoints_ui_points_exchanged_message($variables) {
  // $variables['promotions']
  return "Bedankt, uw punten zijn ingewisseld. U kunt voordelen afhalen.";
}

/**
 * Preprocess a promotion detail.
 *
 * @see culturefeed-userpoints-ui-detail-page.tpl.php
 */
function culturefeed_userpoints_ui_preprocess_culturefeed_userpoints_ui_detail_page(&$variables) {

  $pointsPromotion = $variables['promotion'];
  
  _culturefeed_userpoints_ui_preprocess_detail($variables, $pointsPromotion);

  $variables['exchange_url'] = url('culturefeed/userpoints/exchange');
  $variables['exchange_link'] = l('Voordeel bestellen', 'culturefeed/userpoints/exchange');
  
}

/**
 * Helper preprocessor for one pointsPromotion.
 */
function _culturefeed_userpoints_ui_preprocess_detail(&$variables, $pointsPromotion) {

  $promotion_id = $pointsPromotion->id;
  
  // Calculate constraints. Eg. User volume constraint, volume contraint, ... .
  $promotion_count = 0;
  if (CultureFeedUserpointsWishlist::has($promotion_id)) {
    $data = CultureFeedUserpointsWishlist::get($promotion_id);
    $promotion_count = $data['count'];
  }
  $variables['volume_constraint'] = FALSE;
  $variables['unitsTaken'] = $pointsPromotion->unitsTaken;
  $units_taken = $pointsPromotion->unitsTaken + $promotion_count;
  $variables['currentUnitsTaken'] = $units_taken;
  if (!empty($pointsPromotion->maxAvailableUnits) &&
    $units_taken >= $pointsPromotion->maxAvailableUnits) {
    $variables['volume_constraint'] = TRUE;
  }
  $variables['unitsLeft'] = $pointsPromotion->maxAvailableUnits - $units_taken;
  
  // Calculate the points.
  $cf_account = DrupalCultureFeed::getLoggedInUser();
  $user_points = CultureFeedUserpointsWishlist::getPointsLeft($cf_account->points);
  $variables['points'] = $pointsPromotion->points;
  $variables['real_points'] = CultureFeedUserpointsWishlist::getRealPoints($pointsPromotion->points);
  $variables['active'] = CultureFeedUserpointsWishlist::has($promotion_id);
  $variables['can_exchange'] = $variables['active'] || $user_points >= $variables['real_points'];

  $variables['title'] = $pointsPromotion->title;
  $variables['description1'] = $pointsPromotion->description1;
  $variables['description2'] = $pointsPromotion->description2;
  $variables['picture_url'] = '';
  if (!empty($pointsPromotion->pictures)) {
    $variables['picture_url'] = reset($pointsPromotion->pictures);
  }
  $variables['id'] = $promotion_id;
  $variables['cashInState'] = $pointsPromotion->cashInState;
  $variables['cashedIn'] = $pointsPromotion->cashedIn;
  $variables['cashingPeriodBegin'] = date('d/m/Y', $pointsPromotion->cashingPeriodBegin);
  if (!empty($pointsPromotion->cashingPeriodEnd)) {
    $variables['cashingPeriodEnd'] = date('d/m/Y', $pointsPromotion->cashingPeriodEnd);
  }
  $variables['creationDate'] = date('d/m/Y', $pointsPromotion->creationDate);
  $variables['maxAvailableUnits'] = $pointsPromotion->maxAvailableUnits;
  $variables['inSpotlight'] = $pointsPromotion->inSpotlight;
  
  $variables['period_volume'] = '';
  $variables['period_type'] = '';
  if (!empty($pointsPromotion->periodConstraint)) {
    $variables['period_volume'] = $pointsPromotion->periodConstraint->periodVolume;
    $variables['period_type'] = $pointsPromotion->periodConstraint->periodType;
  }

}
