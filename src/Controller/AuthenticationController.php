<?php

/**
 * @file
 * Contains Drupal\culturefeed\Controller\AuthenticationController.
 */

namespace Drupal\culturefeed\Controller;

use Drupal\Core\Controller\ControllerBase;
use Symfony\Component\HttpFoundation\RedirectResponse;
use Drupal\Core\Routing\TrustedRedirectResponse;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\culturefeed\AuthenticationInterface;
use Symfony\Component\HttpFoundation\Request;

/**
 * Class AuthenticationController.
 *
 * @package Drupal\culturefeed\Controller
 */
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

   * @return TrustedRedirectResponse
   *   A redirect.
   */
  public function connect(Request $request, $type = \CultureFeed::AUTHORIZE_TYPE_REGULAR) {

    $language = $this->languageManager()->getCurrentLanguage();
    $auth_url = $this->authentication->connect($request, $language, $type);
    if ($auth_url == '<front>') {
      $auth_url = $this->getUrlGenerator()->generateFromRoute('<front>');
    }
    return new TrustedRedirectResponse($auth_url, 302);

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

    // Check if a redirect is provided, this can be an external url.
    if ($request->get('destination')) {
      try {
        return $this->redirect($request->get('destination'));
      }
      catch (\Exception $e) {
        return new RedirectResponse($request->get('destination'), 302);
      }
    }

    return $this->redirect('<front>');

  }

}
