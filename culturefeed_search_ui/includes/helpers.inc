<?php

/**
 * Extracted from culturefeed_ui_block_users_activity_view, should be made more generic.
 *
 * @param CultuurNet\Search\ActivityStatsExtendedEntity $activityStatsExtendedEntity
 * @return string
 */
function culturefeed_search_ui_activity_recommend_link(\CultuurNet\Search\ActivityStatsExtendedEntity $activityStatsExtendedEntity) {
  return "TODO: recommended link";

  $user_did_this = FALSE;

  $activity_type = CultureFeed_Activity::TYPE_RECOMMEND;

  module_load_include('inc', 'culturefeed_ui', 'culturefeed_ui.blocks');
  $config = culturefeed_ui_block_users_recommends_config();

  /* @var CultureFeed_Cdb_Item_Event $event */
  $event = $activityStatsExtendedEntity->getEntity();
  $node_id = $event->getCdbId();

  $activity_content_type = 'event';

  //return '';

  if (DrupalCultureFeed::isCultureFeedUser()) {
    try {
      $user_did_this = DrupalCultureFeed::userDidActivity(CultureFeed_Activity::TYPE_RECOMMEND, $event->getCdbId(), CultureFeed_Activity::CONTENT_TYPE_EVENT);
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_ui', $e);
    }
  }

  $current_query = drupal_get_query_parameters();
  $path = current_path();

  $query = array(
    'destination' => url($path, array('query' => $current_query)),
  );

  $link_options = array(
    'query' => $query,
  );

  if (!$user_did_this) {
    $url = 'culturefeed/do/' . $activity_type . '/' . $activity_content_type . '/' . $node_id;
    if (0 == $activityStatsExtendedEntity->getActivityCount('recommend')) {
      $title = $config['do_first_link_title'];
    }
    else {
      $title = $config['do_link_title'];
    }

    $link_options['attributes'] = array(
      'class' => 'do-link ' . 'do-' . $config['activity_type'] . '-link',
    );

    $markup = '<div class="do-link-wrapper">' . l($title, $url, $link_options) . '</div>';

    if (user_is_anonymous()) {
      $markup .= theme('culturefeed_ui_connect_hover', array('url' => $url));
    }

    return $markup;

  }
  else {
    $url = 'culturefeed/undo/' . $activity_type . '/' . $activity_content_type . '/' . $node_id;
    $title = $config['undo_link_title'];

    $link_options['attributes'] = array(
      'class' => 'undo-link ' . 'undo-' . $config['activity_type'] . '-link',
    );

    return '<div class="undo-link-wrapper">' . l($title, $url, $link_options) . '</div>';
  }
}

/**
 * Convert a given url to a youtube embed.
 */
function _culturefeed_search_ui_video_url_to_embed($url, $width, $height) {

  // Construct embed url.
  $embed_url = str_replace('watch', 'v', $url);

  $video_parts = explode('?v=', $embed_url);

  if (count($video_parts) < 2) {
    return FALSE;
  }
  $embed_url = $video_parts[0] .'/' . $video_parts[1];
  $embed_url .= '?fs=1&showinfo=0';

  $embed = '';
  $embed .= '<object width="' . $width . '" height="' . $height . '">';
  $embed .= '<param name="movie" value="'.  $embed_url .'"></param>';
  $embed .= '<param name="allowFullScreen" value="true"></param>';
  $embed .= '<param name="wmode" value="transparent"></param>';
  $embed .= '<embed src="'.  $embed_url .'"';
  $embed .= 'type="application/x-shockwave-flash" width="' . $width . '" height="' . $height . '" allowfullscreen="true" wmode="transparent">';
  $embed .= '</embed>';
  $embed .= '</object>';

  return $embed;

}

/**
 * Get the key value from the currently active search page.
 */
function _culturefeed_search_ui_get_active_search_page() {

  $searchable_types = culturefeed_get_searchable_types();
  foreach ($searchable_types as $key => $type) {

    // Check if this is the current page.
    if ($type['path'] == $_GET['q']) {

      // If this page has active facets in the info definition. Check if all the facets matches.
      if (!empty($type['active_facets']) && !empty($query['facet'])) {
        $total_requested = count($type['active_facets']);
        $total_matches = 0;
        // Loop through the requested facets, and check if this is active in current search.
        foreach ($type['active_facets'] as $requested_facet => $requested_values) {

          // If the requested facet is active. Calculate the intersection, and check if all requested values are in the current page facets.
          if (isset($query['facet'][$requested_facet])) {
            $matches = array_intersect($requested_values, $query['facet'][$requested_facet]);
            if (count($matches) == count($requested_values)) {
              $total_matches++;
            }
          }
        }

        // If all the requested facets are found, this type should be default.
        if ($total_matches == $total_requested) {
          return $key;
        }

      }
      else {
        return $key;
      }

    }
  }

  return NULL;

}