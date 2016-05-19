<?php

namespace Drupal\culturefeed_jwt;

use Drupal\Core\DependencyInjection\ContainerBuilder;
use Drupal\Core\DependencyInjection\ServiceProviderBase;
use Symfony\Component\DependencyInjection\Reference;

/**
 * Class CulturefeedJwtServiceProvider.
 *
 * @package Drupal\culturefeed_jwt
 */
class CulturefeedJwtServiceProvider extends ServiceProviderBase {

  /**
   * {@inheritdoc}
   */
  public function alter(ContainerBuilder $container) {

    $definition = $container->getDefinition('culturefeed.current_user');
    $factory = array(
      new Reference('culturefeed_jwt.user_factory'),
      'get',
    );
    $definition->setFactory($factory);

  }

}
