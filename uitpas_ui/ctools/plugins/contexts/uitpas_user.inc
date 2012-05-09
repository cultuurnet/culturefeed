<?php

/**
 * @file
 * Plugin to provide a context for uitpas user.
 */

$plugin = array(
  'title' => t('Uitpas: user'),
  'description' => t('Uitpas: user.'),
  'context' => 'uitpas_ui_context_uitpas_user',
  'context name' => 'uitpas_user',
  'convert list' => array(
    'id' => t('Uitpas user id'),
    'points' => t('Uitpas user points'),
    'culture_feed' => t('Uitpas user culturefeed'),
  ),
  'convert' => 'uitpas_ui_context_uitpas_user_convert',
  'keyword' => 'uitpas_user',
);

function uitpas_ui_context_uitpas_user($empty, $data = NULL, $conf = FALSE) {
  $context = new ctools_context('uitpas_user');
  $context->plugin = 'uitpas_user';
  $context->data = FALSE;
  try {
    if (DrupalCultureFeed::isCultureFeedUser()) {
      $context->data = new stdClass();
      $context->data->user = DrupalCultureFeed::getLoggedInUser();
      $cf = DrupalCultureFeed::getLoggedInUserInstance();
      $context->data->cf = $cf;
      try {
        $context->data->passholder = $cf->uitpas()->getPassholderByUser($context->data->user->id);
      }
      catch (Exception $e) {
        $context->data->passholder = FALSE;
      }
    }
  }
  catch (Exception $e) {
    watchdog_exception('uitpas_ui_context_uitpas_user', $e);
  }
  return $context;
}

function uitpas_ui_context_uitpas_user_convert($context, $type) {
  switch ($type) {
    case 'id':
      return $context->data->id;
    case 'points':
      return $context->data->passholder->points;
    case 'cf':
      return $context->data->cf;
  }
}