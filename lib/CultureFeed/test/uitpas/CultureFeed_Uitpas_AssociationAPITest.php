<?php
/**
 * @file
 */

class CultureFeed_Uitpas_AssociationAPITest extends PHPUnit_Framework_TestCase {

  public function testGetAssociations() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $balie_consumer_key = 'e52efb7f-2eab-47a5-9cf3-9e7413ffd942';

    $xml = file_get_contents(dirname(__FILE__) . '/data/associations/list.xml');

    $oauth_client_stub
      ->expects($this->once())
      ->method('authenticatedGetAsXml')
      ->with('uitpas/association/list', array(
          'balieConsumerKey' => $balie_consumer_key,
        ))
      ->will($this->returnValue($xml));

    $cf = new CultureFeed($oauth_client_stub);

    $result = $cf->uitpas()->getAssociations($balie_consumer_key);

    $this->assertInstanceOf('CultureFeed_ResultSet', $result);
    $this->assertEquals(2, $result->total);

    $this->assertInternalType('array', $result->objects);
    $this->assertCount(2, $result->objects);
    $this->assertContainsOnly('CultureFeed_Uitpas_Association', $result->objects);

    /* @var CultureFeed_Uitpas_Association $association */
    $association = reset($result->objects);

    $this->assertEquals(1, $association->id);
    $this->assertEquals('CJP', $association->name);
    $this->assertInstanceOf('CultureFeed_Uitpas_CardSystem', $association->cardSystem);
    $this->assertEquals(6, $association->cardSystem->id);
    $this->assertEquals('Testsysteem Paspartoe', $association->cardSystem->name);

    $association = next($result->objects);

    $this->assertEquals(2, $association->id);
    $this->assertEquals('Okra', $association->name);
    $this->assertInstanceOf('CultureFeed_Uitpas_CardSystem', $association->cardSystem);
    $this->assertEquals(1, $association->cardSystem->id);
    $this->assertEquals('HELA', $association->cardSystem->name);
  }
} 
