<?php
/**
 * @file
 * Helper functions for the culturefeed_search.
 */

/**
 * @file
 * Leverages convenience function to perform global or specific searches.
 */

/**
 * Gets the search service as a singleton.
 *
 * @return \CultuurNet\Search\ServiceInterface
 */
function culturefeed_get_search_service() {

  $credentials = new \CultuurNet\Auth\ConsumerCredentials();
  $credentials->setKey(variable_get('culturefeed_search_api_application_key', ''));
  $credentials->setSecret(variable_get('culturefeed_search_api_shared_secret', ''));

  return DrupalCultureFeedSearchService::getClient($credentials);

}

/**
 * Load the detail from an item on the search service.
 * @param $cdb_id
 *   Cdb id to load.
 * @return mixed
 *   FALSE if not found, otherwise CultuurNet\Search\ActivityStatsExtendedEntity.
 */
function culturefeed_search_item_load($cdb_id) {

  static $items = array();

  if (isset($items[$cdb_id])) {
    return $items[$cdb_id];
  }

  $parameters = array();
  $parameters[] = new \CultuurNet\Search\Parameter\Group();
  $query_parameter = new \CultuurNet\Search\Parameter\Query($cdb_id);

  $query_parameter->setLocalParam('type', 'term');
  $query_parameter->setLocalParam('f', 'cdbid');
  $parameters[] = $query_parameter;

  try {
    $result = culturefeed_get_search_service()->search($parameters);
    $items = $result->getItems();
    $items[$cdb_id] = reset($items);
    return $items[$cdb_id];
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_search', $e);
  }

  return FALSE;

}

/**
 * Get the current active search object.
 */
function culturefeed_search_get_active_object() {

  $first_argument = arg(0);
  $second_argument = arg(1);

  // Agenda items (events, actors, productions) + pages
  if ($first_argument == 'agenda') {

    if ($second_argument == 'g') {
      return menu_get_object('culturefeed_pages_page', 3);
    }
    elseif ($second_argument == 'e' || $second_argument == 'p' || $second_argument == 'a') {
      return culturefeed_agenda_get_active_object();
    }

  }
  // Bibnet books
  elseif ($first_argument == 'book') {
    return menu_get_object('bibnet_book', 1);
  }
  // Drupal nodes.
  else {
    $node = menu_get_object();
    if ($node) {
      return DrupalNodeExtendedEntity::fromDrupal($node);
    }
  }

}

/**
 * @param string $type
 * @param CultureFeed_Cdb_Item_Base $item
 * @return array()
 */
function culturefeed_cdb_item_get_categories($type, CultureFeed_Cdb_Item_Base $item) {
  $categories = array();
  if ($item->getCategories()) {
    foreach ($item->getCategories() as $category) {
      if ($category->getType() == $type) {
        $categories[] = $category;
      }
    }
  }
  return $categories;
}

/**
 * Returns a link to the detail page from the given detail type / cdbid.
 *
 * @param $type
 *   Type of object to load. Example event / production / actor.
 * @param $cdbid
 *   Cdbid from the item.
 * @param $slug_title
 *   Title to be used as slug in the url.
 * @param $display_title
 *   Title to show as link. If empty, the slug title will be used.
 * @param $options
 *   An associative array of additional options for the l() function.
 */
function culturefeed_search_detail_l($type, $cdbid, $slug_title, $display_title = NULL, $options = array()) {

  $url = culturefeed_search_detail_url($type, $cdbid, $slug_title);

  if (empty($display_title)) {
    $display_title = $slug_title;
  }

  // Set external TRUE, so the second url() wont add an extra /.
  $options['external'] = TRUE;

  return l($display_title, $url, $options);

}

/**
 * Returns the url to the detail page from the given detail type / cdbid.
 *
 * @param $type
 *   Type of object to load. Example event / production / actor/ page.
 * @param $cdbid
 *   Cdbid from the item.
 * @param $title
 *   Title to be used as slug in the url.
 * @param $options
 *   An associative array of additional options for the url() function.
 */
function culturefeed_search_detail_url($type, $cdbid, $title, $options = array()) {
  $path = culturefeed_search_detail_path($type, $cdbid, $title);
  return url($path, $options);
}

