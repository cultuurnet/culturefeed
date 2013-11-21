<?php

$plugin = array(
  'name' => 'uitpas_promotions',
  'base_table' => 'uitpas_promotions',
  'form_wizard_class' => array(
    'file' => 'culturefeed_uitpas_promotions_views_wizard.class.php',
    'class' => 'CulturefeedUitpasPromotionsViewsWizard',
  ),
  'title' => t('UiTPAS Promotions'),
  'available_sorts' => array(
    //'points:DESC' => t('Points'),
  ),
  ''
);
