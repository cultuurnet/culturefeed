<?php

/**
 * @file
 * Helper functions for UiTPAS event details advantages.
 */

/**
 * Returns event details advantages.
 */
function culturefeed_uitpas_event_details_advantages_get() {

  $item = menu_get_object('culturefeed_agenda_event', 3);
  $event = culturefeed_uitpas_get_uitpas_event($item);

  if ($event) {

    // Promotions.
    $promotions = array();

    try {

      /* @var \DrupalCulturefeed_Cache $cf */
      $cf = DrupalCultureFeed::getLoggedInUserInstance();

      $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
      $query->balieConsumerKey = $event->organiserId;
      $query->cashingPeriodBegin = time();
      $query->unexpired = TRUE;
      $query->start = 0;
      $query->max = 5;

      do {
        $result = $cf->uitpas()->getPromotionPoints($query);
        $promotions = array_merge($promotions, $result->objects);
        $query->start += $query->max;
      } while ($query->start < $result->total);

    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_uitpas_event_details_promotions', $e);
    }

    if (count($promotions)) {

      $title = t('Promotions at @organisation', array('@organisation' => $event->organiserName));
      $show_all_link_title = t('All promotions');
      $show_all_link_path = 'promotions';

      $table = array(
        'rows' => array(),
      );
      // A helper array in case the default implementation needs to be
      // overridden.
      $promotions_array = array();

      foreach ($promotions as $promotion) {

        /* @var \CultureFeed_Uitpas_Passholder_PointsPromotion $promotion */
        $promotion_title = $promotion->title;
        $promotion_points = $promotion->points;
        $points_remark = '';
        $points_classes = array('points-value', 'points-value__brand');

        /* @var \CultureFeed_User $cf_user */
        $cf_user = DrupalCultureFeed::getLoggedInUser();
        if ($cf_user && $cf_user->points < $promotion->points) {
          $points_remark = t('You have insufficient points');
          $points_classes[] = 'insufficient-points';
        }

        $points_html = theme('html_tag', array(
          'element' => array(
            '#attributes' => array('class' => $points_classes),
            '#tag' => 'span',
            '#value' => format_plural($promotion->points, '1 point', '@count points'),
          ),
        ));

        if ($points_remark) {
          $points_html .= theme('html_tag', array(
            'element' => array(
              '#attributes' => array('class' => array('points-remark')),
              '#tag' => 'span',
              '#value' => $points_remark,
            ),
          ));
        }

        $table['rows'][] = array(
          $promotion_title,
          $points_html,
        );

        $promotions_array[] = array(
          'points' => $promotion_points,
          'points_classes' => $points_classes,
          'points_remark' => $points_remark,
          'title' => $promotion_title,
        );

      }

      return array(
        '#theme' => 'culturefeed_uitpas_event_details_advantages',
        '#show_all_link_title' => $show_all_link_title,
        '#show_all_link_path' => $show_all_link_path,
        '#show_all_link_default_render' => l($show_all_link_title, $show_all_link_path),
        '#promotions' => $promotions,
        '#promotions_array' => $promotions_array,
        '#promotions_default_render' => theme('table', $table),
        '#title' => $title,
      );

    }

  }

}
