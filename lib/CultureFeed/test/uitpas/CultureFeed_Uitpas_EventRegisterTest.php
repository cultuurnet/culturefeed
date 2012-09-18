<?php
/**
 * @file
 * PHPUnit Testing the registering of an event.
 */

class CultureFeed_Uitpas_EventsRegisterTest extends PHPUnit_Framework_TestCase {

  const EVENTXML = "/data/events/event.xml";

  /**
   * Test the registering of an event.
   */
  public function testRegisterEvent() {

    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');
    $event_xml_str = file_get_contents(dirname(__FILE__) . self::EVENTXML);
    $event_xml_obj = new CultureFeed_SimpleXMLElement($event_xml_str);

    $event = CultureFeed_Uitpas_Event_CultureEvent::createFromXML($event_xml_obj);

    $this->assertInstanceOf('CultureFeed_Uitpas_Event_CultureEvent', $event);
    $this->assertEquals("9ba1b072-40ea-41b6-a66b-ac3fdf646f36", $event->cdbid);
    $this->assertEquals("5C9C73D3-E82F-E7B3-44161E6E3802E64F", $event->locationId);

    $cf = new CultureFeed($oauth_client_stub);

    $response = $cf->uitpas()->registerEvent($event);
  }

}
