<?php

class CultureFeed_Uitpas_PasHoudersAPITest extends PHPUnit_Framework_TestCase {

  const PRICE = 15;
  const UID = "94305b2e-e7ff-4dfc-8d96-ef4d43de9038";
  const WELCOME_ADVANTAGE_ID = 1;
  const UITPAS_NUMBER = "32483743";
  const CONSUMER_KEY_COUNTER = "94305r2e-e7ff-4dfc-8dd6-ef4d43de9098";
  const POINTS = 2;

  public function testGetPrice() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $oauth_client_stub->expects($this->any())
             ->method('consumerGet')
             ->will($this->returnValue(self::PRICE));

    $cf = new CultureFeed($oauth_client_stub);

    $price = $cf->uitpas()->getPrice();

    $this->assertEquals(self::PRICE, $price);
  }

  public function testCreatePassholder() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $oauth_client_stub->expects($this->any())
             ->method('consumerPost')
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

  public function testGetWelcomeAdvantagesForPassholder() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $advantages_xml = file_get_contents(dirname(__FILE__) . '/data/passholder/advantages.xml');

    $oauth_client_stub->expects($this->any())
             ->method('consumerGetAsXML')
             ->will($this->returnValue($advantages_xml));

    $cf = new CultureFeed($oauth_client_stub);

    $result = $cf->uitpas()->getWelcomeAdvantagesForPassholder(self::UITPAS_NUMBER);

    $this->assertEquals(2, $result->total);

    $advantages = $result->objects;

    $this->assertInternalType('array', $advantages);
    $this->assertEquals(2, count($advantages));
    $this->assertContainsOnly('CultureFeed_Uitpas_Passholder_WelcomeAdvantage', $advantages);

    $this->assertEquals(5, $advantages[0]->id);
    $this->assertEquals('Gratis armbandjes', $advantages[0]->title);
    $this->assertEquals(0, $advantages[0]->points);
    $this->assertEquals(false, $advantages[0]->cashedIn);

    $this->assertEquals(3, $advantages[1]->id);
    $this->assertEquals('Gekleurde lampjes', $advantages[1]->title);
    $this->assertEquals(2, $advantages[1]->points);
    $this->assertEquals(true, $advantages[1]->cashedIn);
  }

  public function testCheckinPassholder() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $checkin_xml = file_get_contents(dirname(__FILE__) . '/data/passholder/checkin.xml');

    $oauth_client_stub->expects($this->any())
             ->method('consumerPostAsXml')
             ->will($this->returnValue($checkin_xml));

    $cf = new CultureFeed($oauth_client_stub);

    $event = new CultureFeed_Uitpas_Passholder_Event();
    $event->uitpas_number = self::UITPAS_NUMBER;

    $points = $cf->uitpas()->checkinPassholder($event);

    $this->assertEquals(self::POINTS, $points);
  }

  public function testCashInWelcomeAdvantage() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $promotion_xml = file_get_contents(dirname(__FILE__) . '/data/passholder/promotion.xml');

    $oauth_client_stub->expects($this->any())
             ->method('consumerPostAsXml')
             ->will($this->returnValue($promotion_xml));

    $cf = new CultureFeed($oauth_client_stub);

    $promotion = $cf->uitpas()->cashInWelcomeAdvantage(self::UITPAS_NUMBER, self::CONSUMER_KEY_COUNTER, self::WELCOME_ADVANTAGE_ID);
    $this->assertEquals(5, $promotion->id);
    $this->assertEquals('Gratis armbandjes', $promotion->title);
    $this->assertEquals(0, $promotion->points);
    $this->assertEquals(true, $promotion->cashedIn);
  }
}