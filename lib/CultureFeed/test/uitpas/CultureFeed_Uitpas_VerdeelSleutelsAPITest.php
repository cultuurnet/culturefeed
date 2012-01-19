<?php

class CultureFeed_Uitpas_VerdeelSleutelsAPITest extends PHPUnit_Framework_TestCase {

  public function testGetDistributionKeysForOrganizer() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $consumers_xml = file_get_contents(dirname(__FILE__) . '/data/verdeelsleutels/list.xml');

    $oauth_client_stub->expects($this->any())
             ->method('consumerGetAsXML')
             ->will($this->returnValue($consumers_xml));

    $cf = new CultureFeed($oauth_client_stub);

    $cdbid = "31413BDF-DFC7-7A9F-10403618C2816E44";
    $result = $cf->uitpas()->getDistributionKeysForOrganizer($cdbid);

    $this->assertEquals(2, $result->total);

    $distribution_keys = $result->objects;

    $this->assertInternalType('array', $distribution_keys);
    $this->assertEquals(2, count($distribution_keys));
    $this->assertContainsOnly('CultureFeed_Uitpas_DistributionKey', $distribution_keys);

    $this->assertEquals(1, $distribution_keys[0]->id);
    $this->assertEquals('Standaard verdeelsleutel', $distribution_keys[0]->name);

    $this->assertEquals(2, $distribution_keys[1]->id);
    $this->assertEquals('Afwijkende verdeelsleutel', $distribution_keys[1]->name);
  }

}