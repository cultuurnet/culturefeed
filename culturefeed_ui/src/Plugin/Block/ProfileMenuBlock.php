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
 *  admin_label = @Translation("Profile menu block"),
 * )
 */
class ProfileMenuBlock extends BlockBase {

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

    $items = array();

    foreach ($menuItems as $menu_item) {
      /** @var Url $itemUrl */
      $itemUrl = $menu_item['url'];
      $itemUrl->setOption('set_active_class', TRUE);

      $items[] = [
        '#theme' => 'culturefeed_ui_profile_menu_item',
        '#title' => $menu_item['title'],
        '#url' => $menu_item['url'],
        '#description' => $menu_item['description'],
      ];
    }

    $block = array(
      '#theme' => 'item_list',
      '#items' => $items,
    );

    return $block;
  }

}
