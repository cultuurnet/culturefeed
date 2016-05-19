<?php

namespace Drupal\culturefeed_jwt\Routing;

use Drupal\Core\Routing\RouteSubscriberBase;
use Symfony\Component\Routing\RouteCollection;

/**
 * Class RouteSubscriber.
 *
 * @package Drupal\culturefeed_jwt\Routing
 */
class RouteSubscriber extends RouteSubscriberBase {

  /**
   * {@inheritdoc}
   */
  public function alterRoutes(RouteCollection $routes) {

    $connect = $routes->get('culturefeed.oauth.connect');
    $connect->setDefault('_controller', 'Drupal\culturefeed_jwt\Controller\AuthenticationController::connect');

    $authorize = $routes->get('culturefeed.oauth.authorize');
    $authorize->setDefault('_controller', 'Drupal\culturefeed_jwt\Controller\AuthenticationController::authorize');

  }

}
