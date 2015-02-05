<?php
/**
 * @file
 */

namespace Drupal\culturefeed_udb3;


use Drupal\Core\DependencyInjection\ContainerBuilder;
use Drupal\Core\DependencyInjection\ServiceProviderInterface;

class CulturefeedUdb3ServiceProvider implements ServiceProviderInterface
{
    /**
     * Registers services to the container.
     *
     * @param ContainerBuilder $container
     *   The ContainerBuilder to register services to.
     */
    public function register(ContainerBuilder $container)
    {
        $container->addCompilerPass(new RegisterEventBusSubscribersPass());
    }

}
