<?php
use CultuurNet\Search\Parameter\Parameter;

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
 * @param $type
 *   Type of item to load (example event)
 * @return mixed
 *   FALSE if not found, otherwise CultuurNet\Search\ActivityStatsExtendedEntity.
 */
function culturefeed_search_item_load($cdb_id, $type = '', $watchdog = TRUE) {

  $items = &drupal_static(__FUNCTION__, array());

  if (isset($items[$cdb_id])) {
    return $items[$cdb_id];
  }

  if (empty($type)) {

    $parameters = array();
    $parameters[] = new \CultuurNet\Search\Parameter\Group();
    $parameters[] = new \CultuurNet\Search\Parameter\Parameter("past", "true");
    $parameters[] = new \CultuurNet\Search\Parameter\Query('cdbid:' . $cdb_id);

    try {
      $result = culturefeed_get_search_service()->search($parameters);
      $search_item = $result->getItems();
      $items[$cdb_id] = reset($search_item);
      return $items[$cdb_id];
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_search', $e);
    }
  }
  else {
    try {
      $items[$cdb_id] = culturefeed_get_search_service()->detail($type, $cdb_id);
      return $items[$cdb_id];
    }
    catch (Exception $e) {
      if ($watchdog) {
        watchdog_exception('culturefeed_search', $e);
      }
    }
  }

  return FALSE;

}

/**
 * Search a cdb item by title and type.
 * @param string $title
 *  Title to search.
 * @param string $type
 *  Type to search
 * @return mixed
 *   FALSE if not found, otherwise CultuurNet\Search\ActivityStatsExtendedEntity
 */
function culturefeed_search_item_load_by_title($title, $type) {

  $search_string = trim($title);
  $parameters = array();
  $parameters[] = new \CultuurNet\Search\Parameter\Query('title:"' . $search_string . '"');
  $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery('type:actor');
  $parameters[] = new \CultuurNet\Search\Parameter\Group();
  $result = culturefeed_get_search_service()->search($parameters);

  if ($result->getTotalCount()  > 0) {
    $items = $result->getItems();
    return $items[0];
  }

  return FALSE;

}

/**
 * Get the current active search object.
 * @param $path
 *   Path to load the active object of. Defaults to current path.
 */
