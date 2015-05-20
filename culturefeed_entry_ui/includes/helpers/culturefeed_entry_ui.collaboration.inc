<?php

/**
 * @file
 * Contains culturefeed_entry_ui.collaboration.inc.
 */

/**
 * Returns the collaboration data for a specific keyword.
 *
 * @param \CultureFeed_Cdb_Item_Event $event
 *   The event.
 * @param string $keyword
 *   The keyword.
 *
 * @return array
 *   The data.
 */
function culturefeed_entry_ui_collaboration_get_data_by_keyword(\CultureFeed_Cdb_Item_Event $event, $keyword) {

  $info = array(
    'keyword' => $keyword,
  );

  $consumer_key = variable_get('culturefeed_api_application_key', '');

  /* @var \CultureFeed_Cdb_Data_EventDetail $event_details */
  $event_details = $event->getDetails()->getDetailByLanguage(
    culturefeed_entry_ui_get_preferred_language()
  );

  if ($event_details) {
    $media_links = $event_details->getMedia()->byMediaTypes(array(CultureFeed_Cdb_Data_File::MEDIA_TYPE_ROADMAP));
    foreach ($media_links as $link) {

      if ($link->getSubBrand() && $link->getSubBrand() == $consumer_key) {

        $description = drupal_json_decode($link->getDescription());

        if (isset($description['keyword']) && $description['keyword'] == $keyword) {

          $info['copyright'] = $link->getCopyright();
          $info['description'] = $description;
          $info['title'] = $link->getTitle();

        }

      }

    }

  }

  return $info;

}
