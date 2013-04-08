<?php
/**
 * @file
 * Defines page callbacks which access the Search api.
 */

/**
 * Page callback to show the detail from an agenda item.
 * @param \CultuurNet\Search\ActivityStatsExtendedEntity $item
 *   Extended entity to view.
 */
function culturefeed_agenda_detail_page(\CultuurNet\Search\ActivityStatsExtendedEntity $item) {

  // Hardcoded breadcrumb requested. Use active trail, so breadcrumb can be altered.
  $active_trail = array();

  $active_trail[] = array(
    'title' => t('Home'),
    'href' => '<front>',
    'link_path' => '',
    'localized_options' => array(),
    'type' => 0,
  );

  $active_trail[] = array(
    'title' => 'Activiteiten',
    'href' => 'agenda/search',
    'link_path' => '',
    'localized_options' => array(),
    'type' => 0,
  );

  // First theme.
  $themes = $item->getEntity()->getCategories()->getCategoriesByType(CultureFeed_Cdb_Data_Category::CATEGORY_TYPE_THEME);
  if ($themes) {
    $active_trail[] = array(
      'title' => $themes[0]->getName(),
      'href' => 'agenda/search',
      'link_path' => '',
      'localized_options' => array(
        'query' => array(
          'facet' => array('category_theme_name' => array($themes[0]->getName())),
        ),
      ),
      'type' => 0,
    );

  }

  $active_trail[] = array(
    'title' => drupal_get_title(),
    'href' => $_GET['q'],
    'link_path' => '',
    'localized_options' => array(),
    'type' => 0,
  );

  menu_set_active_trail($active_trail);

  return array(
    '#theme' => 'culturefeed_' . $item->getType(),
    '#item' => $item,
  );

}
