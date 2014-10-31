<?php

/**
 * @file
 * Contains Drupal\culturefeed_udb3\Tests\AccessTest.
 */

namespace Drupal\culturefeed_udb3\Tests;

use Drupal\simpletest\WebTestBase;

/**
 * Tests the access.
 *
 * @group Culturefeed
 * @see \Drupal\culturefeed\Access\UserAccess
 */
class AccessTest extends WebTestBase {

  /**
   * Modules to enable.
   *
   * @var array
   */
  public static $modules = array('culturefeed_udb3');

  /**
   * Tests culturefeed udb3 access.
   */
  public function testAccess() {

    $this->drupalGet('udb3/api/1.0/user');
    $this->assertResponse('403');

  }

}
