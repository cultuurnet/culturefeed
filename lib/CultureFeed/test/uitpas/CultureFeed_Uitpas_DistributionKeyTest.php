<?php

class CultureFeed_Uitpas_DistributionKeyTest extends PHPUnit_Framework_TestCase {

  const ORGANIZERCDBID = "47B6FA21-ACB1-EA8F-2C231182C7DD0A19";

  public function testRegisterDistributionKey() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $get_xml = file_get_contents(dirname(__FILE__) . '/data/distributionkey/get.xml');

    $oauth_client_stub->expects($this->once())
             ->method('consumerGetAsXML')
             ->will($this->returnValue($get_xml));

    $cf = new CultureFeed($oauth_client_stub);

    $data = $cf->uitpas()->getDistributionKeysForOrganizer(self::ORGANIZERCDBID);
    
    $this->assertContainsOnly( 'CultureFeed_Uitpas_DistributionKey' , $data->objects );
    $this->assertCount(2, $data->objects);
    
    $this->assertEquals("Standaard verdeelsleutel", $data->objects[0]->name);
    $this->assertEquals("Test verdeelsleutel - TK421", $data->objects[1]->name);
    $this->assertEquals("1", $data->objects[0]->id);
    $this->assertEquals("2", $data->objects[1]->id);
    
    $this->assertEquals( "1" , "2" );
    
  }
}