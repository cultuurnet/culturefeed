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
    $this->assertInstanceOf('CultureFeed_Uitpas_CardSystem', $prices->objects[0]->cardSystem);
    $this->assertEquals(5, $prices->objects[0]->cardSystem->id);
    $this->assertEquals('Test', $prices->objects[0]->cardSystem->name);
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

  public function testGetCard() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $card_xml = file_get_contents(dirname(__FILE__) . '/data/card.xml');

    $oauth_client_stub->expects($this->any())
             ->method('authenticatedGetAsXML')
             ->with('uitpas/card', array('chipNumber' => self::CHIP_NUMBER))
             ->will($this->returnValue($card_xml));

    $cf = new CultureFeed($oauth_client_stub);
    $query = new CultureFeed_Uitpas_CardInfoQuery();
    $query->chipNumber = self::CHIP_NUMBER;
    $card = $cf->uitpas()->getCard($query);

    $this->assertInstanceOf('CultureFeed_Uitpas_CardInfo', $card);
    $this->assertEquals(self::UITPAS_NUMBER, $card->uitpasNumber);
    $this->assertEquals('ACTIVE', $card->status);
    $this->assertInstanceOf('CultureFeed_Uitpas_CardSystem', $card->cardSystem);
    $this->assertEquals(6, $card->cardSystem->id);
    $this->assertEquals('Testsysteem Paspartoe', $card->cardSystem->name);
  }

  public function testSearch() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $search_xml = file_get_contents(dirname(__FILE__) . '/data/passholder/search.xml');

    $oauth_client_stub
      ->expects($this->any())
      ->method('consumerGetAsXml')
      ->with('uitpas/passholder/search', array('sort' => 'creationDate'))
      ->will($this->returnValue($search_xml));

    $cf = new CultureFeed($oauth_client_stub);

    $query = new CultureFeed_Uitpas_Passholder_Query_SearchPassholdersOptions();

    $results = $cf->uitpas()->searchPassholders($query);

    $this->assertInstanceOf('CultureFeed_ResultSet', $results);

    $this->assertEquals(1851, $results->total);

    $this->assertInternalType('array', $results->objects);
    $this->assertCount(10, $results->objects);
    $this->assertContainsOnly('CultureFeed_Uitpas_Passholder', $results->objects);

    /** @var CultureFeed_Uitpas_Passholder $passholder */
    $passholder = reset($results->objects);

    $this->assertInternalType('array', $passholder->cardSystemSpecific);
    $this->assertCount(1, $passholder->cardSystemSpecific);
    $this->assertContainsOnly('CultureFeed_Uitpas_Passholder_CardSystemSpecific', $passholder->cardSystemSpecific);

    /** @var CultureFeed_Uitpas_Passholder_CardSystemSpecific $cardSystemSpecific */
    $cardSystemSpecific = reset($passholder->cardSystemSpecific);
    $this->assertInstanceOf('CultureFeed_Uitpas_CardSystem', $cardSystemSpecific->cardSystem);
    $this->assertEquals('HELA', $cardSystemSpecific->cardSystem->name);
    $this->assertEquals(1, $cardSystemSpecific->cardSystem->id);
    $this->assertInstanceOf('CultureFeed_Uitpas_Passholder_Card', $cardSystemSpecific->currentCard);
    $this->assertEquals(TRUE, $cardSystemSpecific->currentCard->kansenpas);
    $this->assertEquals('ACTIVE', $cardSystemSpecific->currentCard->status);
    $this->assertEquals('0942000000125', $cardSystemSpecific->currentCard->uitpasNumber);
    $this->assertEquals('ALL_MAILS', $cardSystemSpecific->emailPreference);
    $this->assertEquals(TRUE, $cardSystemSpecific->kansenStatuut);
    $this->assertEquals(1388530799, $cardSystemSpecific->kansenStatuutEndDate);
    $this->assertEquals(FALSE, $cardSystemSpecific->kansenStatuutExpired);
    $this->assertEquals(FALSE, $cardSystemSpecific->kansenStatuutInGracePeriod);
    $this->assertEquals('NO_SMS', $cardSystemSpecific->smsPreference);
    $this->assertEquals('ACTIVE', $cardSystemSpecific->status);

    $this->assertEquals('AALST', $passholder->city);
    $this->assertEquals(1151452800, $passholder->dateOfBirth);
    $this->assertEquals('tadug', $passholder->firstName);
    $this->assertEquals('MALE', $passholder->gender);
    $this->assertEquals('0475/51.87.60', $passholder->gsm);
    $this->assertInternalType('array', $passholder->memberships);
    $this->assertCount(0, $passholder->memberships);
    $this->assertEquals("Nieuwe aanvraag\r OCMW Ja\r Via MvM", $passholder->moreInfo);

    $this->assertEquals('celab', $passholder->name);
    $this->assertEquals('Belg', $passholder->nationality);
    $this->assertEquals(0, $passholder->numberOfCheckins);
    $this->assertEquals('Aalst', $passholder->placeOfBirth);
    $this->assertEquals(3.000, $passholder->points);
    $this->assertEquals(9300, $passholder->postalCode);

    $this->assertEquals('b95d1bcf-533d-45ac-afcd-e015cfe86c84', $passholder->registrationBalieConsumerKey);
    $this->assertEquals('0717a28c-78be-40fc-9ad1-25bc45252f3a', $passholder->schoolConsumerKey);
    $this->assertEquals('opubi 73', $passholder->street);
    $this->assertEquals(FALSE, $passholder->verified);
  }
}
