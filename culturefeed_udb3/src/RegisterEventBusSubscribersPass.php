<?php
/**
 * @file
 */

namespace Drupal\culturefeed_udb3;


use Symfony\Component\DependencyInjection\Compiler\CompilerPassInterface;
use Symfony\Component\DependencyInjection\ContainerBuilder;

class RegisterEventBusSubscribersPass implements CompilerPassInterface
{
    /**
     * @inheritdoc
     */
    public function process(ContainerBuilder $container)
    {
        $busServiceId = 'culturefeed_udb3.event_bus';
        if (!$container->hasDefinition($busServiceId)) {
            return;
        }

        $definition = $container->getDefinition($busServiceId);

        $taggedServices = $container->findTaggedServiceIds($busServiceId . '.subscriber');
        $taggedServiceIds = array_keys($taggedServices);

        $definition->addArgument($taggedServiceIds);
    }

}
