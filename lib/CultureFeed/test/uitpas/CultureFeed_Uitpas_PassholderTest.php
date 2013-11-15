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
    $this->assertNull($cardsystemSpecific->currentCard);

    $cardsystemSpecific = $passholder->cardSystemSpecific[6];
    $this->assertInstanceOf('CultureFeed_Uitpas_Passholder_Card', $cardsystemSpecific->currentCard);

    $this->assertEquals(FALSE, $cardsystemSpecific->currentCard->kansenpas);
    $this->assertEquals('ACTIVE', $cardsystemSpecific->currentCard->status);
    $this->assertEquals('1000001500601', $cardsystemSpecific->currentCard->uitpasNumber);
    $this->assertNull($cardsystemSpecific->currentCard->city);
  }
}
