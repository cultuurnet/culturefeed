<?php

class CultureFeed_Uitpas_PasHoudersAPITest extends PHPUnit_Framework_TestCase {

  const PRICE = 15;
  const UID = "94305b2e-e7ff-4dfc-8d96-ef4d43de9038";

  public function testGetPrice() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $oauth_client_stub->expects($this->any())
             ->method('consumerGetAsXML')
             ->will($this->returnValue(self::PRICE));

    $cf = new CultureFeed($oauth_client_stub);

    $price = $cf->uitpas()->getPrice();

    $this->assertEquals(self::PRICE, $price);
  }

  public function testCreatePassholder() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $oauth_client_stub->expects($this->any())
             ->method('consumerPostAsXML')
             ->will($this->returnValue(self::UID));

    $cf = new CultureFeed($oauth_client_stub);

    $passholder = new CultureFeed_Uitpas_Passholder();
    $passholder->name = "Name";
    $passholder->firstName = "First name";
    $passholder->inszNumber = "87090513355";
    $passholder->dateOfBirth = strtotime('05/09/1987');
    $passholder->postalCode = "3293";
    $passholder->city = "Example city";
    $passholder->uitpasNumber = "122345";

    $uid = $cf->uitpas()->createPassholder($passholder);

    $this->assertEquals(self::UID, $uid);
  }
}