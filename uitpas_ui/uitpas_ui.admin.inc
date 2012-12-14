<?php

/**
* @file
* Callback and handler implementations for administration pages.
*/

/**
* Configuration form for uitpas promotions highlight.
*/
function uitpas_ui_admin_promotions_highlight_form($form, &$form_state) {
  $form['#tree'] = TRUE;
  $form['promotions'] = array();
  try {
    $cf = DrupalCultureFeed::getConsumerInstance();
    $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
    $query->unexpired = TRUE;
    $query->sort = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::SORT_CASHING_PERIOD_END;
    $result = $cf->uitpas()->getPromotionPoints($query);
    $promotions = variable_get('uitpas_ui_promotions_highlight', array());
    $weights = array();
    $max_weight = 0;
    foreach ($promotions as $promotion) {
      $weights[$promotion] = $max_weight++;
    }
    $weight = 0;
    foreach ($result->objects as $promotion) {
      $form['promotions'][$promotion->id] = array(
        'value' => array(
          '#type' => 'checkbox',
          '#default_value' => (in_array($promotion->id, $promotions)) ? 1 : 0,
          '#title' => $promotion->title,
        ),
        'weight' => array(
          '#type' => 'weight',
          '#title' => t('Weight for @title', array('@title' => $promotion->title)),
          '#title_display' => 'invisible',
          '#default_value' => (isset($weights[$promotion->id])) ? $weights[$promotion->id] : $max_weight++,
        ),
      );
    }
    uasort($form['promotions'], 'uitpas_ui_admin_promotions_highlight_form_weight_sort');
  }
  catch (Exception $e) {
    watchdog_exception('uitpas_ui_admin_promotions_highlight', $e);
  }
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save changes'));
  return $form;
}

/**
 * Sorting function for uitpas_ui_admin_promotions_highlight_form().
 */
function uitpas_ui_admin_promotions_highlight_form_weight_sort($a, $b) {
  if ($a['weight']['#default_value'] == $b['weight']['#default_value']) {
    return 0;
  }
  return ($a['weight']['#default_value'] < $b['weight']['#default_value']) ? -1 : 1;
}
        
/**
 * Submit callback form for uitpas promotions highlight.
 */
function uitpas_ui_admin_promotions_highlight_form_submit($form, &$form_state) {
  uasort($form_state['values']['promotions'], 'drupal_sort_weight');
  $promotions = array();
  foreach ($form_state['values']['promotions'] as $key => $promotion) {
    if ($promotion['value']) {
      $promotions[] = $key;
    }
  }
  variable_set('uitpas_ui_promotions_highlight', $promotions);
}