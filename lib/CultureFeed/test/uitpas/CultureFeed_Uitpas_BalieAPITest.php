<?php
/**
 * @file
 */

class CultureFeed_Uitpas_BalieAPITest extends PHPUnit_Framework_TestCase {

  public function testSearchCountersForMember() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $uid = 'e52efb7f-2eab-47a5-9cf3-9e7413ffd942';

    $xml = file_get_contents(dirname(__FILE__) . '/data/balie/list.xml');

    $oauth_client_stub
      ->expects($this->once())
      ->method('authenticatedGetAsXml')
      ->with('uitpas/balie/list', array(
          'uid' => $uid,
        ))
      ->will($this->returnValue($xml));

    $cf = new CultureFeed($oauth_client_stub);

    $counters = $cf->uitpas()->searchCountersForMember($uid);

    $this->assertInstanceOf('CultureFeed_ResultSet', $counters);
    $this->assertEquals(18, $counters->total);

    $this->assertInternalType('array', $counters->objects);
    $this->assertCount(18, $counters->objects);
    $this->assertContainsOnly('CultureFeed_Uitpas_Counter_Employee', $counters->objects);

    /* @var CultureFeed_Uitpas_Counter_Employee $counter */
    $counter = reset($counters->objects);

    $this->assertEquals(11, $counter->id);
    $this->assertEquals('\'t Gasthuys - Stedelijk Museum Aalst', $counter->name);
    $this->assertEquals('5c9c73d3-e82f-e7b3-44161e6e3802e64f', $counter->consumerKey);
    $this->assertEquals('admin', $counter->role);
    $this->assertEquals('5c9c73d3-e82f-e7b3-44161e6e3802e64f', $counter->actorId);

    $this->assertInternalType('array', $counter->cardSystems);
    $this->assertContainsOnly('CultureFeed_Uitpas_Counter_EmployeeCardSystem', $counter->cardSystems);
    $this->assertCount(1, $counter->cardSystems);

    /* @var CultureFeed_Uitpas_Counter_EmployeeCardSystem $card_system */
    $card_system = reset($counter->cardSystems);

    $this->assertEquals(1, $card_system->id);
    $this->assertEquals('HELA', $card_system->name);
    $this->assertInternalType('array', $card_system->groups);
    $this->assertCount(1, $card_system->groups);
    $this->assertContainsOnly('string', $card_system->groups);
    $this->assertEquals('Niet-geauthorizeerde registratie balies', reset($card_system->groups));

    $this->assertInternalType('array', $card_system->permissions);
    $this->assertCount(1, $card_system->permissions);
    $this->assertContainsOnly('string', $card_system->permissions);
    $this->assertEquals('registratie', reset($card_system->permissions));
  }

  public function testGetCardCounts() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $xml = file_get_contents(dirname(__FILE__) . '/data/balie/countCards.xml');

    $oauth_client_stub
      ->expects($this->once())
      ->method('authenticatedGetAsXml')
      ->with('uitpas/balie/countCards')
      ->will($this->returnValue($xml));

    $cf = new CultureFeed($oauth_client_stub);

    $cardCounters = $cf->uitpas()->getCardCounters();

    $this->assertInternalType('array', $cardCounters);
    $this->assertCount(4, $cardCounters);

    /** @var CultureFeed_Uitpas_Counter_CardCounter $cardCounter */
    $cardCounter = reset($cardCounters);
    $this->assertInstanceOf('CultureFeed_Uitpas_CardSystem', $cardCounter->cardSystem);
    $this->assertEquals('HELA', $cardCounter->cardSystem->name);
    $this->assertEquals(1, $cardCounter->cardSystem->id);
    $this->assertFalse($cardCounter->kansenstatuut);
    $this->assertEquals('SENT_TO_BALIE', $cardCounter->status);
    $this->assertEquals(22, $cardCounter->count);

    $cardCounter = next($cardCounters);
    $cardCounter = reset($cardCounters);
    $this->assertInstanceOf('CultureFeed_Uitpas_CardSystem', $cardCounter->cardSystem);
    $this->assertEquals('HELA', $cardCounter->cardSystem->name);
    $this->assertEquals(1, $cardCounter->cardSystem->id);
    $this->assertTrue($cardCounter->kansenstatuut);
    $this->assertEquals('SENT_TO_BALIE', $cardCounter->status);
    $this->assertEquals(33, $cardCounter->count);

    $cardCounter = reset($cardCounters);
    $this->assertInstanceOf('CultureFeed_Uitpas_CardSystem', $cardCounter->cardSystem);
    $this->assertEquals('HELA', $cardCounter->cardSystem->name);
    $this->assertEquals(1, $cardCounter->cardSystem->id);
    $this->assertFalse($cardCounter->kansenstatuut);
    $this->assertEquals('LOCAL_STOCK', $cardCounter->status);
    $this->assertEquals(3, $cardCounter->count);

    $cardCounter = reset($cardCounters);
    $this->assertInstanceOf('CultureFeed_Uitpas_CardSystem', $cardCounter->cardSystem);
    $this->assertEquals('HELA', $cardCounter->cardSystem->name);
    $this->assertEquals(1, $cardCounter->cardSystem->id);
    $this->assertTrue($cardCounter->kansenstatuut);
    $this->assertEquals('LOCAL_STOCK', $cardCounter->status);
    $this->assertEquals(4, $cardCounter->count);
  }
} 
