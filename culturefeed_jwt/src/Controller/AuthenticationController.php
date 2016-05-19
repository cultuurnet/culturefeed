<?php

namespace Drupal\culturefeed_jwt\Controller;

use Drupal\Core\Config\ConfigFactoryInterface;
use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\Routing\TrustedRedirectResponse;
use Drupal\Core\Url;
use Drupal\culturefeed_jwt\AuthenticationInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Symfony\Component\HttpFoundation\RedirectResponse;
use Symfony\Component\HttpFoundation\Request;

/**
 * Class AuthenticationController.
 *
 * @package Drupal\culturefeed_jwt\Controller
 */
class AuthenticationController extends ControllerBase {

  /**
   * The config factory.
   *
   * @var \Drupal\Core\Config\ConfigFactoryInterface
   */
  protected $config;

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
      $container->get('culturefeed_jwt.authentication'),
      $container->get('config.factory')
    );
  }

  /**
   * Constructs an AuthenticationController object.
   *
   * @param \Drupal\culturefeed_jwt\AuthenticationInterface $authentication
   *   The authentication service.
   * @param \Drupal\Core\Config\ConfigFactoryInterface $config
   *   The config factory.
   */
  public function __construct(AuthenticationInterface $authentication, ConfigFactoryInterface $config) {
    $this->authentication = $authentication;
    $this->config = $config->get('culturefeed_jwt.settings');
  }

  /**
   * Redirects to the culturefeed auth service.
   *
   * @return TrustedRedirectResponse
   *   A redirect.
   */
  public function connect() {

    $authorize_url = Url::fromRoute(
      'culturefeed.oauth.authorize',
      array(),
      array('absolute' => TRUE)
    );

    $authorize = $authorize_url->toString(TRUE)->getGeneratedUrl();

    $redirect_url = Url::fromUri(
      $this->config->get('login_url'),
      array('query' => array('destination' => $authorize))
    );

    $redirect = $redirect_url->toString(TRUE)->getGeneratedUrl();

    return new TrustedRedirectResponse(
      $redirect,
      302
    );

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
