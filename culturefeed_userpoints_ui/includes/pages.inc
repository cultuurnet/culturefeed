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
    $build = "Er is een fout opgetreden tijdens het laden van de gebruikers voordelen.";
  }
  catch (Exception\CurlException $e) {
    watchdog_exception('culturefeed_userpoints_ui', $e);
    $build = "Er is een fout opgetreden tijdens het laden van de gebruikers voordelen.";
  }
  catch (CultureFeed_ParseException $e) {
    watchdog_exception('culturefeed_userpoints_ui', $e);
    $build = "Er is een fout opgetreden tijdens het laden van de gebruikers voordelen.";
  }

  return $build;

}

/**
 * Page callback to list the gifts and exchange the points for a gift.
 */
function culturefeed_userpoints_ui_exchange_page() {

  $cf_account = DrupalCultureFeed::getLoggedInUser();

  try {

    $pointsPromotions = culturefeed_userpoints_ui_get_pointspromotions();

    $build = array();

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
    $build = "Er is een fout opgetreden tijdens het laden van de gebruikers voordelen.";
  }
  catch (Exception\CurlException $e) {
    watchdog_exception('culturefeed_userpoints_ui', $e);
    $build = "Er is een fout opgetreden tijdens het laden van de gebruikers voordelen.";
  }
  catch (CultureFeed_ParseException $e) {
    watchdog_exception('culturefeed_userpoints_ui', $e);
    $build = "Er is een fout opgetreden tijdens het laden van de gebruikers voordelen.";
  }

  return $build;

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
    drupal_set_message('Het voordeel werd verwijderd van uw voordelen.');
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
    if ($wishlistItem = CultureFeedUserpointsWishlist::get($promotion->id))  {
      $default_count = $wishlistItem['count'];
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
      '#value' => 'Selecteer',
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
    '#title' => 'E-mailadres',
    '#default_value' => $cf_user->mbox,
    '#required' => TRUE,
  );

  $form['familyName'] = array(
    '#type' => 'textfield',
    '#title' => 'Naam',
    '#default_value' => $cf_user->familyName,
    '#required' => TRUE,
  );

  $form['givenName'] = array(
    '#type' => 'textfield',
    '#title' => 'Voornaam',
    '#default_value' => $cf_user->givenName,
    '#required' => TRUE,
  );

  $form['street'] = array(
    '#type' => 'textfield',
    '#title' => 'Straat',
    '#default_value' => $cf_user->street,
    '#required' => TRUE,
  );

  $form['zip'] = array(
    '#type' => 'textfield',
    '#title' => 'Postcode',
    '#default_value' => $cf_user->zip,
    '#required' => TRUE,
  );

  $form['city'] = array(
    '#type' => 'textfield',
    '#title' => 'Gemeente',
    '#default_value' => $cf_user->city,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Geschenk aanvragen'
  );

  return $form;

}

/**
 * Validate handler to exchange selected promotions for userpoints.
 * @param array $form
 * @param array $form_state
 * @param Culturefeed_User $cf_account
 */
function culturefeed_userpoints_ui_exchange_form_validate($form, $form_state) {

  $count = CultureFeedUserpointsWishlist::getIds();
  if ($count <= 0) {
    form_set_error('', 'U moet minstens 1 promotie selecteren.');
  }

  try {
    $user_points = DrupalCultureFeed::getLoggedInUser()->points;
    $form_state['points_left'] = CultureFeedUserpointsWishlist::getPointsLeft($user_points);
    if ($form_state['points_left'] < 0) {
      form_set_error('', 'U hebt niet voldoende punten om dit te bestellen');
    }
  }
  catch (Exception $e) {
   watchdog_exception('culturefeed_userpoints_ui', $e);
  }

}

/**
 * Submit handler to exchange selected promotions for userpoints.
 * @param array $form
 * @param array $form_state
 * @param Culturefeed_User $cf_account
 */
function culturefeed_userpoints_ui_exchange_form_submit($form, $form_state) {

  try {

    $cf_user = DrupalCultureFeed::getLoggedInUser();
    $pointsPromotions = DrupalCultureFeed::cashInPromotion($cf_user->id,
      CultureFeedUserpointsWishlist::getIds(),
      CultureFeedUserpointsWishlist::getCounts());

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
      drupal_set_message('Jouw gegevens zijn bijgewerkt.');
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_userpoints_ui', $e);
      drupal_set_message('Er is een fout opgetreden bij het bewaren van jouw gegevens.', 'error');
      $success = FALSE;
    }

    // Thank you message if successful.
    if (count($pointsPromotions) >= 1) {
      $message = theme('culturefeed_userpoints_ui_points_exchanged_message', array(
        'promotions' => $pointsPromotions
        'points_left' => $form_state['points_left'],
      ));
      drupal_set_message($message);
      CultureFeedUserpointsWishlist::clear();

      // Send mail to administrator.
      $admin_uid = variable_get('culturefeed_social_report_destination_uid');
      $to_email = empty($admin_uid) ? variable_get('site_mail', '') : user_load($admin_uid)->mail;

      $params = array(
        'promotions' => $pointsPromotions,
        'wishlist' => $wishlistPromotions,
        'values' => $values,
        'cf_account' => DrupalCultureFeed::getLoggedInUser(),
      );

      drupal_mail('culturefeed_userpoints_ui', 'userpoints-exchange-request', $to_email, $GLOBALS['language']->language, $params);

    }

  }
  catch (CultureFeed_Exception $e) {
    drupal_set_message($e->getMessage());
  }
  catch (CultureFeed_InvalidCodeException $e) {
    drupal_set_message($e->getMessage());
  }
  catch (Exception\ClientErrorResponseException $e) {
    watchdog_exception('culturefeed_userpoints_ui', $e);
    $build = "Er is een fout opgetreden tijdens het inruilen van de gebruikers voordelen.";
  }
  catch (Exception\CurlException $e) {
    watchdog_exception('culturefeed_userpoints_ui', $e);
    $build = "Er is een fout opgetreden tijdens het inruilen van de gebruikers voordelen.";
  }
  catch (CultureFeed_ParseException $e) {
    watchdog_exception('culturefeed_userpoints_ui', $e);
    $build = "Er is een fout opgetreden tijdens het inruilen van de gebruikers voordelen.";
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
    drupal_set_message('Er ging iets fout tijdens het laden van je puntenoverzicht', 'error');
    watchdog_exception('culturefeed_userpoints_ui', $e);
  }

  return array(
    '#theme' => 'culturefeed_userpoints_earned_overview',
    '#activities' => $activities,
  );

}
