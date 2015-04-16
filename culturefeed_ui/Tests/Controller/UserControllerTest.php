<?php

/**
 * @file
 * Contains Drupal\culturefeed_ui\Tests\UserController.
 */

namespace Drupal\culturefeed_ui\Tests;

use Drupal\simpletest\WebTestBase;

/**
 * Provides automated tests for the culturefeed_ui module.
 */
class UserControllerTest extends WebTestBase {
  /**
   * {@inheritdoc}
   */
  public static function getInfo() {
    return array(
      'name' => "culturefeed_ui UserController's controller functionality",
      'description' => 'Test Unit for module culturefeed_ui and controller UserController.',
      'group' => 'Other',
    );
  }

  /**
   * {@inheritdoc}
   */
  public function setUp() {
    parent::setUp();
  }

  /**
   * Tests culturefeed_ui functionality.
   */
  public function testUserController() {
    // Check that the basic functions of module culturefeed_ui.
    $this->assertEqual(TRUE, TRUE, 'Test Unit Generated via App Console.');
  }

}
