<?php
/**
 *
 */

class CultureFeed_Uitpas_Passholder_EventActions {

  /**
   * @var CultureFeed_Uitpas_Passholder
   */
  public $passholder;

  /**
   * @var CultureFeed_Uitpas_Passholder_EventCheckin
   */
  public $eventCheckin;

  /**
   * @var CultureFeed_Uitpas_Passholder_EventBuyTicket
   */
  public $eventBuyTicket;

  /**
   * @var CultureFeed_Uitpas_Passholder_PointsPromotionResultSet
   */
  public $pointsPromotions;

  /**
   * @var CultureFeed_Uitpas_Passholder_WelcomeAdvantageResultSet
   */
  public $welcomeAdvantages;

  /**
   * @var bool
   */
  public $checkinAction;

  public static function createFromXML(CultureFeed_SimpleXMLElement $xml) {
    $eventActions = new self();

    $eventActions->passholder = CultureFeed_Uitpas_Passholder::createFromXML($xml->xpath('passHolder', FALSE));
    $eventActions->passholder->uitpasNumber = $xml->xpath_str('passHolder/uitpasNumber', FALSE);
    $eventActions->passholder->currentCard->uitpasNumber = $eventActions->passholder->uitpasNumber;
    $eventActions->passholder->currentCard->kansenpas = $eventActions->passholder->kansenStatuut;

    $eventActions->welcomeAdvantages = CultureFeed_Uitpas_Passholder_WelcomeAdvantageResultSet::createFromXML($xml->xpath('welcomeAdvantages', FALSE), 'welcomeAdvantage');
    $eventActions->pointsPromotions = CultureFeed_Uitpas_Passholder_PointsPromotionResultSet::createFromXML($xml->xpath('pointsPromotions', FALSE), 'pointsPromotion');

    $eventCheckin = $xml->xpath('eventCheckin', FALSE);
    $eventActions->eventCheckin = CultureFeed_Uitpas_Passholder_EventCheckin::createFromXML($eventCheckin);
    $eventBuyTicket = $xml->xpath('eventBuyTicket', FALSE);
    if ($eventBuyTicket instanceof CultureFeed_SimpleXMLElement) {
      $eventActions->eventBuyTicket = CultureFeed_Uitpas_Passholder_EventBuyTicket::createFromXML($eventBuyTicket);
    }

    return $eventActions;
  }

  /**
   * Constructs a partially filled CultureFeed_Uitpas_Event_CultureEvent object.
   *
   * @return CultureFeed_Uitpas_Event_CultureEvent
   */
  public function getPartialEvent() {
    $event = new CultureFeed_Uitpas_Event_CultureEvent();

    $event->cdbid = $this->eventBuyTicket->cdbid;

    if ($this->eventBuyTicket) {
      $event->buyConstraintReason = $this->eventBuyTicket->buyConstraintReason;
      $event->price = $this->eventBuyTicket->price;
      $event->tariff = $this->eventBuyTicket->tariff;
    }

    $event->checkinAllowed = $this->eventCheckin->checkinAllowed;
    $event->checkinConstraintReason = $this->eventCheckin->checkinConstraintReason;
    $event->numberOfPoints = $this->eventCheckin->numberOfPoints;

    return $event;
  }
}