/**
 * Returns the path to the detail page from the given detail type / cdbid.
 *
 * @param $type
 *   Type of object to load. Example event / production / actor/ page.
 * @param $cdbid
 *   Cdbid from the item.
 * @param $title
 *   Title to be used as slug in the url.
 */
function culturefeed_search_detail_path($type, $cdbid, $title) {

  switch ($type) {

    // Currenly agenda detail pages are always the first letter of type. (event = e, actor = a, production = p).
    case 'event':
    case 'production':
    case 'actor':
      $url_prefix = 'agenda/' . $type[0];
    break;

    case 'page':
      $url_prefix = 'agenda/g';
    break;

    default:
      return '';

  }

  return $url_prefix . '/' . culturefeed_search_slug($title) . '/' . $cdbid;

}

/**
 * Calculate a slug with a maximum length for a string.
 *
 * @param $string
 *   The string you want to calculate a slug for.
 * @param $length
 *   The maximum length the slug can have.
 * @return
 *   A string representing the slug
 */
function culturefeed_search_slug($string, $length = 50, $separator = '-') {

  // transliterate
  $string = culturefeed_search_transliterate($string);

  // lowercase
  $string = strtolower($string);

  // replace non alphanumeric and non underscore charachters by separator
  $string = preg_replace('/[^a-z0-9]/i', $separator, $string);

  // replace multiple occurences of separator by one instance
  $string = preg_replace('/' . preg_quote($separator) . '[' . preg_quote($separator) . ']*/', $separator, $string);

  // cut off to maximum length
  if ($length > -1 && strlen($string) > $length) {
    $string = substr($string, 0, $length);
  }

  // remove separator from start and end of string
  $string = preg_replace('/' . preg_quote($separator) . '$/', '', $string);
  $string = preg_replace('/^' . preg_quote($separator) . '/', '', $string);

  return $string;

}

/**
 * Transliterate a given string.
 *
 * @param $string
 *   The string you want to transliterate.
 * @return
 *   A string representing the transliterated version of the input string.
 */
