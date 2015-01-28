<?php
/**
 * @file
 */

namespace Drupal\culturefeed_udb3_app\Controller;


use Drupal\Core\Controller\ControllerBase;

class EventController extends ControllerBase {

  public function newEvent() {

    $renderArray = [
      '#theme' => 'udb3_new_event_form',
      '#attached' => [
        'library' => [
          'culturefeed_udb3_app/udb3-angular'
        ],
      ],
    ];

    return $renderArray;
  }
}
