<?php
/**
 *
 */
class CultureFeed_Uitpas_PassholderTest extends PHPUnit_Framework_TestCase {

  /**
   * @var CultureFeed_Uitpas_Passholder
   */
  protected $passholder;

  public function setUp() {
    $this->passholder = new CultureFeed_Uitpas_Passholder();

    $this->passholder->schoolConsumerKey = '111';
    $this->passholder->gender = 'M';
    $this->passholder->kansenStatuut = FALSE;
    $this->passholder->street = '';
  }

  public function testToPostData() {
    $postData = $this->passholder->toPostData();

    $this->assertArrayNotHasKey('street', $postData);

    $this->assertArrayHasKey('schoolConsumerKey', $postData);
    $this->assertEquals($this->passholder->schoolConsumerKey, $postData['schoolConsumerKey']);

    $this->assertArrayHasKey('gender', $postData);
    $this->assertEquals($this->passholder->gender, $postData['gender']);

    $this->assertArrayHasKey('kansenStatuut', $postData);
    $this->assertInternalType('string', $postData['kansenStatuut']);
    $this->assertEquals('false', $postData['kansenStatuut']);

    $this->passholder->kansenStatuut = TRUE;

    $postData = $this->passholder->toPostData();

    $this->assertArrayHasKey('kansenStatuut', $postData);
    $this->assertInternalType('string', $postData['kansenStatuut']);
    $this->assertEquals('true', $postData['kansenStatuut']);
  }

  public function testToPostDataDropsEmptyPropertiesByDefault() {
    $this->passholder->schoolConsumerKey = '';

    $postData = $this->passholder->toPostData();

    $this->assertArrayNotHasKey('schoolConsumerKey', $postData);
  }

  public function testKeepEmptySchoolConsumerKeyWhenSpecified() {
    $this->passholder->schoolConsumerKey = '';
    $this->passholder->toPostDataKeepEmptySchoolConsumerKey();

    $postData = $this->passholder->toPostData();

    $this->assertArrayNotHasKey('postDataEmptyPropertiesToKeep', $postData);

    $this->assertArrayHasKey('schoolConsumerKey', $postData);
    $this->assertEquals($this->passholder->schoolConsumerKey, $postData['schoolConsumerKey']);

    $this->passholder->toPostDataKeepEmptySchoolConsumerKey(FALSE);

    $postData = $this->passholder->toPostData();

    $this->assertArrayNotHasKey('schoolConsumerKey', $postData);
  }

  public function testCreateFromXML() {
    $xml = file_get_contents(dirname(__FILE__) . '/data/passholder.xml');
    $simple_xml = new CultureFeed_SimpleXMLElement($xml);

    $passholder = CultureFeed_Uitpas_Passholder::createFromXML($simple_xml);

    $this->assertInstanceOf('CultureFeed_Uitpas_Passholder', $passholder);

    $this->assertInternalType('array', $passholder->cardSystemSpecific);
    $this->assertCount(2, $passholder->cardSystemSpecific);

    $keys = array_keys($passholder->cardSystemSpecific);
    $this->assertEquals(array(4,6), $keys);

    $this->assertContainsOnly('CultureFeed_Uitpas_Passholder_CardSystemSpecific', $passholder->cardSystemSpecific);

    $cardsystemSpecific = $passholder->cardSystemSpecific[4];
    $this->assertEquals(CultureFeed_Uitpas_Passholder::EMAIL_ALL_MAILS, $cardsystemSpecific->emailPreference);
    $this->assertEquals(CultureFeed_Uitpas_Passholder::SMS_NO_SMS, $cardsystemSpecific->smsPreference);
    $this->assertNull($cardsystemSpecific->currentCard);

    $cardsystemSpecific = $passholder->cardSystemSpecific[6];
    $this->assertEquals(CultureFeed_Uitpas_Passholder::EMAIL_NO_MAILS, $cardsystemSpecific->emailPreference);
    $this->assertEquals(CultureFeed_Uitpas_Passholder::SMS_ALL_SMS, $cardsystemSpecific->smsPreference);
    $this->assertInstanceOf('CultureFeed_Uitpas_Passholder_Card', $cardsystemSpecific->currentCard);

    $this->assertEquals(FALSE, $cardsystemSpecific->currentCard->kansenpas);
    $this->assertEquals('ACTIVE', $cardsystemSpecific->currentCard->status);
    $this->assertEquals('1000001500601', $cardsystemSpecific->currentCard->uitpasNumber);
    $this->assertNull($cardsystemSpecific->currentCard->city);

    $this->assertEquals('BRUSSEL', $passholder->city);
    $this->assertEquals(378691200, $passholder->dateOfBirth);
    $this->assertEquals('Frontend', $passholder->firstName);
    $this->assertEquals('FEMALE', $passholder->gender);
    $this->assertEquals('62cb6cc2a58d1b23d85b5993894c2fcd2d55ed01c0f6ff1f4f0ee87ac2b83dd9', $passholder->inszNumberHash);
    $this->assertInternalType('array', $passholder->memberships);
    $this->assertCount(0, $passholder->memberships);
    $this->assertEquals('Tester', $passholder->name);
    $this->assertEquals(2, $passholder->numberOfCheckins);
    $this->assertEquals(10, $passholder->points);
    $this->assertEquals(1060, $passholder->postalCode);
    $this->assertEquals('3456548c761dfbf68bb4474e69ac8a67', $passholder->registrationBalieConsumerKey);
    $this->assertInstanceOf('CultureFeed_Uitpas_Passholder_UitIdUser', $passholder->uitIdUser);

    $this->assertEquals('a81b1741-5e97-4eee-ab30-a71865fc266a', $passholder->uitIdUser->id);
    $this->assertEquals('Frontend Tester', $passholder->uitIdUser->nick);
    $this->assertEquals(FALSE, $passholder->verified);
  }
}
