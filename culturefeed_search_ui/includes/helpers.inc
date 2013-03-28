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