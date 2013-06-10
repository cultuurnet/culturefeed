<?php
/**
 * Page callback for the userpoints handling.
 */

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
      '#points_in_wishlist' => CultureFeedUserpointsWishlist::getPointsInWishlist(),
      '#points_left' => CultureFeedUserpointsWishlist::getPointsLeft($cf_account->points),
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
 * Page callback to select a promotion to exchange for userpoints.
 */
function culturefeed_userpoints_ui_promotion_select_page($promotion_id, $request_type, $operation = 'add') {
  
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
    
    // Update the list.
    $list_build = culturefeed_userpoints_ui_promotions_list($pointsPromotions);
    $list = drupal_render($list_build);
    $commands[] = ajax_command_html('#culturefeed-userpoints-promotions-list', $list);
    
    // Update the wishlist.
    $wishlist_build = culturefeed_userpoints_ui_get_wishlist($pointsPromotions->objects);
    $wishlist = drupal_render($wishlist_build);
    $commands[] = ajax_command_html('#culturefeed-userpoints-wishlist', $wishlist);
    
    // Update the current userpoints in all places where the points are displayed.
    $user_points = DrupalCultureFeed::getLoggedInUser()->points;
    $commands[] = ajax_command_invoke('.userpoints-points', 'html', array(
      CultureFeedUserpointsWishlist::getPointsLeft($user_points)));
    
    ajax_deliver(array('#type' => 'ajax', '#commands' => $commands));
    
  }
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
  
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => 'E-mailadres',
    '#default_value' => $cf_user->mbox,
    '#required' => TRUE,
  );
  
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => 'Naam',
    '#default_value' => $cf_user->familyName,
    '#required' => TRUE,
  );
  
  $form['firstname'] = array(
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
  
  $form['number'] = array(
    '#type' => 'textfield',
    '#title' => 'Nummer',
    '#default_value' => $cf_user->street,
    '#required' => TRUE,
  );
  
  $form['box'] = array(
    '#type' => 'textfield',
    '#title' => 'Bus',
    '#default_value' => '',
  );
  
  $form['postalcode'] = array(
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
    
    // @todo Update the user profile.
    // Check the fields with nils first.
    
    // Thank you message.
    if (count($pointsPromotions) > 1) {
      $message = theme('culturefeed_userpoints_ui_points_exchanged_message', array('promotions' => $pointsPromotions));
      drupal_set_message($message);
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
