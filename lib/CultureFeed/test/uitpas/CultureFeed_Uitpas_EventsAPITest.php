<?php

class CultureFeed_Uitpas_EventsAPITest extends PHPUnit_Framework_TestCase {

  const PRICE = 15;
  const CDBID = "94305b2e-e7ff-4dfc-8d96-ef4d43de9038";
  const WELCOME_ADVANTAGE_ID = 1;
  const UITPAS_NUMBER = "0930011111208";
  const CHIP_NUMBER = "8473847";
  const CONSUMER_KEY_COUNTER = "94305r2e-e7ff-4dfc-8dd6-ef4d43de9098";
  const POINTS = 2;

  public function testRegisterTicketSale() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $register_xml = file_get_contents(dirname(__FILE__) . '/data/events/register.xml');

    $oauth_client_stub->expects($this->any())
             ->method('authenticatedPostAsXml')
             ->will($this->returnValue($register_xml));

    $cf = new CultureFeed($oauth_client_stub);

    $ticket_sale = $cf->uitpas()->registerTicketSale(self::UITPAS_NUMBER, self::CDBID, self::CONSUMER_KEY_COUNTER);
    $this->assertEquals(7, $ticket_sale->id);
    $this->assertEquals(1322825015, $ticket_sale->creationDate);
    $this->assertEquals(45.0, $ticket_sale->price);
  }
}