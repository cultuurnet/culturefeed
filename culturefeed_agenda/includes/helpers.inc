<?php
/**
 * @file
 * Helper functions for the culturefeed agenda.
 **/

/**
 * Returns a link to the detail page from the given detail type / cdbid.
 * @param $type
 *   Type of object to load. This can be event / production or actor.
 * @param $cdbid
 *   Cdbid from the item.
 * @param $slug_title
 *   Title to be used as slug in the url.
 * @param $display_title
 *   Title to show as link. If empty, the slug title will be used.
 * @param $options
 *   An associative array of additional options for the l() function.
 */
function culturefeed_agenda_detail_l($type, $cdbid, $slug_title, $display_title = NULL, $options = array()) {

  $url = culturefeed_agenda_detail_url($type, $cdbid, $slug_title);

  if (empty($display_title)) {
    $display_title = $slug_title;
  }

  return l($display_title, $url, $options);

}

/**
 * Returns the url to the detail page from the given detail type / cdbid.
 * @param $type
 *   Type of object to load. This can be event / production or actor.
 * @param $cdbid
 *   Cdbid from the item.
 * @param $title
 *   Title to be used as slug in the url.
 * @param $options
 *   An associative array of additional options for the url() function.
 *
 */
function culturefeed_agenda_detail_url($type, $cdbid, $title, $options = array()) {

  // Currenly detail pages are always the first letter of type. (event = e, actor = a, production = p).
  return url('agenda/' . $type[0] . culturefeed_search_slug($title) . '/' . $cdbid);

}
