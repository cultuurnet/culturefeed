<?php
/**
 * @file
 * PHPUnit test for searching events.
 */

class CultureFeed_Uitpas_SearchEventsTest extends PHPUnit_Framework_TestCase {

  /**
   * Test the searching events.
   */
  public function testSearchEvents() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $get_xml = file_get_contents(dirname(__FILE__) . '/data/events/searchresults.xml');

    $oauth_client_stub->expects($this->once())
             ->method('consumerGetAsXML')
             ->will($this->returnValue($get_xml));

    $cf = new CultureFeed($oauth_client_stub);

    $query = new CultureFeed_Uitpas_Event_Query_SearchEventsOptions();

    $data = $cf->uitpas()->searchEvents($query);

    $this->assertContainsOnly('CultureFeed_Uitpas_Event_CultureEvent', $data->objects);
    $this->assertCount(7, $data->objects);
    $this->assertEquals("8ea1787b-d08c-4fa8-8d3c-20d1e5cc6e6a", $data->objects[2]->cdbid);
  }
}
