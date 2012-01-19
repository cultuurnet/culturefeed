<?php

class CultureFeed_Uitpas_PasHoudersAPITest extends PHPUnit_Framework_TestCase {

  const PRICE = 15;

  public function testGetPrice() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $oauth_client_stub->expects($this->any())
             ->method('consumerGetAsXML')
             ->will($this->returnValue(self::PRICE));

    $cf = new CultureFeed($oauth_client_stub);

    $price = $cf->uitpas()->getPrice();

    $this->assertEquals(self::PRICE, $price);
  }
}