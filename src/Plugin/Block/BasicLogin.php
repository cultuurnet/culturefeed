<?php

/**
 * @file
 * Contains \Drupal\culturefeed\Plugin\Block\BasicLogin.
 */

namespace Drupal\culturefeed\Plugin\Block;

use Drupal\Core\Block\BlockBase;

/**
 * Provides a 'Basic Login' block.
 *
 * @Block(
 *   id = "basic_login",
 *   admin_label = @Translation("Basic login"),
 *   category = @Translation("Culturefeed")
 * )
 */
class BasicLogin extends BlockBase {

  /**
   * {@inheritdoc}
   */
  public function build() {

    return array(
      'login' => array(
        '#markup' => 'test',
      ),
    );
  }

}