function culturefeed_search_transliterate($string) {
  static $charmap;
  if (!$charmap) {
    $charmap = array(
      // Decompositions for Latin-1 Supplement
      chr(195) . chr(128) => 'A', chr(195) . chr(129) => 'A',
      chr(195) . chr(130) => 'A', chr(195) . chr(131) => 'A',
      chr(195) . chr(132) => 'A', chr(195) . chr(133) => 'A',
      chr(195) . chr(135) => 'C', chr(195) . chr(136) => 'E',
      chr(195) . chr(137) => 'E', chr(195) . chr(138) => 'E',
      chr(195) . chr(139) => 'E', chr(195) . chr(140) => 'I',
      chr(195) . chr(141) => 'I', chr(195) . chr(142) => 'I',
      chr(195) . chr(143) => 'I', chr(195) . chr(145) => 'N',
      chr(195) . chr(146) => 'O', chr(195) . chr(147) => 'O',
      chr(195) . chr(148) => 'O', chr(195) . chr(149) => 'O',
      chr(195) . chr(150) => 'O', chr(195) . chr(153) => 'U',
      chr(195) . chr(154) => 'U', chr(195) . chr(155) => 'U',
      chr(195) . chr(156) => 'U', chr(195) . chr(157) => 'Y',
      chr(195) . chr(159) => 's', chr(195) . chr(160) => 'a',
      chr(195) . chr(161) => 'a', chr(195) . chr(162) => 'a',
      chr(195) . chr(163) => 'a', chr(195) . chr(164) => 'a',
      chr(195) . chr(165) => 'a', chr(195) . chr(167) => 'c',
      chr(195) . chr(168) => 'e', chr(195) . chr(169) => 'e',
      chr(195) . chr(170) => 'e', chr(195) . chr(171) => 'e',
      chr(195) . chr(172) => 'i', chr(195) . chr(173) => 'i',
      chr(195) . chr(174) => 'i', chr(195) . chr(175) => 'i',
      chr(195) . chr(177) => 'n', chr(195) . chr(178) => 'o',
      chr(195) . chr(179) => 'o', chr(195) . chr(180) => 'o',
      chr(195) . chr(181) => 'o', chr(195) . chr(182) => 'o',
      chr(195) . chr(182) => 'o', chr(195) . chr(185) => 'u',
      chr(195) . chr(186) => 'u', chr(195) . chr(187) => 'u',
      chr(195) . chr(188) => 'u', chr(195) . chr(189) => 'y',
      chr(195) . chr(191) => 'y',
      // Decompositions for Latin Extended-A
      chr(196) . chr(128) => 'A', chr(196) . chr(129) => 'a',
      chr(196) . chr(130) => 'A', chr(196) . chr(131) => 'a',
      chr(196) . chr(132) => 'A', chr(196) . chr(133) => 'a',
      chr(196) . chr(134) => 'C', chr(196) . chr(135) => 'c',
      chr(196) . chr(136) => 'C', chr(196) . chr(137) => 'c',
      chr(196) . chr(138) => 'C', chr(196) . chr(139) => 'c',
      chr(196) . chr(140) => 'C', chr(196) . chr(141) => 'c',
      chr(196) . chr(142) => 'D', chr(196) . chr(143) => 'd',
      chr(196) . chr(144) => 'D', chr(196) . chr(145) => 'd',
      chr(196) . chr(146) => 'E', chr(196) . chr(147) => 'e',
      chr(196) . chr(148) => 'E', chr(196) . chr(149) => 'e',
      chr(196) . chr(150) => 'E', chr(196) . chr(151) => 'e',
      chr(196) . chr(152) => 'E', chr(196) . chr(153) => 'e',
      chr(196) . chr(154) => 'E', chr(196) . chr(155) => 'e',
      chr(196) . chr(156) => 'G', chr(196) . chr(157) => 'g',
      chr(196) . chr(158) => 'G', chr(196) . chr(159) => 'g',
      chr(196) . chr(160) => 'G', chr(196) . chr(161) => 'g',
      chr(196) . chr(162) => 'G', chr(196) . chr(163) => 'g',
      chr(196) . chr(164) => 'H', chr(196) . chr(165) => 'h',
      chr(196) . chr(166) => 'H', chr(196) . chr(167) => 'h',
      chr(196) . chr(168) => 'I', chr(196) . chr(169) => 'i',
      chr(196) . chr(170) => 'I', chr(196) . chr(171) => 'i',
      chr(196) . chr(172) => 'I', chr(196) . chr(173) => 'i',
      chr(196) . chr(174) => 'I', chr(196) . chr(175) => 'i',
      chr(196) . chr(176) => 'I', chr(196) . chr(177) => 'i',
      chr(196) . chr(178) => 'IJ', chr(196) . chr(179) => 'ij',
      chr(196) . chr(180) => 'J', chr(196) . chr(181) => 'j',
      chr(196) . chr(182) => 'K', chr(196) . chr(183) => 'k',
      chr(196) . chr(184) => 'k', chr(196) . chr(185) => 'L',
      chr(196) . chr(186) => 'l', chr(196) . chr(187) => 'L',
      chr(196) . chr(188) => 'l', chr(196) . chr(189) => 'L',
      chr(196) . chr(190) => 'l', chr(196) . chr(191) => 'L',
      chr(197) . chr(128) => 'l', chr(197) . chr(129) => 'L',
      chr(197) . chr(130) => 'l', chr(197) . chr(131) => 'N',
      chr(197) . chr(132) => 'n', chr(197) . chr(133) => 'N',
      chr(197) . chr(134) => 'n', chr(197) . chr(135) => 'N',
      chr(197) . chr(136) => 'n', chr(197) . chr(137) => 'N',
      chr(197) . chr(138) => 'n', chr(197) . chr(139) => 'N',
      chr(197) . chr(140) => 'O', chr(197) . chr(141) => 'o',
      chr(197) . chr(142) => 'O', chr(197) . chr(143) => 'o',
      chr(197) . chr(144) => 'O', chr(197) . chr(145) => 'o',
      chr(197) . chr(146) => 'OE', chr(197) . chr(147) => 'oe',
      chr(197) . chr(148) => 'R', chr(197) . chr(149) => 'r',
      chr(197) . chr(150) => 'R', chr(197) . chr(151) => 'r',
      chr(197) . chr(152) => 'R', chr(197) . chr(153) => 'r',
      chr(197) . chr(154) => 'S', chr(197) . chr(155) => 's',
      chr(197) . chr(156) => 'S', chr(197) . chr(157) => 's',
      chr(197) . chr(158) => 'S', chr(197) . chr(159) => 's',
      chr(197) . chr(160) => 'S', chr(197) . chr(161) => 's',
      chr(197) . chr(162) => 'T', chr(197) . chr(163) => 't',
      chr(197) . chr(164) => 'T', chr(197) . chr(165) => 't',
      chr(197) . chr(166) => 'T', chr(197) . chr(167) => 't',
      chr(197) . chr(168) => 'U', chr(197) . chr(169) => 'u',
      chr(197) . chr(170) => 'U', chr(197) . chr(171) => 'u',
      chr(197) . chr(172) => 'U', chr(197) . chr(173) => 'u',
      chr(197) . chr(174) => 'U', chr(197) . chr(175) => 'u',
      chr(197) . chr(176) => 'U', chr(197) . chr(177) => 'u',
      chr(197) . chr(178) => 'U', chr(197) . chr(179) => 'u',
      chr(197) . chr(180) => 'W', chr(197) . chr(181) => 'w',
      chr(197) . chr(182) => 'Y', chr(197) . chr(183) => 'y',
      chr(197) . chr(184) => 'Y', chr(197) . chr(185) => 'Z',
      chr(197) . chr(186) => 'z', chr(197) . chr(187) => 'Z',
      chr(197) . chr(188) => 'z', chr(197) . chr(189) => 'Z',
      chr(197) . chr(190) => 'z', chr(197) . chr(191) => 's',
      // Euro Sign
      chr(226) . chr(130) . chr(172) => 'E'
    );
  }

  // transliterate
  return strtr($string, $charmap);
}

