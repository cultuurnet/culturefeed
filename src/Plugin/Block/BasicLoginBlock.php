<?php

/**
 * @file
 * Contains \Drupal\culturefeed\Plugin\Block\BasicLogin.
 */

namespace Drupal\culturefeed\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Url;

/**
 * Provides a 'Basic Login' block.
 *
 * @Block(
 *   id = "basic_login",
 *   admin_label = @Translation("Basic login"),
 *   category = @Translation("Culturefeed")
 * )
 */
class BasicLoginBlock extends BlockBase {

  /**
   * {@inheritdoc}
   */
  public function build() {

    $link = \Drupal::l(t('Login'), Url::fromRoute('culturefeed.oauth.connect'));
    return array(
      'login' => array(
        '#markup' => $link,
      ),
    );
  }

}
