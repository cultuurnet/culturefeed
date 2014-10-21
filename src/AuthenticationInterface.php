<?php

/**
 * @file
 * Contains \Drupal\culturefeed\AuthenticationInterface.
 */

namespace Drupal\culturefeed;

use Drupal\Core\Language\LanguageInterface;
use Symfony\Component\HttpFoundation\Request;

/**
 * The interface for authenticating a culturefeed user.
 */
interface AuthenticationInterface {

  /**
   * Returns the authentication connect url.
   *
   * @param LanguageInterface $language
   *   The language.
   *
   * @return string $url
   *   A url.
   */
  public function connect(LanguageInterface $language);

  /**
   * Authenticates the user.
   *
   * @param Request $request.
   *   The request.
   */
  public function authorize(Request $request);

}
