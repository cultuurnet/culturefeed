<?php
/**
 * @file
 */

class CultureFeed_Uitpas_SearchCheckinsTest extends PHPUnit_Framework_TestCase {

  public function testSearchCheckins() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $checkins_xml = file_get_contents(dirname(__FILE__) . '/data/cultureevent/searchCheckins.xml');

    $oauth_client_stub->expects($this->any())
      ->method('consumerGetAsXml')
      ->will($this->returnValue($checkins_xml));

    $cf = new CultureFeed($oauth_client_stub);

    $query = new CultureFeed_Uitpas_Event_Query_SearchCheckinsOptions();
    $checkins = $cf->uitpas()->searchCheckins($query, NULL, CultureFeed_Uitpas::CONSUMER_REQUEST);

    $this->assertEquals(12, $checkins->total);

    $this->assertInternalType('array', $checkins->objects);

    $this->assertContainsOnly('CultureFeed_Uitpas_Event_CheckinActivity', $checkins->objects, FALSE);

    /** @var CultureFeed_Uitpas_Event_CheckinActivity $checkin */
    $checkin = reset($checkins->objects);

    $this->assertEquals(12, $checkin->type);
    $this->assertEquals('event', $checkin->contentType);
    $this->assertEquals('CC De Werf', $checkin->createdVia);
    //$this->assertEquals(, $checkin->creationDate);
    $this->assertEquals('http://media.uitid.be/fis/rest/download/ce126667652776f0e9e55160f12f5478/uiv/default.png', $checkin->depiction);
    $this->assertEquals('76faa534-4c3e-4f2e-abe5-10bd0a2e5e14', $checkin->id);
    $this->assertEquals('Frontend Tester', $checkin->nick);
    $this->assertEquals('c44d849a-1290-4927-81ac-e782428de887', $checkin->nodeId);
    $this->assertEquals('test cine palace', $checkin->nodeTitle);
    $this->assertEquals(0, $checkin->points);
    $this->assertEquals(FALSE, $checkin->private);
    $this->assertEquals('a81b1741-5e97-4eee-ab30-a71865fc266a', $checkin->userId);
    $this->assertEquals('Frontend', $checkin->firstName);
    $this->assertEquals('FEMALE', $checkin->gender);
    $this->assertEquals(FALSE, $checkin->kansenStatuut);
    $this->assertEquals('Tester', $checkin->lastName);
    $this->assertEquals('Aalst', $checkin->location);
    $this->assertEquals('CC De Werf', $checkin->organiser);
    $this->assertInternalType('array', $checkin->organiserCardSystems);

    $this->assertContainsOnly('integer', $checkin->organiserCardSystems, TRUE);
    $this->assertCount(2, $checkin->organiserCardSystems);
    $this->assertEquals(array(8, 5), $checkin->organiserCardSystems);

    $this->assertEquals('Brussel', $checkin->userHomeCity);
    $this->assertEquals(50.8299126, $checkin->userHomeLocationLat);
    $this->assertEquals(4.3464309, $checkin->userHomeLocationLon);
    $this->assertEquals(10, $checkin->userPoints);

    // @todo Check properties of the other items as well, one by one.
  }
}
