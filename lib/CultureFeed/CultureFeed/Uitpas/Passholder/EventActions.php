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
   * @var
   */
  public $eventCheckin;

  /**
   * @var
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

    $eventActions->passholder = CultureFeed_Uitpas_Passholder::createFromXML($xml->xpath('//passHolder', FALSE));
    $eventActions->welcomeAdvantages = CultureFeed_Uitpas_Passholder_WelcomeAdvantageResultSet::createFromXML($xml->xpath('//welcomeAdvantages', FALSE), 'welcomeAdvantage');
    $eventActions->pointsPromotions = CultureFeed_Uitpas_Passholder_PointsPromotionResultSet::createFromXML($xml->xpath('//pointsPromotions', FALSE), 'pointsPromotion');

    //$eventActions->eventCheckin = ::createFromXML($xml->xpath('eventCheckin'));
    //$eventActions->eventBuyTicket = ::createFromXML($xml->xpath('eventBuyTicket'));

    return $eventActions;
  }
}
