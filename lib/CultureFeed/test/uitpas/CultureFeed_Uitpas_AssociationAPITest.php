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

    $associations = $cf->uitpas()->getAssociations($balie_consumer_key);

    $this->assertInstanceOf('CultureFeed_ResultSet', $associations);
    $this->assertEquals(2, $associations->total);

    $this->assertInternalType('array', $associations->objects);
    $this->assertCount(2, $associations->objects);
    $this->assertContainsOnly('CultureFeed_Uitpas_Association', $associations->objects);

    /* @var CultureFeed_Uitpas_Association $association */
    $association = reset($associations->objects);

    $this->assertEquals(1, $association->id);
    $this->assertEquals('CJP', $association->name);

    $association = next($associations->objects);

    $this->assertEquals(2, $association->id);
    $this->assertEquals('Okra', $association->name);
  }
} 
