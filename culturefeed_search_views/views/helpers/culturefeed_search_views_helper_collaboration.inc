<?php

/**
 * @file
 * Contains culturefeed_search_views_helper_collaboration.inc.
 */

/**
 * Returns the collaboration data for an event.
 *
 * @param \CultuurNet\Search\ActivityStatsExtendedEntity $extended_entity
 *   The extended entity.
 * @param view $view
 *   The view.
 *
 * @return array
 *   The collaboration data.
 */
function culturefeed_search_views_helper_collaboration_get_data(
  \CultuurNet\Search\ActivityStatsExtendedEntity $extended_entity,
  view $view
) {

  // First check if there are any collaboration tags used in the view.
  $collaboration_tags = array_keys(variable_get('culturefeed_entry_ui_custom_collaboration_tags', array()));
  $keywords = array();
  if (count($collaboration_tags)) {

    $filters = $view->display_handler->get_option('filters');
    foreach ($filters as $filter) {

      if (
        isset($filter['table']) && $filter['table'] == 'cdb_items' &&
        isset($filter['field']) && $filter['field'] == 'tags' &&
        isset($filter['keyword']) && is_array($filter['keyword'])
      ) {

        foreach ($filter['keyword'] as $keyword) {

          if (in_array($keyword['value'], $collaboration_tags)) {
            $keywords[] = $keyword['value'];
          }

        }

      }

    }

    // Check the collaboration data for a match.
    if (count($keywords)) {

      // If there are multiple matches, we take the first one.
      $keyword = $keywords[0];
      $media = $extended_entity->getEntity()->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language())->getMedia();
      $cdb_files = $media->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_COLLABORATION);
      $consumer_id = variable_get('culturefeed_api_application_key');

      foreach ($cdb_files as $cdb_file) {

        $sub_brand = $cdb_file->getSubbrand();
        $description = json_decode($cdb_file->getDescription());

        if ($consumer_id == $sub_brand && !is_null($description)) {
          if ($keyword == $description->keyword) {
            return array(
              'copyright' => $cdb_file->getCopyright(),
              'description' => $description,
              'title' => $cdb_file->getTitle(),
            );
            break;
          }
        }

      }
    }
  }
}