function culturefeed_search_get_active_object($path = NULL) {

  $first_argument = arg(0, $path);
  $second_argument = arg(1, $path);

  // Agenda items (events, actors, productions) + pages
  if ($first_argument == 'agenda') {

    if ($second_argument == 'g') {
      return menu_get_object('culturefeed_pages_page', 3, $path);
    }
    elseif ($second_argument == 'e' || $second_argument == 'p' || $second_argument == 'a') {
      return culturefeed_agenda_get_active_object($path);
    }

  }
  // Bibnet books
  elseif ($first_argument == 'book') {
    return menu_get_object('bibnet_book', 1, $path);
  }
  // Generic fallback.
  elseif ($first_argument == 'userpoints' && $second_argument == 'promotion') {
    $object = menu_get_object('culturefeed_userpoints_promotion', 2);
    if ($object) {
      return GenericContentExtendedEntity::createFrom($object);
    }
  }
  // Drupal nodes.
  else {
    $node = menu_get_object('node', 1, $path);
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

    case 'news':
      $url_prefix = 'news';
    break;

    case 'node':
      return 'node/' . $cdbid;

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
  $query->condition('cst.show_term', 1);

  // Add parent condition.
  if (!empty($conditions['parent'])) {
    $db_or = db_or();
    $db_or->condition('p1', $conditions['parent'], '=');
    $db_or->condition('p2', $conditions['parent'], '=');
    $db_or->condition('p3', $conditions['parent'], '=');
    $db_or->condition('p4', $conditions['parent'], '=');
    $query->condition($db_or);
  }

  // Add extra conditions.
  if (!empty($conditions['tid'])) {
    if (is_array($conditions['tid'])) {
      $db_or = db_or();
      foreach ($conditions['tid'] as $field => $value) {
        $db_or->condition('tid', $value, '=');
      }
      $query->condition($db_or);
    }
    else {
      $query->condition('tid', $conditions['tid'], '=');
    }
  }

  if (!empty($conditions['tid_like'])) {
    if (is_array($conditions['tid_like'])) {
      $db_or = db_or();
      foreach ($conditions['tid_like'] as $field => $value) {
        $db_or->condition('tid', $value . '%', 'like');
      }
      $query->condition($db_or);
    }
    else {
      $query->condition('tid', $conditions['tid_like'] . '%', 'like');
    }
  }

  if (!empty($conditions['name_like'])) {
    if (is_array($conditions['name_like'])) {
      $db_or = db_or();
      foreach ($conditions['name_like'] as $field => $value) {
        $query->condition('name', '%' . $value . '%', 'like');
      }
      $query->condition($db_or);
    }
    else {
      $query->condition('name', '%' . $conditions['name_like'] . '%', 'like');
    }
  }

  if (empty($conditions['language'])) {
    $conditions['language'] = culturefeed_search_get_preferred_language();
  }
  $query->condition('language', $conditions['language']);

  $result = $query->execute();
  $categories = array();
  foreach ($result as $row) {
    $categories[$row->tid] = $row->name;
  }

  return $categories;

}

/**
 * Retrieves a list of categories for the eventtype domain.
 * @param $conditions
 *   Keyed array of extra conditions for the query. Ex array('tid' => 12.1.0.0.0)
 */
function culturefeed_search_get_eventtype_categories($conditions = array()) {
  return culturefeed_search_get_categories_by_domain('eventtype', $conditions);
}

/**
 * Retrieves a list of categories for the theme domain.
 * @param $conditions
 *   Keyed array of extra conditions for the query. Ex array('tid' => 12.1.0.0.0)
 */
function culturefeed_search_get_theme_categories($conditions = array()) {
  return culturefeed_search_get_categories_by_domain('theme', $conditions);
}

/**
 * Get all available actortypes.
 * @param $conditions
 *   Keyed array of extra conditions for the query. Ex array('tid' => 12.1.0.0.0)
 */
function culturefeed_search_get_actortype_categories($conditions = array()) {
  return culturefeed_search_get_categories_by_domain('actortype', $conditions);
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
 * Retrieves a list of categories for the workingregion domain.
 *
 * @param $conditions
 *   Keyed array of extra conditions for the query. Ex array('tid' => 12.1.0.0.0)
 *
 */
function culturefeed_search_get_workingregion_categories($conditions = array()) {
 return culturefeed_search_get_categories_by_domain('workingregion', $conditions);
}

/**
 * Retrieves a list of categories for the flandersregion domain.
 *
 * @param $conditions
 *   Keyed array of extra conditions for the query. Ex array('tid' => 12.1.0.0.0)
 *
 */
function culturefeed_search_get_flandersregion_categories($conditions = array()) {

  $query = db_select('culturefeed_search_terms', 'cst');
  $query->fields('cst', array('tid', 'name'));
  $query->condition('cst.did', 'flandersregion');
  $query->condition('cst.language', culturefeed_search_get_preferred_language());

  if (!empty($conditions)) {
    foreach ($conditions as $field => $value) {
      if (is_array($value)) {
        $query->condition($field, $value, 'IN');
      }
      else {
        if ($value === NULL) {
          $query->isNull($field);
        }
        else {
          $query->condition($field, $value);
        }
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
function culturefeed_search_get_flandersregion_suggestions($search_string, $include_regions = FALSE, $parents = array()) {

  $postal_search = (is_numeric($search_string) && strlen($search_string) >= 3) ? TRUE : FALSE;

  $parts = array_reverse(explode(",", $search_string));
  $search_string = trim(array_shift($parts));
  $old_parts = count($parts) > 0 ? (implode(", ", $parts) . ', ') : '';

  $query = db_select('culturefeed_search_terms', 'cst');
  $query->fields('cst', array('tid', 'name', 'p3'));
  $query->condition('did', 'flandersregion');
  //$query->condition('language', culturefeed_search_get_preferred_language());
  $query->condition('language', 'und');

  if ($postal_search) {
    $query->condition('name', db_like($search_string) . '%', 'LIKE');
  }
  else {
    $query->condition('name', '%' . db_like($search_string) . '%', 'LIKE');
    $query->condition('name', '%(' . db_like($search_string) . '%)', 'NOT LIKE');
  }

  if(!$include_regions) {
    $query->isNotNull('p1');
    $query->isNotNull('p2');
    $query->isNotNull('p3');
  }
  // If parents is set add condition.
  if (count($parents)) {
    $or = db_or();
    $or->condition('p1', $parents, 'IN');
    $or->condition('p2', $parents, 'IN');
    $query->condition($or);
  }
  //$query->range(0, 20);

  // Order by parent, so that provinces appear at top of list.
  $query->orderBy('parent', 'ASC');

  $result = $query->execute();

  $suggestions = array();
  $postal_regions = array();
  foreach ($result as $row) {
    $suggestions[$old_parts . $row->name] = $row->name;

    //When searching on postal and a full postal is entered, add the parent (if not added yet).
    if ($postal_search && !isset($postal_regions[$row->p3])) {
      $parent_query = db_select('culturefeed_search_terms', 'cst');
      $parent_query->fields('cst', array('tid', 'name'));
      $parent_query->condition('did', 'flandersregion');
      $parent_query->condition('language', 'und');
      $parent_query->condition('tid', $row->p3);
      $postal_regions[$row->p3] = $parent_query->execute()->fetch();
      $suggestions[] = $postal_regions[$row->p3]->name;
    }
  }

  foreach ($suggestions as $key => $suggestion) {
    if (strtolower($suggestion) == strtolower($search_string)) {
      $val = $suggestion;
      unset($suggestion);
      $suggestions = array($key => $val) + $suggestions;
    }
  }

  return $suggestions;

}

/**
 * Retrieves a list of cities as suggestion.
 * @param string $search_string String to search for.
 * @param bool $include_regions
 *   Include also the cities in the suggestions, or not.
 */
function culturefeed_search_get_city_suggestions($search_string, $include_regions = FALSE, $parents = array()) {

  $suggestions = array();
  $parts = array_reverse(explode(",", $search_string));
  $search_string = trim(array_shift($parts));
  $old_parts = count($parts) > 0 ? (trim(implode(", ", $parts)) . ', ') : '';

  $region_suggestions = culturefeed_search_get_flandersregion_suggestions($search_string, $include_regions, $parents);

  // Fix for UIV-1403 order by match of $search_string.
  // Change the structure of the $city_suggestions array.
  // Add stripos element to it.
  foreach ($region_suggestions as $key => $region_order_suggestion) {
    unset($region_suggestions[$key]);
    $region_suggestions[$key]['name'] = $region_order_suggestion;
    $region_suggestions[$key]['stripos'] = stripos($region_order_suggestion, $search_string);
  }

  // Order the $city_suggestions array by its element stripos.
  foreach ($region_suggestions as $key => $row) {
    $stripos[$key]  = $row['stripos'];
  }
  if (isset($stripos)) {
    array_multisort($stripos, SORT_ASC, $region_suggestions);
  }

  $region_suggestions = array_slice($region_suggestions, 0, 20);

  foreach ($region_suggestions as $region) {
    $suggestions[$old_parts . $region['name']] = $region['name'];
  }

  return $suggestions;

}

/**
 * Retrieves a list of categories for the educationlevel domain.
 */
function culturefeed_search_get_educationlevel_categories_tree() {

  $query = db_select('culturefeed_search_terms', 'cst');
  $query->fields('cst', array('tid', 'name', 'p1'));
  $query->condition('cst.did', 'educationlevel');
  $result = $query->execute();

  // Prepare the result.
  $rows = array();
  foreach ($result as $row) {
    $rows[] = (object) array('tid' => $row->tid, 'name' => $row->name, 'p1' => $row->p1);
  }

  // We know educationlevel only has two levels ...

  $categories = array();
  // First level.
  foreach ($rows as $row) {
    if (empty($row->p1)) {
      $categories[$row->tid] = (object) array('tid' => $row->tid, 'p1' => $row->p1, 'name' => $row->name, 'children' => array());
    }
  }

  // Second level.
  foreach ($rows as $row) {
    if (!empty($row->p1) && isset($categories[$row->p1])) {
      $categories[$row->p1]->children[$row->tid] = (object) array('tid' => $row->tid, 'name' => $row->name, 'p1' => $row->p1);
    }
  }

  return $categories;

}

/**
 * Get a category by slug.
 */
function culturefeed_search_get_category_by_slug($slug, $domain = '') {

  $query = db_select('culturefeed_search_terms', 'ct');
  $query->condition('slug', $slug);
  $query->range(0, 1);
  $query->addField('ct', 'tid');
  $query->addField('ct', 'name');
  $query->addField('ct', 'p1');
  $query->addField('ct', 'p2');
  $query->addField('ct', 'p3');
  $query->addField('ct', 'p4');
  if (!empty($domain)) {
    $query->condition('did', $domain);
  }

  return $query->execute()->fetchObject();

}

/**
 * Get a category by name.
 */
function culturefeed_search_get_category_by_name($name, $domain = '') {

  $query = db_select('culturefeed_search_terms', 'ct');
  $query->condition('name', $name);
  $query->range(0, 1);
  $query->addField('ct', 'tid');
  $query->addField('ct', 'slug');
  $query->addField('ct', 'p1');
  $query->addField('ct', 'p2');
  $query->addField('ct', 'p3');
  $query->addField('ct', 'p4');
  if (!empty($domain)) {
    $query->condition('did', $domain);
  }

  return $query->execute()->fetchObject();

}

/**
 * Get a city by slug.
 */
function culturefeed_search_get_city_by_slug($slug) {
  return db_query('SELECT name, zip FROM {culturefeed_search_cities}
    WHERE slug = :slug', array(':slug' => $slug))->fetchObject();
}

/**
 * Returns the preferred language of the user.
 */
function culturefeed_search_get_preferred_language() {

  // Use the drupal language.
  return $GLOBALS['language']->language;

  // TODO make a setting of this...
  // If culturefeed is enabled, we can use the user his language.
  if (module_exists('culturefeed')) {
    return culturefeed_get_preferred_language();
  }


}

/**
 * Get a term by id.
 */
function culturefeed_search_get_term_by_id($id, $lang = NULL) {

  if (empty($lang)) {
    $lang = culturefeed_search_get_preferred_language();
  }

  return db_select('culturefeed_search_terms', 't')
    ->condition('tid', $id)
    ->condition('language', array($lang), 'IN')
    ->fields('t')
    ->execute()->fetchObject();

}

/**
 * Returns a list of terms to translate.
 *
 * @param array $tids
 *   (optional) A collection of tids to get translations from.
 * @param boolean $return_tids
 *   (optional) Whether to return the static $terms or not.
 *
 * @return
 *   - TRUE if there are translated terms, FALSE if not
 *   - A collection of tids keyed by the undefined and the user preferred language.
 *     in case $return_tids is set to TRUE.
 */
function culturefeed_search_term_translations($tids = array(), $return_tids = FALSE) {

  static $terms = array();

  // Check what tids are already known.
  foreach ($tids as $key => $tid) {
    if (isset($terms[$tid])) {
      unset($tids[$key]);
    }
  }

  if (!empty($tids)) {
    $translations = db_select('culturefeed_search_terms', 'cst')
      ->fields('cst', array('tid', 'name', 'language'))
      ->condition('tid', $tids, 'IN')
      ->condition('language', array('und', culturefeed_search_get_preferred_language()), 'IN')
      ->execute();
    foreach ($translations as $record) {
      if (!isset($terms[$record->tid])) {
        $terms[$record->tid] = array();
      }
      $terms[$record->tid][$record->language] = $record->name;
    }
  }

  // Return the static terms.
  if ($return_tids) {
    return $terms;
  }

  // Return whether we have translations currently or not.
  return !empty($terms) ? TRUE : FALSE;
}

/**
 * Get the agefrom querystring to be set active.
 *
 * @param string $facet_value
 *   The facet value.
 *
 * @return int
 *   The key.
 */
function culturefeed_search_get_agefrom_value($facet_value) {

  if (module_exists('culturefeed_search_ui')) {

    $options = culturefeed_search_ui_get_age_range_facet_options();
    foreach ($options as $key => $option) {
      if ($option['range'] == $facet_value) {
        return $key;
      }
    }

  }

}

/**
 * Get the translation for a term.
 *
 * @param string $tid
 *   The id of the term.
 *
 * @return string
 *   The label of the translated term.
 */
function culturefeed_search_get_term_translation($tid) {

  $terms = culturefeed_search_term_translations(array($tid), TRUE);

  if (!isset($terms[$tid])) {
    return $tid;
  }

  if (isset($terms[$tid][culturefeed_search_get_preferred_language()])) {
    return $terms[$tid][culturefeed_search_get_preferred_language()];
  }
  else {
    return $terms[$tid][LANGUAGE_NONE];
  }

}

/**
 * Translate the items of a facet.
 */
function culturefeed_search_translate_facet(&$facet_items) {

  $tids = array();
  // Construct array of ids to query all translations at once.
  foreach ($facet_items as $facet_item) {
    $tids[] = $facet_item->getValue();
    // Add the subitems from this facet item.
    if ($facet_item->hasSubItems()) {
      $sub_items = $facet_item->getSubItems();
      foreach ($sub_items as $sub_item) {
        $tids[] = $sub_item->getValue();
      }
    }
  }

  // Translate all the facets.
  culturefeed_search_term_translations($tids);

  // Loop through again to translate.
  foreach ($facet_items as $facet_item) {
    $facet_item->setLabel(culturefeed_search_get_term_translation($facet_item->getValue()));
    // Add the subitems from this facet item.
    if ($facet_item->hasSubItems()) {
      $sub_items = $facet_item->getSubItems();
      foreach ($sub_items as $key => $sub_item) {
        if ($sub_item->getLabel() == culturefeed_search_get_term_translation($sub_item->getValue())) {
          $sub_item->setLabel(culturefeed_search_get_term_translation($sub_item->getValue()));
        }
        else {
          $facet_item->removeSubItem($key);
        }
      }
    }
  }
}

/**
 * Prepare a block to be loaded via ajax.
 * @param array $config
 *   Config for the ajax.
 */
function culturefeed_search_block_ajaxload($config) {

  $block['subject'] = $config['title'];
  $block['content'] = array(
    '#markup' => '<div id="' . $config['id'] . '">' . t('Loading') . '</div>',
    '#id' => $config['id'],
    '#ajax' => array(
      'event' => 'ajaxload',
      'path' => $config['path'],
    ),
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'culturefeed_search') . '/js/culturefeed_search_ajax.js',
        array('data' => array(), 'type' => 'setting'),
      ),
    ),
  );

  $block['content'] = ajax_pre_render_element($block['content']);

  return $block;
}

/**
 * Check if we are connected to acceptance API.
 */
function culturefeed_search_acceptance_mode() {

  // Get culturefeed_search_api_location
  $location = variable_get('culturefeed_search_api_location');
  if (strpos($location,'www') === FALSE) {
    return TRUE;
  }

}

/**
 * Set the "noindex, follow" meta tag when needed.
 */
function culturefeed_search_set_noindex_metatag() {

  $element = array(
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'robots',
      'content' => 'noindex, follow',
    ),
  );

  drupal_add_html_head($element, 'robots');

}
