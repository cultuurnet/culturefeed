<?php
/**
 * @file
 * Contains Drupal\culturefeed_udb3_app\Controller\AppController.
 */

namespace Drupal\culturefeed_udb3_app\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\DependencyInjection\ContainerInjectionInterface;

class AppController extends ControllerBase implements ContainerInjectionInterface {

  public function landing() {

    $baseTag = [
      '#type' => 'html_tag',
      '#tag' => 'base',
      '#attributes' => ['href' => '/']
    ];

    $renderArray = [
      '#theme' => 'udb3_landing',
      '#content' => Array('Hello', 'world'),
      '#attached' => [
        'library' => [
          'culturefeed_udb3_app/udb3-angular'
        ]
      ]
    ];

    return $renderArray;

  }

}