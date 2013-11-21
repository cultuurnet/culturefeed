<?php

class CulturefeedUitpasPromotionsViewsWizard extends ViewsUiBaseViewsWizard {

  protected function default_display_options($form, $form_state) {dpm($form_state);

    $display_options = parent::default_display_options($form, $form_state);

    // Remove the default fields, since we are customizing them here.
    unset($display_options['fields']);

    $display_options['style_plugin'] = 'table';

    $display_options['pager']['type'] = 'some';
    $display_options['pager']['options']['items_per_page'] = '10';
    $display_options['pager']['options']['offset'] = '0';

    $display_options['style_options']['override'] = 1;
    $display_options['style_options']['sticky'] = 0;
    $display_options['style_options']['empty_table'] = 0;

    /* Field: UiTPAS promotions: Image */
    $display_options['fields']['image']['id'] = 'image';
    $display_options['fields']['image']['table'] = 'uitpas_promotions';
    $display_options['fields']['image']['field'] = 'image';
    $display_options['fields']['image']['label'] = '';

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

}
