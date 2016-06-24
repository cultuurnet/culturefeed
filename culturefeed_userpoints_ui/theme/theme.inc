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
 * Preprocess a more promotions block.
 *
 * @see culturefeed-userpoints-ui-block-more-promotions.tpl.php
 */
function culturefeed_userpoints_ui_preprocess_culturefeed_userpoints_ui_block_more_promotions(&$variables) {

  $variables['prev_image_url'] = '';
  $variables['next_image_url'] = '';

  if (isset($variables['previous_item'])) {
    if (!empty($variables['previous_item']->pictures)) {
      $variables['prev_image_url'] = $variables['previous_item']->pictures[0];
    }
    $variables['prev_item_url'] = url('userpoints/promotion/' . $variables['previous_item']->id);
    $variables['prev_item_title'] = $variables['previous_item']->title;
  }

  if (isset($variables['next_item'])) {
    if (!empty($variables['next_item']->pictures)) {
      $variables['next_image_url'] = $variables['next_item']->pictures[0];
    }
    $variables['next_item_url'] = url('userpoints/promotion/' . $variables['next_item']->id);
    $variables['next_item_title'] = $variables['next_item']->title;
  }

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
  $variables['minimum_points'] = CultureFeedUserpointsWishlist::getRealPoints($minimum_points);

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

  $variables['real_points'] = CultureFeedUserpointsWishlist::getRealPoints($cf_user->points);
  $variables['real_points_in_wishlist'] = CultureFeedUserpointsWishlist::getPointsInWishlist();
  $variables['real_points_left'] = CultureFeedUserpointsWishlist::getPointsLeft($cf_user->points);

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

  $variables['link'] = l($pointsPromotion->title, 'userpoints/promotion/' . $promotion_id);
  $variables['item_url'] = url('userpoints/promotion/' . $pointsPromotion->id);
  $variables['add_link'] = '';
  $variables['remove_link'] = '';
  $variables['select_list'] = '';
  $variables['select_btn'] = '';
  $variables['select_id'] = drupal_render($form['select_id']);

  // Make the width 100% so it will fit the container.
  $form['select_count']['#attributes']['class'][] = 'span12';
  $form['select_btn']['#attributes']['class'][] = 'btn-primary';

  if ($variables['can_exchange']) {
    $variables['select_list'] = drupal_render($form['select_count']);
    $variables['select_btn'] = drupal_render($form['select_btn']);
    if (!$variables['active']) {
    }
    else {
      $variables['classes_array'][] = 'item-selected';
      $variables['remove_link'] = l(t('Delete'), 'culturefeed/userpoints/' . $promotion_id . '/remove/nojs',
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

  $variables['title'] = check_plain($variables['item']['title']);
  $variables['count'] = $variables['item']['count'];
  $variables['id'] = $variables['item']['promotion_id'];
  $variables['points'] = CultureFeedUserpointsWishlist::getRealPoints($variables['item']['points']);
  $variables['remove_link'] = l(t('Delete'), 'culturefeed/userpoints/' . $variables['item']['promotion_id'] . '/remove/nojs', array('attributes' => array('class' => array('use-ajax'))));

}

/**
 * Preprocess a promotion detail.
 *
 * @see culturefeed-userpoints-ui-detail-page.tpl.php
 */
function culturefeed_userpoints_ui_preprocess_culturefeed_userpoints_ui_detail_page(&$variables) {

  $pointsPromotion = $variables['promotion'];

  _culturefeed_userpoints_ui_preprocess_detail($variables, $pointsPromotion);

  $variables['exchange_url'] = url('userpoints/promotions/exchange');
  $variables['exchange_link'] = l(t('Order promotion'), 'userpoints/promotions/exchange', array('fragment' => 'culturefeed-userpoints-notifications'));

}

/**
 * Helper preprocessor for one pointsPromotion.
 */
function _culturefeed_userpoints_ui_preprocess_detail(&$variables, $pointsPromotion) {

  $promotion_id = $pointsPromotion->id;

  $variables['unitsTaken'] = $pointsPromotion->unitsTaken;
  $variables['unlimited'] = empty($pointsPromotion->maxAvailableUnits);
  $variables['unitsLeft'] = -1;
  if (!$variables['unlimited']) {
    $variables['maxAvailableUnits'] = $pointsPromotion->maxAvailableUnits;
    $variables['unitsLeft'] = $pointsPromotion->maxAvailableUnits - $pointsPromotion->unitsTaken;
  }

  $variables['active'] = FALSE;
  $variables['can_exchange'] = FALSE;
  $variables['real_points'] = CultureFeedUserpointsWishlist::getRealPoints($pointsPromotion->points);
  // Calculate the points.
  if (DrupalCultureFeed::isCultureFeedUser()) {
    $cf_account = DrupalCultureFeed::getLoggedInUser();
    $user_points = CultureFeedUserpointsWishlist::getPointsLeft($cf_account->points);
    $variables['points'] = $pointsPromotion->points;
    $variables['active'] = CultureFeedUserpointsWishlist::has($promotion_id);
    $variables['can_exchange'] = $variables['active'] || $user_points >= $variables['real_points'];
  }

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
  $variables['cashingPeriodEnd'] = '';
  if (!empty($pointsPromotion->cashingPeriodEnd)) {
    $variables['cashingPeriodEnd'] = date('d/m/Y', $pointsPromotion->cashingPeriodEnd);
  }
  $variables['creationDate'] = date('d/m/Y', $pointsPromotion->creationDate);
  $variables['inSpotlight'] = $pointsPromotion->inSpotlight;

  $variables['period_volume'] = '';
  $variables['period_type'] = '';
  if (!empty($pointsPromotion->periodConstraint)) {
    $variables['period_volume'] = $pointsPromotion->periodConstraint->periodVolume;
    $variables['period_type'] = $pointsPromotion->periodConstraint->periodType;
  }

}

/**
 * Preprocess a promotion list.
 *
 * @see culturefeed-userpoints-ui-promotions-list.tpl.php
 */
function culturefeed_userpoints_ui_preprocess_culturefeed_userpoints_ui_promotions_page(&$variables) {

  $items = array();
  foreach ($variables['promotions']->objects as $promotion) {
    $items[] = theme('culturefeed_userpoints_ui_promotions_page_item', array(
      'promotion' => $promotion,
    ));
  }

  $variables['items'] = $items;

}

/**
 * Preprocess a promotion item.
 *
 * @see culturefeed-userpoints-ui-promotions-item.tpl.php
 */
function culturefeed_userpoints_ui_preprocess_culturefeed_userpoints_ui_promotions_page_item(&$variables) {
  $pointsPromotion = $variables['promotion'];

  _culturefeed_userpoints_ui_preprocess_detail($variables, $pointsPromotion);

  $variables['item_url'] = url('userpoints/promotion/' . $pointsPromotion->id);
}

/**
 * Preprocess promotion exchange form.
 *
 * @see culturefeed-userpoints-ui-exchange-form.tpl.php
 */
function culturefeed_userpoints_ui_preprocess_culturefeed_userpoints_ui_exchange_form(&$variables) {

  // Email
  $variables['form']['mbox']['#attributes']['class'][] = 'span12';

  // Last name
  $variables['form']['familyName']['#attributes']['class'][] = 'span12';

  // First name
  $variables['form']['givenName']['#attributes']['class'][] = 'span12';

  // Street
  $variables['form']['street']['#attributes']['class'][] = 'span12';

  // Zip
  $variables['form']['zip']['#attributes']['class'][] = 'span12';

  // City
  $variables['form']['city']['#attributes']['class'][] = 'span12';

  // Submit button
  $variables['form']['submit']['#attributes']['class'][] = 'btn-primary';

}


/**
 * Preprocess the userpoints earned overview.
 * @see culturefeed-userpoints-earned-overview.tpl.php
 */
function culturefeed_preprocess_culturefeed_userpoints_earned_overview(&$variables) {

  $variables['intro'] = check_plain(variable_get('culturefeed_userpoints_ui_points_overview_intro'));
  $variables['total_points'] = 0;
  $variables['exchange_link'] = l(t('Exchange'), 'userpoints/promotions/exchange', array('attributes' => array('class' => array('btn btn-large btn-warning'))));

  try {
    $cf_user = DrupalCultureFeed::getLoggedInUser();
    $variables['total_points'] = CultureFeedUserpointsWishlist::getRealPoints($cf_user->points);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_userpoints_ui', $e);
  }

  $variables['items'] = array();
  foreach ($variables['activities'] as $activity) {

    $item = array();
    $item['date'] = format_date($activity->creationDate, 'custom', 'd/m/Y');
    $item['points'] = format_plural(CultureFeedUserpointsWishlist::getRealPoints($activity->points), '@count punt', '@count punten');
    $item['created_via'] =  $activity->createdVia;

    $config = culturefeed_social_activity_get_config($activity->type);
    $description = '';
    if (!empty($config->pointsOverviewPrefix)) {
      $description = $config->pointsOverviewPrefix . ' ';
    }

    $description .= $activity->nodeTitle;
    if (!empty($config->pointsOverviewSuffix)) {
      $description .= ' ' . $config->pointsOverviewSuffix;
    }

    $item['description'] = $description;

    $variables['items'][] = $item;

  }

}

/**
 * Theme the message when a user earned some points.
 */
function theme_culturefeed_userpoints_ui_points_earned_message($variables) {

  $activity = $variables['activity'];
  $points = CultureFeedUserpointsWishlist::getRealPoints($activity->points);
  $total_points = CultureFeedUserpointsWishlist::getRealPoints($activity->newTotalPoints);

  $points_message = format_plural($points, '@count punt', '@count punten');
  $total_points_message = format_plural($total_points, '@count punt', '@count punten');

  $config = CultureFeedActivityConfigBase::loadByType($activity->type);
  if (!$config) {
    return;
  }

  $message = '<p>' . t('Congratulations! you collected') . ' <strong>' . $points_message . '</strong> ' . t('and have now') . ' <strong>' . $total_points_message . '</strong></p>';

  return $message;

}

/**
 * Theme the message when a user lost some points.
 */
function theme_culturefeed_userpoints_ui_points_lost_message($variables) {

  $activity = $variables['activity'];
  $points = CultureFeedUserpointsWishlist::getRealPoints($activity->points);
  $total_points = CultureFeedUserpointsWishlist::getRealPoints($activity->newTotalPoints);

  $points_message = format_plural($points, '@count punt', '@count punten');
  $total_points_message = format_plural($total_points, '@count punt', '@count punten');

  $config = CultureFeedActivityConfigBase::loadByType($activity->type);
  if (!$config) {
    return;
  }

  $message = '<p>' . t('You lost') . ' <strong>' . $points_message . '</strong> ' . t('with your @action and have now', array('@action' => strtolower($config->action))) . ' <strong>' . $total_points_message . '</strong></p>';

  return $message;
}

/**
 * Theme function for a message when the userpoints were exchanged.
 * @param array $variables
 */
function theme_culturefeed_userpoints_ui_points_exchanged_message($variables) {

  return t('Thank you, your points have been exchanged. Your gift will be sent shortly to the specified address.');

}

/**
 * Preprocess the exchange confirmation page.
 * @see culturefeed-userpoints-ui-page-exchange-confirmation.tpl.php
 */
function culturefeed_userpoints_ui_preprocess_culturefeed_userpoints_ui_page_exchange_confirmation(&$variables) {

  $variables['total_points'] = 0;
  try {
    $cf_user = DrupalCultureFeed::getLoggedInUser();
    $variables['total_points'] = CultureFeedUserpointsWishlist::getRealPoints($cf_user->points);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_userpoints_ui', $e);
  }

}
