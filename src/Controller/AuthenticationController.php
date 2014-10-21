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
use Symfony\Component\HttpFoundation\Request;

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
   * @param Request $request
   *   The request.

   * @return RedirectResponse
   *   A redirect.
   */
  public function connect(Request $request) {

    $language = $this->languageManager()->getCurrentLanguage();
    $auth_url = $this->authentication->connect($request, $language);
    return new RedirectResponse($auth_url, 302);

  }

  /**
   * Redirects after authentication.
   *
   * @param Request $request
   *   The request.
   *
   * @return RedirectResponse
   *   A redirect.
   */
  public function authorize(Request $request) {

    $this->authentication->authorize($request);
    return $this->redirect('<front>');

  }

}
