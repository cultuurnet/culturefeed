<?php

/**
 * @file
 * Contains Drupal\culturefeed\Controller\AuthenticationController.
 */

namespace Drupal\culturefeed\Controller;

use Drupal\Core\Controller\ControllerBase;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\culturefeed\AuthenticationInterface;
use Symfony\Component\HttpFoundation\RedirectResponse;

class AuthenticationController extends ControllerBase {

  /**
   * The culturefeed authentication service.
   *
   * @var \Drupal\culturefeed\Authentication;
   */
  protected $authentication;

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('culturefeed.authentication')
    );
  }

  /**
   * Constructs an AuthenticationController object.
   *
   * @param \Drupal\culturefeed\AuthenticationInterface $authentication
   *   The authentication interface.
   */
  public function __construct(AuthenticationInterface $authentication) {
    $this->authentication = $authentication;
  }

  /**
   * Redirects to the culturefeed auth service.
   *
   * @return RedirectResponse
   *   A redirect.
   */
  public function connect() {

    $auth_url = $this->authentication->connect();
    return new RedirectResponse($auth_url, 302);

  }

  /**
   * Redirects after authentication.
   *
   * @return RedirectResponse
   *   A redirect.
   */
  public function authorize() {

    $this->authentication->authorize();
    return $this->redirect('<front>');

  }

}