/**
 * Get a list of all domains.
 */
function culturefeed_search_get_domains() {

  $domains = drupal_static(__FUNCTION__, NULL);
  if ($domains !== NULL) {
    return $domains;
  }

  $result = db_query('SELECT did, label FROM {culturefeed_search_domains}');
  $domains = array();
  foreach ($result as $row) {
    $domains[$row->did] = $row->label;
  }

  return $domains;

}

/**
 * Get a list of all categories for a given domain.
 * @param string $domain
 *   Domain to get.
 * @param $conditions
 *   Keyed array of extra conditions for the query. Ex array('tid' => 12.1.0.0.0)
 */
function culturefeed_search_get_categories_by_domain($domain, $conditions = array()) {

  $query = db_select('culturefeed_search_terms', 'cst');
  $query->fields('cst', array('tid', 'name'));
  $query->condition('cst.did', $domain);

  // Add extra conditions.
  if (!empty($conditions['tid'])) {
    foreach ($conditions as $field => $value) {
      $query->condition($field, $value);
    }
  }

  $result = $query->execute();
  $categories = array();
  foreach ($result as $row) {
    $categories[$row->tid] = $row->name;
  }

  return $categories;

}

/**
 * Get all available actortypes.
 */
function culturefeed_search_get_actortype_categories() {
  culturefeed_search_get_categories_by_domain('actortype');
}

/**
 * Retrieves a list of categories for the educationfield domain.
 * @param $conditions
 *   Keyed array of extra conditions for the query. Ex array('tid' => 12.1.0.0.0)
 */
function culturefeed_search_get_educationfield_categories($conditions = array()) {
  return culturefeed_search_get_categories_by_domain('educationfield', $conditions);
}

/**
 * Retrieves a list of categories for the educationlevel domain.
 * @param $conditions
 *   Keyed array of extra conditions for the query. Ex array('tid' => 12.1.0.0.0)
 */
function culturefeed_search_get_educationlevel_categories($conditions = array()) {
  return culturefeed_search_get_categories_by_domain('educationlevel', $conditions);
}

