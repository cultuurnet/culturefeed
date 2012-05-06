<?php

/**
* @file
* Callback and handler implementations for administration pages.
*/

/**
* Configuration form for uitpas promotions highlight.
*/
function uitpas_ui_admin_promotions_highlight_form($form, &$form_state) {
  $options = array();
  try {
    $cf = DrupalCultureFeed::getConsumerInstance();
    $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
    $query->sort = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::SORT_CASHING_PERIOD_END;
    $result = $cf->uitpas()->getPromotionPoints($query);
    $uitpas_ui_promotions_highlight = variable_get('uitpas_ui_promotions_highlight', array());
    $form['uitpas_ui_promotions_highlight'] = array();
    foreach ($result->objects as $promotion) {
      $default_value = (isset($uitpas_ui_promotions_highlight[$promotion->id]) && $uitpas_ui_promotions_highlight[$promotion->id]) ? 1 : 0;
      $form['uitpas_ui_promotions_highlight'][$promotion->id] = array(
        'value' => array(
          '#type' => 'checkbox',
          '#default_value' => $default_value,
          '#title' => $promotion->title,
        ),
        'weight' => array(
          '#type' => 'weight',
          '#delta' => 50,
          '#default_value' => 0,
          '#title_display' => 'invisible',
          '#title' => '',
        ),
      );
    }
  }
  catch (Exception $e) {
    watchdog_exception('uitpas_ui_admin_promotions_highlight', $e);
  }
  $form['#theme'] = 'uitpas_ui_admin_promotions_highlight_form';
  return $form;
}