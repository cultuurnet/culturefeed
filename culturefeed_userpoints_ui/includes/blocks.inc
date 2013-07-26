<?php
/**
 * @file
 * Blocks for the Culturefeed Userpoints module.
 */

/**
 * Returns a block with your userpoints balance.
 */
function culturefeed_userpoints_ui_block_balance() {
  
  $cf_user = DrupalCultureFeed::getLoggedInUser();
  if (!$cf_user) {
    return;
  }
  
  $output = array(
    '#theme' => 'culturefeed_userpoints_ui_my_balance',
  );

  return $output;
  
}

/**
 * Returns a block with previous en next promotions.
 */
function culturefeed_userpoints_ui_block_more_promotions() {

  $cf_user = DrupalCultureFeed::getLoggedInUser();
  if (!$cf_user) {
    return;
  }

  try {
  
    $current_item = menu_get_object('culturefeed_userpoints_promotion', 2);
    $data = culturefeed_userpoints_ui_get_pointspromotions(array('sort' => 'CREATION_DATE', 'order' => 'ASC'));
    
    $previous_item = NULL;
    $next_item = NULL;
    
    foreach ($data->objects as $i => $pointsPromotion) {
  
      if ($current_item->creationDate == $pointsPromotion->creationDate) {
        if (isset($data->objects[$i - 1])) {
          $previous_item = $data->objects[$i - 1];
        }
        if (isset($data->objects[$i + 1])) {
          $next_item = $data->objects[$i + 1];
        }
      }
      
    }
    
    $output = array(
      '#previous_item' => $previous_item,
      '#next_item' => $next_item,
      '#theme' => 'culturefeed_userpoints_ui_block_more_promotions',
    );
    
  }
  catch (Exception\ClientErrorResponseException $e) {
    watchdog_exception('culturefeed_userpoints_ui', $e);
    $output = t('There was an error while loading the promotions.');
  }
  catch (Exception\CurlException $e) {
    watchdog_exception('culturefeed_userpoints_ui', $e);
    $output = t('There was an error while loading the promotions.');
  }
  catch (CultureFeed_ParseException $e) {
    watchdog_exception('culturefeed_userpoints_ui', $e);
    $output = t('There was an error while loading the promotions.');
  }
  
  return $output;
  
}
