<?php
/**
 * @file
 * Integration with the Views module.
 */

/**
 * Implements hook_views_plugins().
 */
function culturefeed_uitpas_views_plugins() {

  $path = drupal_get_path('module', 'culturefeed_uitpas') . '/views';

  $plugins = array();

  $plugins['query']['culturefeed_uitpas_promotions_query'] = array(
    'uses fields' => TRUE,
    'path' => $path . '/plugins',
    'title' => t('Culturefeed UiTPAS promotions query'),
    'help' => t('Query on UiTPAS promotions.'),
    'handler' => 'culturefeed_uitpas_promotions_query',
    'use pager' => TRUE,
  );

  return $plugins;
}

/**
 * Implements hook_views_data().
 */
function culturefeed_uitpas_views_data() {

  $data['uitpas_promotions']['table'] = array(
    'group' => t('UiTPAS promotions'),
    'base' => array(
      'field' => 'id',
      'title' => t('UiTPAS promotions'),
      'query class' => 'culturefeed_uitpas_promotions_query',
      'defaults' => array(
        'field' => 'title',
      ),
    ),
  );

  $data['uitpas_promotions']['id'] = array(
    'title' => t('ID'),
    'help' => t('The promotion ID.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );

  $data['uitpas_promotions']['title'] = array(
    'title' => t('Title'),
    'help' => t('The promotion title.'),
    'field' => array(
      'handler' => 'culturefeed_uitpas_views_handler_field_title',
      'base_path' => 'promotion',
    ),
    'sort' => array(
      'handler' => 'culturefeed_uitpas_views_handler_sort',
      'sort_property' => 'TITLE',
    ),
  );

  $data['uitpas_promotions']['points'] = array(
    'title' => t('Points'),
    'help' => t('The points necessary to make use of the promotion.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
    'sort' => array(
      'handler' => 'culturefeed_uitpas_views_handler_sort',
      'sort_property' => 'POINTS',
    ),
  );

  $data['uitpas_promotions']['cashinstate'] = array(
    'title' => t('Cashin state'),
    'help' => t('The cashin state of the promotion.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );

  $data['uitpas_promotions']['unexpired'] = array(
    'title' => t('Unexpired'),
    'help' => t('If the promotion is not expired yet.'),
    'filter' => array(
      'handler' => 'culturefeed_uitpas_views_handler_filter_boolean',
      'label' => t('Unexpired'),
    ),
  );

  $data['uitpas_promotions']['inSpotlight'] = array(
    'title' => t('In spotlight'),
    'help' => t('If the promotion is in the spotlight.'),
    'filter' => array(
      'handler' => 'culturefeed_uitpas_views_handler_filter_boolean',
      'label' => t('In spotlight'),
    ),
  );

  $data['uitpas_promotions']['owningcardsystemid'] = array(
    'title' => t('Owning card system'),
    'help' => t('The card system owning the promotion.'),
    'filter' => array(
      'handler' => 'culturefeed_uitpas_views_handler_filter_card_system_id',
      'filter_property' => 'owningCardSystemId',
    ),
  );

  $data['uitpas_promotions']['applicablecardsystemid'] = array(
    'title' => t('Applicable card system'),
    'help' => t('The card system the promotion is applicable to.'),
    'filter' => array(
      'handler' => 'culturefeed_uitpas_views_handler_filter_card_system_id',
      'filter_property' => 'applicableCardSystemId',
    ),
  );

  $data['uitpas_promotions']['cashing_period_begin'] = array(
    'title' => t('Cashing period begin'),
    'help' => t("The start of the promotion's cashing period."),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'real field' => 'cashingperiodbegin',
    ),
    'filter' => array(
      'handler' => 'culturefeed_uitpas_views_handler_filter_date',
      'real field' => 'cashingPeriodBegin',
    ),
  );

  $data['uitpas_promotions']['cashing_period_end'] = array(
    'title' => t('Cashing period end'),
    'help' => t("The end of the promotion's cashing period."),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'real field' => 'cashingperiodend',
    ),
    'sort' => array(
      'handler' => 'culturefeed_uitpas_views_handler_sort',
      'sort_property' => 'CASHING_PERIOD_END',
    ),
  );

  $data['uitpas_promotions']['creation_date'] = array(
    'title' => t('Creation date'),
    'help' => t("The creation date of the promotion."),
    'sort' => array(
      'handler' => 'culturefeed_uitpas_views_handler_sort',
      'sort_property' => 'CREATION_DATE',
    ),
  );

  $data['uitpas_promotions']['description_1'] = array(
    'title' => t('Description 1'),
    'help' => t('The description (1) of the promotion.'),
    'field' => array(
      'handler' => 'culturefeed_uitpas_views_handler_field_markup',
      'real field' => 'description1',
      'format' => 'full_html',
    ),
  );

  $data['uitpas_promotions']['description_2'] = array(
    'title' => t('Description 2'),
    'help' => t('The description (2) of the promotion.'),
    'field' => array(
      'handler' => 'culturefeed_uitpas_views_handler_field_markup',
      'real field' => 'description2',
    ),
  );

  $data['uitpas_promotions']['image'] = array(
    'title' => t('Image'),
    'help' => t('The images of the promotion.'),
    'field' => array(
      'handler' => 'culturefeed_uitpas_views_handler_field_image',
      'real field' => 'pictures',
    ),
  );

  return $data;

}
