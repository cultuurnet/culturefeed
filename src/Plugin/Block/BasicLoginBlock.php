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

    $account = \Drupal::currentUser();
    if ($account->isAnonymous()) {
      $link = \Drupal::l(t('Login'), Url::fromRoute('culturefeed.oauth.connect'));
    }
    else {
      $link = \Drupal::l(t('Logout'), Url::fromRoute('user.logout'));
    }
    return array(
      'link' => array(
        '#markup' => $link,
      ),
    );
  }

}
