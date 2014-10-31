<?php

/**
 * @file
 * Contains Drupal\culturefeed\Tests\AuthenticationTest.
 */

namespace Drupal\culturefeed\Tests;

use Drupal\simpletest\WebTestBase;

/**
 * Tests the authentication.
 *
 * @group Culturefeed
 * @see \Drupal\culturefeed\Authentication
 */
class AuthenticationTest extends WebTestBase {

  /**
   * Modules to enable.
   *
   * @var array
   */
  public static $modules = array('culturefeed');

  /**
   * Tests culturefeed authentication.
   */
  public function testAuthentication() {

    $this->drupalGet('culturefeed/oauth/connect');
    $this->assertResponse('200');

  }

}
