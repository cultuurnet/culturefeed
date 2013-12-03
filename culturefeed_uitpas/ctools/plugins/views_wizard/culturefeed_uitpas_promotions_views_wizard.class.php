<?php
// @codingStandardsIgnoreFile

/**
 * @file
 */

class CulturefeedUitpasPromotionsViewsWizard extends ViewsUiBaseViewsWizard {

  /**
   * {@inheritdoc}
   */
  protected function default_display_options($form, $form_state) {

    $display_options = parent::default_display_options($form, $form_state);

    // Remove the default fields, since we are customizing them here.
    unset($display_options['fields']);

    /* Field: UiTPAS promotions: Image */
    $display_options['fields']['image']['id'] = 'image';
    $display_options['fields']['image']['table'] = 'uitpas_promotions';
    $display_options['fields']['image']['field'] = 'image';
    $display_options['fields']['image']['label'] = '';
    $display_options['fields']['image']['maxheight'] = '200';
    $display_options['fields']['image']['maxwidth'] = '200';

    /* Field: UiTPAS promotions: Title */
    $display_options['fields']['title']['id'] = 'title';
    $display_options['fields']['title']['table'] = 'uitpas_promotions';
    $display_options['fields']['title']['field'] = 'title';
    $display_options['fields']['title']['link'] = 1;

    /* Field: UiTPAS promotions: Points */
    $display_options['fields']['points']['id'] = 'points';
    $display_options['fields']['points']['table'] = 'uitpas_promotions';
    $display_options['fields']['points']['field'] = 'points';
    $display_options['fields']['points']['separator'] = '.';

    /* Field: UiTPAS promotions: Description 1 */
    $display_options['fields']['description_1']['id'] = 'description_1';
    $display_options['fields']['description_1']['table'] = 'uitpas_promotions';
    $display_options['fields']['description_1']['field'] = 'description_1';
    $display_options['fields']['description_1']['label'] = '';

    /* Field: UiTPAS promotions: Description 2 */
    $display_options['fields']['description_2']['id'] = 'description_2';
    $display_options['fields']['description_2']['table'] = 'uitpas_promotions';
    $display_options['fields']['description_2']['field'] = 'description_2';
    $display_options['fields']['description_2']['label'] = '';

    /* Field: UiTPAS promotions: Cashing period end */
    $display_options['fields']['cashing_period_end']['id'] = 'cashing_period_end';
    $display_options['fields']['cashing_period_end']['table'] = 'uitpas_promotions';
    $display_options['fields']['cashing_period_end']['field'] = 'cashing_period_end';
    $display_options['fields']['cashing_period_end']['label'] = 'Valid till';
    $display_options['fields']['cashing_period_end']['date_format'] = 'custom';
    $display_options['fields']['cashing_period_end']['custom_date_format'] = 'd-m-Y';
    $display_options['fields']['cashing_period_end']['empty'] = 'End of stock';

    /* Sort criterion: UiTPAS promotions: Points */
    $display_options['sorts']['points']['id'] = 'points';
    $display_options['sorts']['points']['table'] = 'uitpas_promotions';
    $display_options['sorts']['points']['field'] = 'points';
    $display_options['sorts']['points']['order'] = 'DESC';

    /* Filter criterion: UiTPAS promotions: Unexpired */
    $display_options['filters']['unexpired']['id'] = 'unexpired';
    $display_options['filters']['unexpired']['table'] = 'uitpas_promotions';
    $display_options['filters']['unexpired']['field'] = 'unexpired';
    $display_options['filters']['unexpired']['value'] = '1';
    $display_options['filters']['unexpired']['group'] = 1;

    /* Filter criterion: UiTPAS promotions: Cashing period begin */
    $display_options['filters']['cashing_period_begin']['id'] = 'cashing_period_begin';
    $display_options['filters']['cashing_period_begin']['table'] = 'uitpas_promotions';
    $display_options['filters']['cashing_period_begin']['field'] = 'cashing_period_begin';
    $display_options['filters']['cashing_period_begin']['group'] = 1;

    return $display_options;

  }

  /**
   * {@inheritdoc}
   */
  protected function alter_display_options(&$display_options, $form, $form_state) {

    $display_options['default']['pager']['type'] = 'full';
    $display_options['default']['pager']['options']['items_per_page'] = 10;

    // Page.
    if (isset($display_options['page'])) {

      $display_options['page']['style_plugin'] = 'table';
      $display_options['page']['row_plugin'] = 'fields';

    }

    // Block.
    if (isset($display_options['block'])) {

      if (isset($display_options['page'])) {

        $display_options['block']['defaults']['fields'] = FALSE;
        $display_options['block']['defaults']['pager'] = FALSE;

      }

      $display_options['block']['pager']['type'] = 'some';
      $display_options['block']['pager']['options']['items_per_page'] = 5;
      $display_options['block']['style_plugin'] = 'table';
      $display_options['block']['row_plugin'] = 'fields';

      $display_options['block']['fields'] = $display_options['default']['fields'];
      unset($display_options['block']['fields']['image']['maxheight']);
      unset($display_options['block']['fields']['image']['maxwidth']);
      unset($display_options['block']['fields']['description_1']);
      unset($display_options['block']['fields']['description_2']);
      unset($display_options['block']['fields']['cashing_period_end']);

    }

  }

  /**
   * {@inheritdoc}
   */
  public function build_form($form, &$form_state) {

    $form = parent::build_form($form, $form_state);

    $form['displays']['show']['sort']['#access'] = FALSE;

    // Page.
    $form['displays']['page']['options']['style']['#access'] = FALSE;
    $form['displays']['page']['options']['items_per_page']['#access'] = FALSE;
    $form['displays']['page']['options']['pager']['#access'] = FALSE;

    // Block.
    $form['displays']['block']['create']['#default_value'] = TRUE;
    $form['displays']['block']['options']['style']['#access'] = FALSE;
    $form['displays']['block']['options']['items_per_page']['#access'] = FALSE;
    $form['displays']['block']['options']['pager']['#access'] = FALSE;

    return $form;

  }

}
