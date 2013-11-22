<?php

class CultureFeed_Uitpas_PasHoudersAPITest extends PHPUnit_Framework_TestCase {

  const PRICE = 15;
  const UID = "94305b2e-e7ff-4dfc-8d96-ef4d43de9038";
  const WELCOME_ADVANTAGE_ID = 1;
  const UITPAS_NUMBER = "0930011111208";
  const CHIP_NUMBER = "8473847";
  const CONSUMER_KEY_COUNTER = "94305r2e-e7ff-4dfc-8dd6-ef4d43de9098";
  const POINTS = 2;

  public function testGetPrice() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $prices_xml = file_get_contents(dirname(__FILE__) . '/data/passholder/prices.xml');

    $oauth_client_stub->expects($this->any())
             ->method('authenticatedGetAsXml')
             ->will($this->returnValue($prices_xml));

    $cf = new CultureFeed($oauth_client_stub);

    $prices = $cf->uitpas()->getPrice(self::CONSUMER_KEY_COUNTER);

    $this->assertEquals(2, count($prices->objects));
    $this->assertContainsOnly('CultureFeed_Uitpas_Passholder_UitpasPrice', $prices->objects);

    $this->assertEquals(3, $prices->objects[0]->id);
    $this->assertEquals("LOSS_THEFT", $prices->objects[0]->reason);
    $this->assertEquals(FALSE, $prices->objects[0]->kansenStatuut);
    $this->assertEquals(20.5, $prices->objects[0]->price);
  }

  public function testCreatePassholder() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $create_xml = file_get_contents(dirname(__FILE__) . '/data/passholder/create.xml');

    $oauth_client_stub->expects($this->any())
             ->method('authenticatedPostAsXml')
             ->will($this->returnValue($create_xml));

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
             ->method('authenticatedGetAsXml')
             ->will($this->returnValue($advantages_xml));

    $cf = new CultureFeed($oauth_client_stub);

    $query = new CultureFeed_Uitpas_Passholder_Query_WelcomeAdvantagesOptions();
    $query->uitpasNumber = self::UITPAS_NUMBER;
    $result = $cf->uitpas()->getWelcomeAdvantagesForPassholder($query);

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
             ->method('authenticatedPostAsXml')
             ->will($this->returnValue($checkin_xml));

    $cf = new CultureFeed($oauth_client_stub);

    $event = new CultureFeed_Uitpas_Passholder_Query_CheckInPassholderOptions();
    $event->uitpas_number = self::UITPAS_NUMBER;

    $points = $cf->uitpas()->checkinPassholder($event);

    $this->assertEquals(self::POINTS, $points);
  }

  public function testCashInWelcomeAdvantage() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $promotion_xml = file_get_contents(dirname(__FILE__) . '/data/passholder/promotion.xml');

    $oauth_client_stub->expects($this->any())
             ->method('authenticatedPostAsXml')
             ->will($this->returnValue($promotion_xml));

    $cf = new CultureFeed($oauth_client_stub);

    $promotion = $cf->uitpas()->cashInWelcomeAdvantage(self::UITPAS_NUMBER, self::CONSUMER_KEY_COUNTER, self::WELCOME_ADVANTAGE_ID);
    $this->assertEquals(5, $promotion->id);
    $this->assertEquals('Gratis armbandjes', $promotion->title);
    $this->assertEquals(0, $promotion->points);
    $this->assertEquals(true, $promotion->cashedIn);
  }

  public function testGetPromotionPoints() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $advantages_xml = file_get_contents(dirname(__FILE__) . '/data/passholder/promotion_points.xml');

    $oauth_client_stub->expects($this->any())
             ->method('consumerGetAsXML')
             ->will($this->returnValue($advantages_xml));

    $cf = new CultureFeed($oauth_client_stub);

    $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
    $query->uitpasUid = self::UID;
    $query->balieConsumerKey = self::CONSUMER_KEY_COUNTER;
    $query->sort = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::SORT_POINTS;
    $query->order = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::ORDER_ASC;

    $result = $cf->uitpas()->getPromotionPoints($query);

    $this->assertEquals(2, $result->total);

    $promotions = $result->objects;

    $this->assertInternalType('array', $promotions);
    $this->assertEquals(2, count($promotions));
    $this->assertContainsOnly('CultureFeed_Uitpas_Passholder_PointsPromotion', $promotions);

    /** @var CultureFeed_Uitpas_Passholder_PointsPromotion $promotion **/
    $promotion = reset($promotions);

    // If the mapping of 1 object is correct, all objects are correctly mapped
    $this->assertEquals(7, $promotion->id);
    $this->assertEquals(5, $promotion->points);
    $this->assertEquals('Gratis stickers', $promotion->title);
    $this->assertEquals(false, $promotion->cashedIn);
    $this->assertEquals(1323945210, $promotion->creationDate);
    $this->assertEquals(1262304000, $promotion->cashingPeriodBegin);
    $this->assertEquals(1451606399, $promotion->cashingPeriodEnd);
    $this->assertEquals(array('Aalst', 'Erpe_Mere', 'Haaltert'), $promotion->validForCities);
    $this->assertEquals(2, $promotion->maxAvailableUnits);
    $this->assertEquals(2, $promotion->unitsTaken);

    $this->assertContainsOnly('CultureFeed_Uitpas_Passholder_Counter', $promotion->counters);
    $this->assertEquals(3, $promotion->counters[0]->id);
    $this->assertEquals("De Werf", $promotion->counters[0]->name);
    $this->assertEquals(2, $promotion->counters[1]->id);
    $this->assertEquals("Scouts Aalst", $promotion->counters[1]->name);

    $this->assertInstanceOf('CultureFeed_Uitpas_CardSystem', $promotion->owningCardSystem);
    $this->assertEquals(1, $promotion->owningCardSystem->id);
    $this->assertEquals('HELA', $promotion->owningCardSystem->name);

    $this->assertInternalType('array', $promotion->applicableCardSystems);
    $this->assertCount(2, $promotion->applicableCardSystems);
    $this->assertContainsOnly('Culturefeed_Uitpas_CardSystem', $promotion->applicableCardSystems);

    /** @var CultureFeed_Uitpas_CardSystem $applicableCardSystem */
    $applicableCardSystem = reset($promotion->applicableCardSystems);
    $this->assertEquals(1, $applicableCardSystem->id);
    $this->assertEquals('HELA', $applicableCardSystem->name);

    $applicableCardSystem = next($promotion->applicableCardSystems);
    $this->assertEquals(3, $applicableCardSystem->id);
    $this->assertEquals('Test cardsystem', $applicableCardSystem->name);

    $promotion = next($promotions);

    $this->assertInternalType('array', $promotion->applicableCardSystems);
    $this->assertCount(0, $promotion->applicableCardSystems);

    $this->assertNull($promotion->owningCardSystem);
  }

  public function testCashInPromotionPoints() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $promotion_xml = file_get_contents(dirname(__FILE__) . '/data/passholder/promotion_checkin.xml');

    $oauth_client_stub->expects($this->any())
             ->method('authenticatedPostAsXml')
             ->will($this->returnValue($promotion_xml));

    $cf = new CultureFeed($oauth_client_stub);

    $promotion = $cf->uitpas()->cashInPromotionPoints(self::UITPAS_NUMBER, self::WELCOME_ADVANTAGE_ID, self::CONSUMER_KEY_COUNTER);
    $this->assertEquals(3, $promotion->id);
    $this->assertEquals('Gratis broodje', $promotion->title);
    $this->assertEquals(0, $promotion->points);
    $this->assertEquals(true, $promotion->cashedIn);
    $this->assertEquals("De Werf", $promotion->counters[0]->name);
  }

  public function testBlockUitpas() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $block_xml = file_get_contents(dirname(__FILE__) . '/data/passholder/block.xml');

    $oauth_client_stub->expects($this->any())
             ->method('authenticatedPostAsXml')
             ->will($this->returnValue($block_xml));

    $cf = new CultureFeed($oauth_client_stub);

    $response = $cf->uitpas()->blockUitpas(self::UITPAS_NUMBER, self::CONSUMER_KEY_COUNTER);
    $this->assertEquals('BLOCK_UITPAS_SUCCESS', $response->code);
    $this->assertEquals('The uitpas has been blocked.', $response->message);
  }

  public function testSearchWelcomeAdvantages() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $advantages_xml = file_get_contents(dirname(__FILE__) . '/data/passholder/welcome_advantages.xml');

    $oauth_client_stub->expects($this->any())
             ->method('consumerGetAsXML')
             ->will($this->returnValue($advantages_xml));

    $cf = new CultureFeed($oauth_client_stub);

    $query = new CultureFeed_Uitpas_Promotion_Query_WelcomeAdvantagesOptions();
    $result = $cf->uitpas()->searchWelcomeAdvantages($query);

    $this->assertEquals(2, $result->total);

    $promotions = $result->objects;

    $this->assertInternalType('array', $promotions);
    $this->assertEquals(2, count($promotions));
    $this->assertContainsOnly('CultureFeed_Uitpas_Passholder_WelcomeAdvantage', $promotions);

    $this->assertEquals(8, $promotions[0]->id);
    $this->assertEquals(0, $promotions[0]->points);
    $this->assertEquals("Gratis deelname Zumba", $promotions[0]->title);
    $this->assertEquals(1326180281, $promotions[0]->creationDate);
    $this->assertEquals(array('Aalst'), $promotions[0]->validForCities);
    $this->assertEquals(0, $promotions[0]->unitsTaken);
  }

  public function testGetPassholderForChipNumber() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $chip_xml = file_get_contents(dirname(__FILE__) . '/data/passholder/chip_number.xml');

    $oauth_client_stub->expects($this->any())
             ->method('authenticatedGetAsXML')
             ->will($this->returnValue($chip_xml));

    $cf = new CultureFeed($oauth_client_stub);
    $card = $cf->uitpas()->getPassholderForChipNumber(self::CHIP_NUMBER, self::CONSUMER_KEY_COUNTER);

    $this->assertInstanceOf('CultureFeed_Uitpas_CardInfo', $card);
    $this->assertEquals(self::UITPAS_NUMBER, $card->uitpasNumber);
    $this->assertEquals('ACTIVE', $card->status);
    $this->assertInstanceOf('CultureFeed_Uitpas_CardSystem', $card->cardSystem);
    $this->assertEquals(6, $card->cardSystem->id);
    $this->assertEquals('Testsysteem Paspartoe', $card->cardSystem->name);
  }
}
