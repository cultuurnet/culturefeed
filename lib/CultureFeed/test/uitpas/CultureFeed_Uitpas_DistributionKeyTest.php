<?php

class CultureFeed_Uitpas_DistributionKeyTest extends PHPUnit_Framework_TestCase {

  const ORGANIZERCDBID = "47B6FA21-ACB1-EA8F-2C231182C7DD0A19";

  public function testGetDistributionKeysForOrganizer() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $get_xml = file_get_contents(dirname(__FILE__) . '/data/distributionkey/get.xml');

    $oauth_client_stub->expects($this->once())
             ->method('consumerGetAsXML')
             ->will($this->returnValue($get_xml));

    $cf = new CultureFeed($oauth_client_stub);

    $data = $cf->uitpas()->getDistributionKeysForOrganizer(self::ORGANIZERCDBID);


    $this->assertInstanceOf('CultureFeed_ResultSet', $data);

    $this->assertContainsOnly('CultureFeed_Uitpas_DistributionKey', $data->objects);
    $this->assertCount(6, $data->objects);
    $this->assertEquals(6, $data->total);

    /* @var CultureFeed_Uitpas_DistributionKey $key */
    $key = reset($data->objects);

    $this->assertEquals(35, $key->id);
    $this->assertEquals("School - Halve Dag - €1,50", $key->name);
    $this->assertInstanceOf('CultureFeed_Uitpas_CardSystem', $key->cardSystem);
    $this->assertEquals(1, $key->cardSystem->id);
    $this->assertEquals('HELA', $key->cardSystem->name);

    $key = next($data->objects);

    $this->assertEquals(36, $key->id);
    $this->assertEquals("School - hele dag €3", $key->name);
    $this->assertInstanceOf('CultureFeed_Uitpas_CardSystem', $key->cardSystem);
    $this->assertEquals(1, $key->cardSystem->id);
    $this->assertEquals('HELA', $key->cardSystem->name);
  }
}