/**
 * Retrieves a list of categories for the flandersregion domain.
 *
 * @param string $parent_operator
 *   Indicates what operator should be used to load the regions. Can be '', != or =.
 *   = means, load all the regions without a parent.
 *   != means load all the regions that have a parent.
 *   If false, only subelements will be fetched.
 * @param $conditions
 *   Keyed array of extra conditions for the query. Ex array('tid' => 12.1.0.0.0)
 *
 */
function culturefeed_search_get_flandersregion_categories($parent_operator = '', $conditions = array()) {

  $query = db_select('culturefeed_search_terms', 'cst');
  $query->fields('cst', array('tid', 'name'));
  $query->condition('cst.did', 'flandersregion');

  if (!empty($parent_operator)) {
    $query->condition('cst.parent', '', $parent_operator);
  }

  if (!empty($conditions['tid'])) {
    foreach ($conditions as $field => $value) {
      if (is_array($value)) {
        $query->condition($field, $value, 'IN');
      }
      else {
        $query->condition($field, $value);
      }
    }
  }
  $result = $query->execute();
  $categories = array();
  foreach ($result as $row) {
    $categories[$row->tid] = $row->name;
  }

  return $categories;

}

/**
 * Retrieves a list of flanders regions as suggestion.
 */
function culturefeed_search_get_flandersregion_suggestions($search_string, $provinces_only = FALSE) {

  $parts = array_reverse(explode(",", $search_string));
  $search_string = trim(array_shift($parts));
  $old_parts = count($parts) > 0 ? (implode(", ", $parts) . ', ') : '';


  $query = db_select('culturefeed_search_terms', 'cst');
  $query->fields('cst', array('tid', 'name'));
  $query->condition('did', 'flandersregion');
  $query->condition('name', '%' . db_like($search_string) . '%', 'LIKE');
  $query->range(0, 10);

  if ($provinces_only) {
    $query->condition('parent', '');
  }

  // Order by parent, so that provinces appear at top of list.
  $query->orderBy('parent', 'ASC');

  $result = $query->execute();
  $suggestions = array();
  foreach ($result as $row) {
    $suggestions[$old_parts . $row->name] = $row->name;
  }

  return $suggestions;

}

/**
 * Retrieves a list of flanders regions as suggestion.
 * @param string $search_string String to search for.
 * @param bool $include_regions
 *   Include also the regions in the suggestions, or not.
 */
function culturefeed_search_get_city_suggestions($search_string, $include_regions = FALSE) {

  $suggestions = array();
  if ($include_regions && !is_numeric($search_string)) {
    $region_suggestions = culturefeed_search_get_flandersregion_suggestions($search_string, FALSE);
    foreach ($region_suggestions as $region) {
      $suggestions[$region] = $region;
    }
  }

  $limit = 10 - count($suggestions);
  $result = db_query("SELECT name, zip FROM {culturefeed_search_cities}
      WHERE name LIKE :search OR zip LIKE :search LIMIT " . $limit, array(':search' => "%" . db_like($search_string) . "%"));

  foreach ($result as $row) {
    $suggestions[$row->zip . ' ' . $row->name] = $row->name . ' (' . $row->zip . ')';
  }

  return $suggestions;

}

/**
 * Retrieves a list of categories for the educationlevel domain.
 */
function culturefeed_search_get_educationlevel_categories_tree() {

  $query = db_select('culturefeed_search_terms', 'cst');
  $query->fields('cst', array('tid', 'name', 'parent'));
  $query->condition('cst.did', 'educationlevel');
  $result = $query->execute();

  // Prepare the result.
  $rows = array();
  foreach ($result as $row) {
    $rows[] = (object) array('tid' => $row->tid, 'name' => $row->name, 'parent' => $row->parent);
  }

  $categories = array();
  // First level.
  foreach ($rows as $row) {
    if (empty($row->parent)) {
      $categories[$row->tid] = (object) array('tid' => $row->tid, 'parent' => $row->parent, 'name' => $row->name, 'children' => array());
    }
  }

  // Second level.
  foreach ($rows as $row) {
    if (!empty($row->parent) && isset($categories[$row->parent])) {
      $categories[$row->parent]->children[$row->tid] = (object) array('tid' => $row->tid, 'name' => $row->name, 'parent' => $row->parent);
    }
  }

  return $categories;

}
