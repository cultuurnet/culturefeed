<?php
/**
 * Page callback for the userpoints handling.
 */

/**
 * Page callback for a list of promotions.
 */
function culturefeed_userpoints_ui_promotions_page() {

  try {

    $pointsPromotions = culturefeed_userpoints_ui_get_pointspromotions();
    $build = array(
      '#theme' => 'culturefeed_userpoints_ui_promotions_page',
      '#promotions' => $pointsPromotions,
    );

  }
  catch (Exception\ClientErrorResponseException $e) {
    watchdog_exception('culturefeed_userpoints_ui', $e);
    $build = t('There was an error while loading the promotions.');
  }
  catch (Exception\CurlException $e) {
    watchdog_exception('culturefeed_userpoints_ui', $e);
    $build = t('There was an error while loading the promotions.');
  }
  catch (CultureFeed_ParseException $e) {
    watchdog_exception('culturefeed_userpoints_ui', $e);
    $build = t('There was an error while loading the promotions.');
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_userpoints_ui', $e);
    $build = '';
  }

  return $build;

}

/**
 * Page callback to list the gifts and exchange the points for a gift.
 */
function culturefeed_userpoints_ui_exchange_page() {

  try {

    $cf_account = DrupalCultureFeed::getLoggedInUser();

    $pointsPromotions = culturefeed_userpoints_ui_get_pointspromotions();

    $build = array();

    if (!culturefeed_is_culturefeed_user()) {

      $build['message'] = array(
        '#theme' => 'culturefeed_ui_connect_hover',
        '#url' => $_GET['q'],
      );
      
      return $build;

    }

    $form = drupal_get_form('culturefeed_userpoints_ui_exchange_form', $cf_account);

    $js_setting = array(
      'cultureFeedUserpointsWishlist' => array(
        'selectedIds' => CultureFeedUserpointsWishlist::getIds(),
      ),
    );
    $build = array(
      '#theme' => 'culturefeed_userpoints_ui_exchange_page',
      '#points' => CultureFeedUserpointsWishlist::getRealPoints($cf_account->points),
      '#cf_user' => $cf_account,
      '#promotions' => $pointsPromotions,
      '#promotions_form' => drupal_render($form),
      '#attached' => array(
        'library' => array(array('system', 'drupal.ajax')),
        'js' => array(
          array('data' => $js_setting, 'type' => 'setting'),
        ),
      ),
    );

  }
  catch (Exception\ClientErrorResponseException $e) {
    watchdog_exception('culturefeed_userpoints_ui', $e);
    $build = t('There was an error while loading the promotions.');
  }
  catch (Exception\CurlException $e) {
    watchdog_exception('culturefeed_userpoints_ui', $e);
    $build = t('There was an error while loading the promotions.');
  }
  catch (CultureFeed_ParseException $e) {
    watchdog_exception('culturefeed_userpoints_ui', $e);
    $build = t('There was an error while loading the promotions.');
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_userpoints_ui', $e);
    drupal_not_found();
    exit();
  }

  return $build;

}

/**
 * Page callback to show a confirmation page.
 */
function culturefeed_userpoints_ui_page_exchange_confirmation() {

  return array('#theme' => 'culturefeed_userpoints_ui_page_exchange_confirmation');

}

/**
 * Page callback to remove a promotion in the wishlist.
 */
function culturefeed_userpoints_ui_page_remove_promotion($promotion_id, $request_type = 'nojs') {

  $promotion_points = culturefeed_userpoints_ui_get_pointspromotions();

  // Get the correct promotion
  $promotion = NULL;
  foreach ($promotion_points->objects as $object) {
    if ($object->id == $promotion_id) {
      $promotion = $object;
    }
  }

  CultureFeedUserpointsWishlist::remove($promotion_id);

  // Check if this is a non ajax request.
  if ($request_type != 'ajax') {
    drupal_set_message(t('The advantage was removed from your promotions.'));
    drupal_goto('userpoints/promotions/exchange');
  }
  else {
    $form = drupal_get_form('culturefeed_userpoints_ui_promotions_list_form', $promotion_points);
    ajax_deliver(culturefeed_userpoints_ui_promotions_list_ajax_refresh($form, $promotion_points));
    exit();
  }

}

