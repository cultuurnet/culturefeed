<?php

/**
 * @file
 * Contains Drupal\culturefeed\Controller\UserRestController.
 */

namespace Drupal\culturefeed_udb3\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\culturefeed\UserInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Symfony\Component\HttpFoundation\JsonResponse;

class UserRestController extends ControllerBase {

  /**
   * The culturefeed user service.
   *
   * @var \Drupal\culturefeed\User;
   */
  protected $user;

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('culturefeed.user')
    );
  }

  /**
   * Constructs a RestController.
   *
   * @param UserInterface $user
   */
  public function __construct(UserInterface $user) {
    $this->user = $user;
  }

  function info() {

    $response = JsonResponse::create()
      ->setPublic()
      ->setClientTtl(60 * 1)
      ->setTtl(60 * 5);

    $user = $this->user->get();
    $response
      ->setData($user)
      ->setPublic()
      ->setClientTtl(60 * 30)
      ->setTtl(60 * 5);

    $response->headers->set('Content-Type', 'application/ld+json');

    return $response;

  }

}
