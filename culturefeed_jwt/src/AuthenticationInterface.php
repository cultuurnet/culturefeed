<?php

namespace Drupal\culturefeed_jwt;

use Symfony\Component\HttpFoundation\Request;

/**
 * The interface for authenticating a culturefeed user.
 */
interface AuthenticationInterface {

  /**
   * Authenticates the user.
   *
   * @param Request $request
   *   The request.
   */
  public function authorize(Request $request);

}
