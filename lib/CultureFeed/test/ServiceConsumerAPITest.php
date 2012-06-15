<?php

/**
 * @todo test if Culturefeed methods let through CultureFeed_HttpException 421 existing service consumer with the same consumerKey
 * @todo test if CultureFeed methods let through CultureFeed_HttpException 403 permission denied
 * @todo test CultureFeed::updateServiceConsumer()
 */
class CultureFeed_ServiceConsumerAPITest extends PHPUnit_Framework_TestCase {

  public function testGetServiceConsumers() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');

    $consumers_xml = file_get_contents(dirname(__FILE__) . '/data/consumer/list.xml');

    $oauth_client_stub->expects($this->any())
             ->method('consumerGetAsXML')
             ->will($this->returnValue($consumers_xml));

    $cf = new CultureFeed($oauth_client_stub);

    $result = $cf->getServiceConsumers();

    $this->assertEquals(2, $result->total);

    $consumers = $result->objects;

    $this->assertInternalType('array', $consumers);
    $this->assertEquals(2, count($consumers));
    $this->assertContainsOnly('CultureFeed_Consumer', $consumers);

    $this->assertEquals('ay1dgdxusn52hgzlnmqlebmaejytpm5l', $consumers[0]->consumerKey);
    $this->assertEquals('una4equafq25xcg8r0po0z8lvh8d1bbi', $consumers[0]->consumerSecret);
    $this->assertEquals('', $consumers[0]->callback);
    $this->assertInternalType('integer', $consumers[0]->creationDate);
    $this->assertEquals(1316701860, $consumers[0]->creationDate);
    $this->assertInternalType('integer', $consumers[0]->id);
    $this->assertEquals(17, $consumers[0]->id);
    $this->assertEquals('Example Corp.', $consumers[0]->name);
    $this->assertEquals('Example Corp. CultureFeed consumer', $consumers[0]->description);
    $this->assertEquals('', $consumers[0]->logo);
    $this->assertEquals('ACTIVE', $consumers[0]->status);
    $this->assertEquals('example.com', $consumers[0]->domain);
    $this->assertEquals('', $consumers[0]->destinationAfterEmailVerification);
    $this->assertEquals(array(1,2), $consumers[0]->group);

    $this->assertEquals('wad324omxeegtejdp7ycqgiak6z78scm', $consumers[1]->consumerKey);
    $this->assertEquals('8r6b2o5zy1irnnt5ltds7kio6ozmh6nh', $consumers[1]->consumerSecret);
    $this->assertEquals('https://clone.example.com/callback', $consumers[1]->callback);
    $this->assertInternalType('integer', $consumers[1]->creationDate);
    $this->assertEquals(1318062600, $consumers[1]->creationDate);
    $this->assertInternalType('integer', $consumers[1]->id);
    $this->assertEquals(18, $consumers[1]->id);
    $this->assertEquals('Clone of Example Corp.', $consumers[1]->name);
    $this->assertEquals('', $consumers[1]->description);
    $this->assertEquals('', $consumers[1]->logo);
    $this->assertEquals('BLOCKED', $consumers[1]->status);
    $this->assertEquals('clone.example.com', $consumers[1]->domain);
    $this->assertEquals('https://clone.example.com/after/verification/email', $consumers[1]->destinationAfterEmailVerification);
    $this->assertEquals(array(3), $consumers[1]->group);
  }

  public function testCreateServiceConsumer() {
    $oauth_client_stub = $this->getMock('CultureFeed_OAuthClient');
    $cf = new Culturefeed($oauth_client_stub);

    $consumerKey = 'y1j72kx0btua10otvftd25cf6mws39pg';
    $consumerSecret = 'o86hyiffn254i3v26o0pqiononc4yw0v';
    $callback = 'http://example.com/callback';
    $description = 'Consumer for Example Corp.';
    $logo = 'http://example.com/logo.png';
    $domain = 'example.com';
    $name = 'Example.com';
    $destinationAfterEmailVerification = 'http://example.com/after/verification/email';

    $empty_consumer_xml = file_get_contents(dirname(__FILE__) . '/data/consumer/empty_consumer.xml');
    $element = new SimpleXMLElement($empty_consumer_xml);

    $element->addChild('consumerKey', $consumerKey);
    $element->addChild('consumerSecret', $consumerSecret);
    $element->addChild('callback', $callback);
    $element->addChild('domain', $domain);
    $element->addChild('description', $description);
    $element->addChild('logo', $logo);
    $element->addChild('name', $name);
    $element->addChild('destinationAfterEmailVerification', $destinationAfterEmailVerification);
    $element->addChild('creationDate', '2011-10-09T16:00Z');
    $element->addChild('id', 27);
    $element->addChild('status', 'ACTIVE');

    $xml = $element->asXML();

    $oauth_client_stub->expects($this->once())
                      ->method('consumerPostAsXml')
                      ->with(
                        $this->equalTo('serviceconsumer'),
                        $this->equalTo(array(
                          'consumerKey' => $consumerKey,
                          'consumerSecret' => $consumerSecret,
                          'callback' => $callback,
                          'domain' => $domain,
                          'description' => $description,
                          'logo' => $logo,
                          'name' => $name,
                          'destinationAfterEmailVerification' => $destinationAfterEmailVerification,
                        ))
                      )
                      ->will($this->returnValue($xml));

    $consumer = new CultureFeed_Consumer();

    $consumer->consumerKey = 'y1j72kx0btua10otvftd25cf6mws39pg';
    $consumer->consumerSecret = 'o86hyiffn254i3v26o0pqiononc4yw0v';

    $consumer->domain = $domain;
    $consumer->name = $name;
    $consumer->callback = $callback;
    $consumer->description = $description;
    $consumer->logo = $logo;
    $consumer->destinationAfterEmailVerification = "http://example.com/after/verification/email";
    // The following properties shouldn't be passed to the server, we add them so we can test
    // that they are not set in the new service consumer
    $consumer->id = 1;
    // 2011-10-09T14:00Z
    $consumer->creationDate = 1318168800;
    $consumer->status = 'BLOCKED';

    try {
      $new_consumer = $cf->createServiceConsumer($consumer);
    }
    catch (CultureFeed_HttpException $e) {
      $this->fail('CultureFeed::createServiceConsumer() failed, HTTP status ' . $e->getCode() . ', message: ' . $e->getMessage());
    }

    $this->assertInstanceOf('CultureFeed_Consumer', $new_consumer);

    $this->assertEquals($consumer->name, $new_consumer->name);
    $this->assertEquals($consumer->callback, $new_consumer->callback);
    $this->assertEquals($consumer->consumerKey, $new_consumer->consumerKey);
    $this->assertEquals($consumer->domain, $new_consumer->domain);
    $this->assertEquals($consumer->description, $new_consumer->description);
    $this->assertEquals($consumer->logo, $new_consumer->logo);
    $this->assertEquals($consumer->destinationAfterEmailVerification, $new_consumer->destinationAfterEmailVerification);

    // properties that differ

    // creationDate should be 2011-10-09T16:00Z as returned by the oauth_client
    $this->assertInternalType('integer', $new_consumer->creationDate);
    $this->assertEquals(1318176000, $new_consumer->creationDate);

    // id as returned by the oauth_client
    $this->assertInternalType('integer', $new_consumer->id);
    $this->assertEquals(27, $new_consumer->id);

    // status as returned by the oauth_client
    $this->assertEquals('ACTIVE', $new_consumer->status);
  }
}