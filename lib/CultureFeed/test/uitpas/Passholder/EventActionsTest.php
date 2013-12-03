<?php
/**
 * @file
 */

class CultureFeed_Uitpas_Passholder_EventActionsTest extends PHPUnit_Framework_TestCase {

  protected $dataDir;

  protected function setUp() {
    $this->dataDir = dirname(__FILE__) . '/data/eventactions';
  }

  protected function loadEventActions($file) {
    $xml = file_get_contents($this->dataDir. '/' . $file);
    $xml_element = new CultureFeed_SimpleXMLElement($xml);
    $event_actions = CultureFeed_Uitpas_Passholder_EventActions::createFromXML($xml_element);
    return $event_actions;
  }

  public function testCreateFromXML() {
    $event_actions = $this->loadEventActions('get.xml');

    $this->assertInstanceOf('CultureFeed_Uitpas_Passholder_EventActions', $event_actions);

    $this->assertInstanceOf('CultureFeed_Uitpas_Passholder', $event_actions->passholder);
    $this->assertEquals('Hendrickx', $event_actions->passholder->name);
    $this->assertEquals('Anne', $event_actions->passholder->firstName);
    $this->assertEquals(9, $event_actions->passholder->points);
    $this->assertInternalType('string', $event_actions->passholder->uitpasNumber);
    $this->assertEquals('0930000150316', $event_actions->passholder->uitpasNumber);
    $this->assertInternalType('string', $event_actions->passholder->currentCard->uitpasNumber);
    $this->assertEquals('0930000150316', $event_actions->passholder->currentCard->uitpasNumber);
    $this->assertEquals(TRUE, $event_actions->passholder->kansenStatuut);
    $this->assertEquals(TRUE, $event_actions->passholder->currentCard->kansenpas);
    $this->assertEquals(FALSE, $event_actions->passholder->kansenStatuutExpired);

    $this->assertInstanceOf('CultureFeed_Uitpas_Passholder_EventCheckin', $event_actions->eventCheckin);
    $this->assertEquals('27f1e25a-1ba6-4a43-96de-0c6f99b508f3', $event_actions->eventCheckin->cdbid);
    $this->assertEquals(FALSE, $event_actions->eventCheckin->checkinAllowed);
    $this->assertEquals(CultureFeed_Uitpas_Event_CultureEvent::CHECKIN_CONSTRAINT_REASON_MAXIMUM_REACHED, $event_actions->eventCheckin->checkinConstraintReason);
    $this->assertEquals(1, $event_actions->eventCheckin->numberOfPoints);

    $this->assertInstanceOf('CultureFeed_Uitpas_Passholder_EventBuyTicket', $event_actions->eventBuyTicket);
    $this->assertEquals('27f1e25a-1ba6-4a43-96de-0c6f99b508f3', $event_actions->eventBuyTicket->cdbid);
    $this->assertEquals(CultureFeed_Uitpas_Event_CultureEvent::BUY_CONSTRAINT_REASON_MAXIMUM_REACHED, $event_actions->eventBuyTicket->buyConstraintReason);
    $this->assertEquals(15.0, $event_actions->eventBuyTicket->price);
    $this->assertEquals(15.0, $event_actions->eventBuyTicket->tariff);

    $this->assertInstanceOf('CultureFeed_Uitpas_Passholder_WelcomeAdvantageResultSet', $event_actions->welcomeAdvantages);
    $this->assertEquals(0, $event_actions->welcomeAdvantages->total);
    $this->assertContainsOnly('CultureFeed_Uitpas_Passholder_WelcomeAdvantage', $event_actions->welcomeAdvantages->objects);

    /* @var CultureFeed_Uitpas_Passholder_WelcomeAdvantage $welcomeAdvantage */
    /*
    $welcomeAdvantage = reset($event_actions->welcomeAdvantages->objects);

    $this->assertEquals('', $welcomeAdvantage->id);
    $this->assertEquals('', $welcomeAdvantage->title);
    $this->assertEquals(0, $welcomeAdvantage->points);
    $this->assertEquals(0, $welcomeAdvantage->cashedIn);
    */

    $this->assertInstanceOf('CultureFeed_Uitpas_Passholder_PointsPromotionResultSet', $event_actions->pointsPromotions);
    $this->assertEquals(3, $event_actions->pointsPromotions->total);
    $this->assertContainsOnly('CultureFeed_Uitpas_Passholder_PointsPromotion', $event_actions->pointsPromotions->objects);

    /* @var CultureFeed_Uitpas_Passholder_PointsPromotion $pointsPromotion */
    $pointsPromotion = reset($event_actions->pointsPromotions->objects);
    $this->assertEquals(15, $pointsPromotion->id);
    $this->assertEquals('test UITPAS-395', $pointsPromotion->title);
    $this->assertEquals(5, $pointsPromotion->points);
    $this->assertEquals(FALSE, $pointsPromotion->cashedIn);
    $this->assertEquals(CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::FILTER_POSSIBLE, $pointsPromotion->cashInState);

    $pointsPromotion = next($event_actions->pointsPromotions->objects);
    $this->assertEquals(13, $pointsPromotion->id);
    $this->assertEquals('Bon akaartweek', $pointsPromotion->title);
    $this->assertEquals(5, $pointsPromotion->points);
    $this->assertEquals(FALSE, $pointsPromotion->cashedIn);
    $this->assertEquals(CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::FILTER_POSSIBLE, $pointsPromotion->cashInState);

    $pointsPromotion = next($event_actions->pointsPromotions->objects);
    $this->assertEquals(3, $pointsPromotion->id);
    $this->assertEquals('Gratis drankje', $pointsPromotion->title);
    $this->assertEquals(1, $pointsPromotion->points);
    $this->assertEquals(FALSE, $pointsPromotion->cashedIn);
    $this->assertEquals(CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::FILTER_POSSIBLE, $pointsPromotion->cashInState);
  }

  public function testGetPartialEvent() {
    $event_actions = $this->loadEventActions('get.xml');
    $partial_event = $event_actions->getPartialEvent();

    $this->assertInstanceOf('CultureFeed_Uitpas_Event_CultureEvent', $partial_event);

    $this->assertEquals('27f1e25a-1ba6-4a43-96de-0c6f99b508f3', $partial_event->cdbid);

    //$this->assertInstanceOf('CultureFeed_Uitpas_Event_CheckinConstraint', $partial_event->checkinConstraint);

    $this->assertEquals(CultureFeed_Uitpas_Event_CultureEvent::CHECKIN_CONSTRAINT_REASON_MAXIMUM_REACHED, $partial_event->checkinConstraintReason);

    $this->assertEquals(1, $partial_event->numberOfPoints);

    $this->assertEquals(15, $partial_event->price);
    $this->assertEquals(15, $partial_event->tariff);

    $this->assertEquals(CultureFeed_Uitpas_Event_CultureEvent::BUY_CONSTRAINT_REASON_MAXIMUM_REACHED, $partial_event->buyConstraintReason);
  }

}
