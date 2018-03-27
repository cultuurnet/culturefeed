<?php

/**
 * @file
 * Helper functions for the culturefeed content module.
 */

/**
 * Add a category row to the query builder categories field.
 *
 * @param $id
 * @param array $values
 *
 * @return string
 */
function _culturefeed_content_add_builder_category_row($element, $field_name_base, $id = 0, $values = [], $composite_query_field_id) {
  // Check values for defaults.
  $default_type = (isset($values['type']) ? $values['type'] : 'all');
  $default_equal = (isset($values['equal']) ? $values['equal'] : 0);
  $default_ids = (isset($values['ids']) ? $values['ids'] : '');

  // Build category type options.
  $selectable_categories = ['eventtype', 'theme', 'actortype', 'facility'];
  $domains = culturefeed_search_get_domains();
  $category_type_options = ['all' => t('All')];

  foreach ($selectable_categories as $category) {
    $category_type_options[$category] = $domains[$category];
  }

  $element['wrapper']['what']["what_type_$id"] = array(
    '#type' => 'select',
    '#options' => $category_type_options,
    '#default_value' => $default_type,
    '#ajax' => array(
      'callback' => 'culturefeed_content_field_ajax_rebuild_composite_query',
      'wrapper' => $composite_query_field_id,
      'progress' => array('type' => 'none')
    ),
  );

  $element['wrapper']['what']["what_equal_$id"] = array(
    '#type' => 'select',
    '#options' => [
      t('is equal to'),
      t('is not equal to')
    ],
    '#default_value' => $default_equal,
    '#states' => array(
      'invisible' => array(
        array(
          ':input[name="' . $field_name_base . '[wrapper][what][what_type_' . $id . ']"]' => array('value' => 'all'),
        ),
      ),
    ),
    '#ajax' => array(
      'callback' => 'culturefeed_content_field_ajax_rebuild_composite_query',
      'wrapper' => $composite_query_field_id,
      'progress' => array('type' => 'none')
    ),
  );

  // Build select fields for each category.
  foreach ($selectable_categories as $category) {
    $category_options = culturefeed_search_get_categories_by_domain($category);

    $element['wrapper']['what']["what_$category" . "_$id"] = array(
      '#type' => 'select',
      '#options' => $category_options,
      '#multiple' => TRUE,
      '#validated' => TRUE, // Fix for multiselect + ajax issue
      '#states' => array(
        'visible' => array(
          array(
            ':input[name="' . $field_name_base . '[wrapper][what][what_type_' . $id . ']"]' => array('value' => $category),
          ),
        ),
      ),
      '#ajax' => array(
        'callback' => 'culturefeed_content_field_ajax_rebuild_composite_query',
        'wrapper' => $composite_query_field_id,
        'progress' => array('type' => 'none')
      ),
    );

    if ($category == $default_type && $default_type !== 'all') {
      $element['wrapper']['what']["what_$category" . "_$id"]['#default_value'] = $default_ids;
    }
  }

  return $element;
}

/**
 * Unserialize categories form values and convert to custom row value arrays.
 *
 * @param $serialized_values
 *
 * @return array
 */
function _culturefeed_content_convert_serialized_categories_to_rows ($serialized_values) {
  $rows = [];

  $values = unserialize($serialized_values);

  for ($i = 0; $i >= 0; $i++) {
    if (isset($values["what_type_$i"])) {
      if ($values["what_type_$i"] !== 'all') {

        switch ($values["what_type_$i"]) {
          case 'eventtype':
            $ids = $values["what_eventtype_$i"];
            break;
          case 'theme':
            $ids = $values["what_theme_$i"];
            break;
          case 'actortype':
            $ids = $values["what_actortype_$i"];
            break;
          case 'facility':
            $ids = $values["what_facility_$i"];
            break;
          default:
            $ids = [];
            break;
        }

        $rows[] = [
          'type' => $values["what_type_$i"],
          'equal' => $values["what_equal_$i"],
          'ids' => $ids
        ];
      }
      else {
        $rows[] = [];
      }
    }
    else {
      break;
    }
  }

  return $rows;
}

/**
 * Build an advanced query string based on the current builder values.
 *
 * @param $values
 *
 * @return string
 */
