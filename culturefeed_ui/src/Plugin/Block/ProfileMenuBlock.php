<?php

/**
 * @file
 * Contains Drupal\culturefeed_ui\Plugin\Block\ProfileMenuBlock.
 */

namespace Drupal\culturefeed_ui\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Url;
use Drupal\Core\Routing\RouteMatchInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides a 'Profile menu' block.
 *
 * @Block(
 *  id = "ui_profile_menu",
 *  admin_label = @Translation("profile_menu_block"),
 * )
 */
class ProfileMenuBlock extends BlockBase implements ContainerFactoryPluginInterface {

  /**
   * The route match.
   *
   * @var \Drupal\Core\Routing\RouteMatchInterface
   */
  protected $routeMatch;

  /**
   * Constructs a new UserLoginBlock instance.
   *
   * @param array $configuration
   *   The plugin configuration, i.e. an array with configuration values keyed
   *   by configuration option name. The special key 'context' may be used to
   *   initialize the defined contexts by setting it to an array of context
   *   values keyed by context names.
   * @param string $plugin_id
   *   The plugin_id for the plugin instance.
   * @param mixed $plugin_definition
   *   The plugin implementation definition.
   * @param \Drupal\Core\Routing\RouteMatchInterface $route_match
   *   The route match.
   */
  public function __construct(
    array $configuration,
    $plugin_id,
    $plugin_definition,
    RouteMatchInterface $route_match
  ) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);

    $this->routeMatch = $route_match;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('current_route_match')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function build() {

    $menuItems = array(
      'uitid' => array(
        'title' => t('My profile'),
        'url' => Url::fromRoute('culturefeed_ui.user_controller_profile'),
        'description' => t('How other users will see your profile'),
        'weight' => -20,
      ),
      'profile' => array(
        'title' => t('Edit profile'),
        'url' => Url::fromRoute('culturefeed_ui.profile_form'),
        'description' => t('Change name, address, profile picture, …'),
        'weight' => -19,
      ),
      'account' => array(
        'title' => t('Edit account'),
        'url' => Url::fromRoute('culturefeed_ui.account_form'),
        'description' => t('Change password, email address, connected channels, …'),
        'weight' => -18,
      ),
    );

//    $menu = module_invoke_all('culturefeed_ui_profile_menu');
//    drupal_alter('culturefeed_ui_profile_menu', $menu);
//    uasort($menu, 'drupal_sort_weight');

    $items = array();

    foreach ($menuItems as $menu_item) {

      $currentRouteName = $this->routeMatch->getRouteName();
      /** @var Url $itemUrl */
      $itemUrl = $menu_item['url'];

      $class = $currentRouteName == $itemUrl->getRouteName() ? array('active') : array();

      $itemContent = [
        '#theme' => 'culturefeed_ui_profile_menu_item',
        '#title' => $menu_item['title'],
        '#url' => $menu_item['url'],
        '#description' => $menu_item['description'],
        '#attribute_wrapper' => array (
          'class' => $class
        )
      ];

      $renderedItemContent = \Drupal::service('renderer')->render($itemContent);

      $items[] = $renderedItemContent;
    }

    $block = array(
      '#theme' => 'item_list',
      '#items' => $items,
      '#title' => t('Manage profile')
    );

    return $block;
  }

}