/**
 * Page callback to load and display one promotion.
 * @param CultureFeed_PointsPromotion $pointsPromotion
 */
function culturefeed_userpoints_ui_detail_page($pointsPromotion) {
  return array(
    '#theme' => 'culturefeed_userpoints_ui_detail_page',
    '#promotion' => $pointsPromotion,
  );
}

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

  $cf_user = DrupalCultureFeed::getLoggedInUser();

  $objects = array();
  foreach ($promotions->objects as $object) {
    $objects[] = $object;
  }

  $form['#tree'] = TRUE;

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
    );

    $max_count = floor($cf_user->points / $promotion->points);
    if (!empty($promotion->maxAvailableUnits)) {
      $max_count = $promotion->maxAvailableUnits < $max_count ? $promotion->maxAvailableUnits : $max_count;
    }
    $options = culturefeed_userpoints_ui_get_max_available_units_options($max_count);
    $form['list'][$promotion->id]['select_id'] = array(
      '#type' => 'hidden',
      '#default_value' => $promotion->id,
    );

    $default_count = 1;
    $submit_label = t('Add');
    if ($wishlistItem = CultureFeedUserpointsWishlist::get($promotion->id))  {
      $default_count = $wishlistItem['count'];
      $submit_label = t('Edit');
    }

    $form['list'][$promotion->id]['select_count'] = array(
      '#type' => 'select',
      '#title' => '',
      '#default_value' => $default_count,
      '#options' => $options,
    );

    $form['list'][$promotion->id]['select_btn'] = array(
      '#name' => 'select_btn_' . $promotion->id,
      '#type' => 'submit',
      '#value' => $submit_label,
      '#ajax' => array(
        'callback' => 'culturefeed_userpoints_ui_promotions_list_form_callback',
        'wrapper' => 'culturefeed-userpoints-promotions-list',
      ),
      '#submit' => array('culturefeed_userpoints_ui_promotions_list_form_submit'),
      '#validate' => array('culturefeed_userpoints_ui_promotions_list_form_validate'),
    );

  }

  return $form;

}

/**
 * Form to exchange selected promotions for userpoints.
 * @param array $form
 * @param array $form_state
 * @param Culturefeed_User $cf_account
 */
function culturefeed_userpoints_ui_exchange_form($form, $form_state, $cf_account) {

  $cf_user = DrupalCultureFeed::getLoggedInUser();

  $form = array(
    '#theme' => 'culturefeed_userpoints_ui_exchange_form',
  );

  $form['mbox'] = array(
    '#type' => 'textfield',
    '#title' => t('Email address'),
    '#default_value' => $cf_user->mbox,
    '#required' => TRUE,
  );

  $form['familyName'] = array(
    '#type' => 'textfield',
    '#title' => t('Family name'),
    '#default_value' => $cf_user->familyName,
    '#required' => TRUE,
  );

  $form['givenName'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#default_value' => $cf_user->givenName,
    '#required' => TRUE,
  );

  $form['street'] = array(
    '#type' => 'textfield',
    '#title' => t('Street'),
    '#default_value' => $cf_user->street,
    '#required' => TRUE,
  );

  $form['zip'] = array(
    '#type' => 'textfield',
    '#title' => t('Zipcode'),
    '#default_value' => $cf_user->zip,
    '#required' => TRUE,
  );

  $form['city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#default_value' => $cf_user->city,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Get promotion')
  );

  return $form;

}

/**
 * Validate handler to exchange selected promotions for userpoints.
 * @param array $form
 * @param array $form_state
 * @param Culturefeed_User $cf_account
 */