function _culturefeed_content_build_advanced_query($values) {

  $query_param = !empty($values['query_string']) ? _culturefeed_search_ui_sanitize_query_term($values['query_string']) : '*:*';
  $advanced_query = 'q=' . $query_param;

  // Types.
  $types = isset($values['types']) && !empty($values['types']) ? array_filter(explode(',', $values['types'])) : null;
  if (empty($types)) {
    $types = ['event'];
  }

  $type_filters = '';

  foreach ($types as $type) {
    if (!empty($type_filters)) {
      $type_filters .= ' OR ';
    }
    $type_filters .= "type:$type";
  }

  $advanced_query .= "%26fq=($type_filters)";

  if (!empty($values['filter_query'])) {
    $advanced_query .= '%26fq=(' . $values['filter_query'] . ')';
  }

  // Categories query.
  if (!empty($values['what_categories'])) {
    $advanced_query .= _culturefeed_content_build_categories_query(unserialize($values['what_categories']));
  }

  // Where.
  if (!empty($values['where_type'])) {
    switch ($values['where_type']) {
      case 'region':
        $region = $values['where_region'];
        if ($region !== 'all') {
          $advanced_query .= $values['where_equal'] ? "%26fq=(NOT category_flandersregion_id:$region)" : "%26fq=(category_flandersregion_id:$region)";
        }
        break;
      case 'city':
        $city = $values['where_city'];
        if ($city !== 'all') {
          $advanced_query .= $values['where_equal'] ? "%26fq=(NOT category_flandersregion_id:$city)" : "%26fq=(category_flandersregion_id:$city)";
        }
        break;
      case 'postal_code':
        $postal_code = $values['where_postal_code'];
        $radius = $values['where_postal_code_radius'] . 'km';
        $advanced_query .= $values['where_equal'] ? "%26fq=(NOT zipcode:$postal_code)" : "%26zipcode=$postal_code!$radius";
        break;
    }
  }

  // When.
  if (empty($values['when_date_1'])) {
    $values['when_date_1'] = NULL;
  }
  if (empty($values['when_date_2'])) {
    $values['when_date_2'] = NULL;
  }

  if ($values['when_date_1'] || $values['when_date_2']) {
    // To make the checking easier, we set type to 'all' if there is no corresponding date value yet.
    $values['when_type_1'] = $values['when_date_1'] ? $values['when_type_1'] : 'all';
    $values['when_type_2'] = $values['when_date_2'] ? $values['when_type_2'] : 'all';

    $combined_when_type = $values['when_type_1'] . '|' . $values['when_type_2'];

    // 0 = before, 1 = after.
    $ba_1 = $values['when_before_after_1'];
    $ba_2 = $values['when_before_after_2'];

    $date_1 = $values['when_date_1'] ? new DateTime($values['when_date_1']) : null;
    $date_2 = $values['when_date_2'] ? new DateTime($values['when_date_2']) : null;

    switch ($combined_when_type) {
      case 'startdate|all':
      case 'enddate|all':
        $advanced_query .= $ba_1 ? _culturefeed_content_create_date_filter_param($values['when_type_1'], $date_1) : _culturefeed_content_create_date_filter_param($values['when_type_1'], null, $date_1);
        break;
      case 'all|startdate':
      case 'all|enddate':
        $advanced_query .= $ba_2 ? _culturefeed_content_create_date_filter_param($values['when_type_2'], $date_2) : _culturefeed_content_create_date_filter_param($values['when_type_2'], null, $date_2);
        break;
      case 'startdate|startdate':
      case 'enddate|enddate':
        if ($ba_1 == $ba_2) {
          $advanced_query .= $ba_1 ? _culturefeed_content_create_date_filter_param($values['when_type_1'], $date_1) : _culturefeed_content_create_date_filter_param($values['when_type_1'], null, $date_1);
          $advanced_query .= $ba_2 ? _culturefeed_content_create_date_filter_param($values['when_type_2'], $date_2) : _culturefeed_content_create_date_filter_param($values['when_type_2'], null, $date_2);
        }
        else {
          $advanced_query .= $ba_1 && !$ba_2 ? _culturefeed_content_create_date_filter_param($values['when_type_1'], $date_1, $date_2) : _culturefeed_content_create_date_filter_param($values['when_type_1'], $date_2, $date_1);
        }
        break;
      case 'startdate|enddate':
      case 'enddate|startdate':
        $advanced_query .= $ba_1 ? _culturefeed_content_create_date_filter_param($values['when_type_1'], $date_1) : _culturefeed_content_create_date_filter_param($values['when_type_1'], null, $date_1);
        $advanced_query .= $ba_2 ? _culturefeed_content_create_date_filter_param($values['when_type_2'], $date_2) : _culturefeed_content_create_date_filter_param($values['when_type_2'], null, $date_2);
        break;
    }
  }

  return $advanced_query . ')';
}

