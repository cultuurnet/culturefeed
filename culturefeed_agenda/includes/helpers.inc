<?php

/**
 * @return \CultuurNet\Search\ServiceInterface
 */
function searchpoc_get_search_service() {
  $service =& drupal_static(__FUNCTION__);

  if (!$service) {
    $baseUrl = 'http://searchv2.cultuurnet.lodgon.com/search-poc/rest';
    $consumerCredentials = new \CultuurNet\Auth\ConsumerCredentials();
    $consumerCredentials->setKey('');
    $consumerCredentials->setSecret('');
    // @todo token credentials
    $service = new \CultuurNet\Search\Guzzle\Service($baseUrl, $consumerCredentials);
  }

  return $service;
}