function culturefeed_userpoints_ui_exchange_form_validate($form, &$form_state) {

  $count = CultureFeedUserpointsWishlist::getIds();
  if ($count <= 0) {
    form_set_error('', t('You must choose at least one promotion.'));
  }

  try {

    $user_points = DrupalCultureFeed::getLoggedInUser()->points;
    $form_state['points_left'] = CultureFeedUserpointsWishlist::getPointsLeft($user_points);
    if ($form_state['points_left'] < 0) {
      form_set_error('', t('You do not have enough points to order this'));
    }

  }
  catch (Exception $e) {
   watchdog_exception('culturefeed_userpoints_ui', $e);
  }

  try {
    $cf_user = DrupalCultureFeed::getLoggedInUser();
    $form_state['pointsPromotions'] = DrupalCultureFeed::cashInPromotion($cf_user->id, CultureFeedUserpointsWishlist::getIds(), CultureFeedUserpointsWishlist::getCounts());
  }
  catch (CultureFeed_Exception $e) {

    if ($e->getCode() == CultureFeed_PointsPromotion::CASHIN_PROMOTION_NOT_ALLOWED) {
      form_set_error('', t('You are not allowed to cashin this promotions because the allowed amount has been exceeded.'));
    }
    else {
      form_set_error('', t('It is not possible to process the order'));
    }
    watchdog_exception('culturefeed_userpoints_ui', $e);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_userpoints_ui', $e);
    form_set_error('', t('It is not possible to process the order'));
  }
}

/**
 * Submit handler to exchange selected promotions for userpoints.
 */
function culturefeed_userpoints_ui_exchange_form_submit($form, &$form_state) {

  try {

  	$pointsPromotions = $form_state['pointsPromotions'];
    $wishlistPromotions = CultureFeedUserpointsWishlist::getItems($pointsPromotions);

    // Always update the user profile.
    $values = $form_state['values'];
    $user_update = new CultureFeed_User();
    $user_update->id         = DrupalCultureFeed::getLoggedInUserId();
    $user_update->givenName  = $values['givenName'];
    $user_update->familyName = $values['familyName'];
    $user_update->street     = $values['street'];
    $user_update->zip        = $values['zip'];
    $user_update->city       = $values['city'];
    $user_update->mbox       = $values['mbox'];
    $fields = array('id', 'givenName', 'familyName', 'street', 'zip', 'city', 'mbox');

    try {
      DrupalCultureFeed::updateUser($user_update, $fields);
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_userpoints_ui', $e);
      drupal_set_message(t('An error occurred while saving your data.'), 'error');
      $success = FALSE;
    }

    // Thank you message if successful.
    if (count($pointsPromotions) >= 1) {
      $message = theme('culturefeed_userpoints_ui_points_exchanged_message', array(
        'promotions' => $pointsPromotions,
        'points_left' => $form_state['points_left'],
      ));
      drupal_set_message($message);
      CultureFeedUserpointsWishlist::clear();

      // Send mail to administrator.
      $to_email = variable_get('culturefeed_userpoints_ui_exchange_email', '');

      $params = array(
        'promotions' => $pointsPromotions,
        'wishlist' => $wishlistPromotions,
        'values' => $values,
        'cf_account' => DrupalCultureFeed::getLoggedInUser(),
      );

      drupal_mail('culturefeed_userpoints_ui', 'userpoints-exchange-request', $to_email, $GLOBALS['language']->language, $params);

    }

    $form_state['redirect'] = 'userpoints/promotions/confirmation';

  }
  catch (CultureFeed_Exception $e) {
    drupal_set_message($e->getMessage());
  }
  catch (CultureFeed_InvalidCodeException $e) {
    drupal_set_message($e->getMessage());
  }
  catch (Exception\ClientErrorResponseException $e) {
    watchdog_exception('culturefeed_userpoints_ui', $e);
    $build = t('There was an error during the exchange of your promotions.');
  }
  catch (Exception\CurlException $e) {
    watchdog_exception('culturefeed_userpoints_ui', $e);
    $build = t('There was an error during the exchange of your promotions.');
  }
  catch (CultureFeed_ParseException $e) {
    watchdog_exception('culturefeed_userpoints_ui', $e);
    $build = t('There was an error during the exchange of your promotions.');
  }

}

/**
 * Page callback to show the overview of points for current user.
 */
function culturefeed_userpoints_ui_page_points_overview() {

  $activities = array();
  try {
    $timeline = DrupalCultureFeed::getActivityPointsTimeline(DrupalCultureFeed::getLoggedInUserId());
    $activities = $timeline->objects;
  }
  catch (Exception $e) {
    drupal_set_message(t('There was an error loading your points list'), 'error');
    watchdog_exception('culturefeed_userpoints_ui', $e);
  }

  return array(
    '#theme' => 'culturefeed_userpoints_earned_overview',
    '#activities' => $activities,
  );

}
