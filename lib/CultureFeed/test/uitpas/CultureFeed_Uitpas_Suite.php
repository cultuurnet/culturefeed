<?php

require_once('CultureFeed_Uitpas_PasHoudersAPITest.php');
require_once('CultureFeed_Uitpas_VerdeelSleutelsAPITest.php');

/**
 * Static test suite.
 */
class CultureFeed_Uitpas_Suite extends PHPUnit_Framework_TestSuite {

  /**
   * Constructs the test suite handler.
   */
  public function __construct() {
    $this->setName('CultureFeed_Uitpas_Suite');
  }

  /**
   * Creates the suite.
   */
  public static function suite() {
    $suite = new PHPUnit_Framework_TestSuite('CultureFeed_Uitpas');

    $suite->addTestSuite('CultureFeed_Uitpas_VerdeelSleutelsAPITest');
    $suite->addTestSuite('CultureFeed_Uitpas_PasHoudersAPITest');

    return $suite;
  }
}

