<?php

namespace Drupal\culturefeed_jwt\Access;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Routing\Access\AccessInterface;
use Drupal\culturefeed_jwt\Factory\JwtStatelessTokenFactory;
use Symfony\Component\Routing\Route;

/**
 * Class StatelessAccess.
 *
 * @package Drupal\culturefeed_jwt\Access
 */
class StatelessAccess implements AccessInterface {

  /**
   * The stateless token factory.
   *
   * @var \Drupal\culturefeed_jwt\Factory\JwtStatelessTokenFactory
   */
  protected $statelessTokenFactory;

  /**
   * StatelessAccess constructor.
   *
   * @param \Drupal\culturefeed_jwt\Factory\JwtStatelessTokenFactory $stateless_token_factory
   *   The stateless token factory.
   */
  public function __construct(JwtStatelessTokenFactory $stateless_token_factory) {
    $this->statelessTokenFactory = $stateless_token_factory;
  }

  /**
   * Check the stateless access.
   *
   * @param \Symfony\Component\Routing\Route $route
   *   The route.
   *
   * @return \Drupal\Core\Access\AccessResult
   *   The access result.
   */
  public function access(Route $route) {

    $required_status = filter_var($route->getRequirement('_culturefeed_jwt_stateless_access'), FILTER_VALIDATE_BOOLEAN);
    $actual_status = !empty($this->statelessTokenFactory->get());

    return AccessResult::allowedIf($required_status === $actual_status);

  }

}