/**
 * Build the categories query string based on the current builder values.
 *
 * @param $values
 *
 * @return string
 */
function _culturefeed_content_build_categories_query($values) {
  $query = '';

  // Loop through all possible field rows.
  for ($i = 0; $i >= 0; $i++) {

    if (!isset($values["what_type_$i"]) || $values["what_type_$i"] == 'all') {
      break;
    }

    $type = $values["what_type_$i"];

    $selected = array_values($values["what_$type" . "_$i"]);

    if (!empty($selected)) {
      $not_equal = (int)$values["what_equal_$i"];

      $query .= '%26fq=(';

      for ($j = 0; $j<count($selected); $j++) {

        $cat_id = $selected[$j];

        // Check for possible grouped values and split them up.
        if (strpos($cat_id, ',')) {
          $cat_ids = explode(',', $cat_id);
          $selected = array_merge($selected, $cat_ids);
          continue;
        }

        $query .= ($not_equal ? 'NOT ' : '');

        $query .= "category_$type" ."_id:$cat_id";

        if(count($selected)-1 > $j) {
          $query .= ' OR ';
        }
        else {
          continue;
        }
      }

      $query .= ')';
    }
    else {
      break;
    }
  }

  return $query;
}

/**
 * Returns a date filter query parameter string for the advanced query.
 *
 * @param string $type
 * @param \DateTime|null $date_1
 * @param \DateTime|null $date_2
 *
 * @return string
 */
function _culturefeed_content_create_date_filter_param ($type = 'startdate', $date_1 = null, $date_2 = null) {
  if ($date_1) {
    $date_1->setTime(0, 0, 0);
    $value1 = gmdate('Y-m-d\TH:i:s\Z', $date_1->getTimestamp());
  }
  else {
    $value1 = '*';
  }

  if ($date_2) {
    $date_2->setTime(23, 59, 59);
    $value2 = gmdate('Y-m-d\TH:i:s\Z', $date_2->getTimestamp());
  }
  else {
    $value2 = '*';
  }

  return "%26fq=($type:[$value1 TO $value2])";
}

/**
 * Get the search results for given field item.
 * @param $field_item
 *   Field item to use.
 * @param $location
 *   Location to filter on.
 *
 * @return \CultuurNet\Search\SearchResult|null
 */
function _culturefeed_content_get_results($field_item, $postalcode = '') {

  $sort = isset($field_item['sort']) ? $field_item['sort'] : CULTUREFEED_CONTENT_DEFAULT_SORT;
  $rows = isset($field_item['rows']) ? $field_item['rows'] : CULTUREFEED_CONTENT_DEFAULT_ROWS;

  $parameters = array();

  $search_type = culturefeed_get_searchable_type('activiteiten');
  $sort_options = culturefeed_search_ui_get_sort_options_for_page($search_type);
  foreach ($sort_options as $option) {
    if (isset($option['value'])) {
      if ($option['value'] == $sort) {
        $parameters[] = new \CultuurNet\Search\Parameter\Parameter('sort', $option['query']);
        break;
      }
    }
  }

  $parameters[] = new \CultuurNet\Search\Parameter\Rows($rows);
  $parameters[] = new \CultuurNet\Search\Parameter\Group();

  // Retrieve advanced query string and convert values to parameters.
  foreach (explode('%26', $field_item['composite_query']) as $filter) {
    $key_value = explode('=', $filter);

    switch ($key_value[0]) {
      case 'q':
        $parameters[] = new \CultuurNet\Search\Parameter\Query($key_value[1]);
        break;
      case 'zipcode':
        // Split code and radius.
        $split = explode('!', $key_value[1]);
        $parameters[] = new \CultuurNet\Search\Parameter\Spatial\Zipcode((int)$split[0], (int)$split[1]);
        break;
      case 'fq':
      default:
        $facetFilterQuery = new \CultuurNet\Search\Parameter\FilterQuery(trim($key_value[1], '()'));
        $parameters[] = $facetFilterQuery;
        break;
    }
  }

  if ($postalcode) {
    $parameters[] = new \Cultuurnet\Search\Parameter\FilterQuery('zipcode:' . $postalcode);
  }

  // Execute search.
  return culturefeed_get_search_service()->search($parameters);

}