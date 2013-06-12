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
      $message = theme('culturefeed_userpoints_ui_points_exchanged_message', array('promotions' => $pointsPromotions));
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
