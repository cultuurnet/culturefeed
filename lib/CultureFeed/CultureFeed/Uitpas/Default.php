<?php

/**
 *
 */
class CultureFeed_Uitpas_Default implements CultureFeed_Uitpas {

  /**
   *
   * CultureFeed object to make CultureFeed core requests.
   * @var ICultureFeed
   */
  protected $culturefeed;

  /**
   * OAuth request object to do the request.
   *
   * @var CultureFeed_OAuthClient
   */
  protected $oauth_client;

  /**
   *
   * Constructor for a new UitPas_Default instance
   * @param ICultureFeed $culturefeed
   */
  public function __construct(ICultureFeed $culturefeed) {
    $this->culturefeed = $culturefeed;
    $this->oauth_client = $culturefeed->getClient();
  }

  /**
   * Get the distribution keys for an organizer.
   *
   * @param string $cdbid The CDBID of the organizer
   */
  public function getDistributionKeysForOrganizer($cdbid) {
    $result = $this->oauth_client->consumerGetAsXML('uitpas/distributionkey/organiser/' . $cdbid, array());

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $distribution_keys = array();
    $objects = $xml->xpath('/response/distributionkeys/distributionKey');
    $total = count($objects);

    foreach ($objects as $object) {
      $distribution_keys[] = CultureFeed_Uitpas_DistributionKey::create($object);
    }

    return new CultureFeed_ResultSet($total, $distribution_keys);
  }

  /**
   * Register a set of distribution keys for an organizer. The entire set (including existing)
   * of distribution keys must be provided.
   *
   * @param string $cdbid The CDBID of the organizer
   * @param array $distribution_keys The identification of the distribution key
   */
  public function registerDistributionKeysForOrganizer($cdbid, $distribution_keys) {
    $this->oauth_client->consumerPostAsXml('uitpas/distributionkey/organiser/' . $cdbid, $distribution_keys);
  }

  /**
   * Get the price of the UitPas.
   */
  public function getPrice() {
    $price = $this->oauth_client->consumerGetAsXML('uitpas/passholder/uitpasPrice', array());

    return $price;
  }

/* (non-PHPdoc)
 * @see CultureFeed_Uitpas::createPassholder()
 */
  public function createPassholder(CultureFeed_Uitpas_Passholder $passholder) {
    // TODO Auto-generated method stub

  }

/* (non-PHPdoc)
 * @see CultureFeed_Uitpas::createMembershipForPassholder()
 */
  public function createMembershipForPassholder($id, $organization, $end_date) {
    // TODO Auto-generated method stub

  }

/* (non-PHPdoc)
 * @see CultureFeed_Uitpas::getPassholder()
 */
  public function getPassholder($uitpas_number) {
    // TODO Auto-generated method stub

  }

/* (non-PHPdoc)
 * @see CultureFeed_Uitpas::searchPassholders()
 */
  public function searchPassholders(CultureFeed_Uitpas_SearchPassHoldersQuery $query) {
    // TODO Auto-generated method stub

  }

/* (non-PHPdoc)
 * @see CultureFeed_Uitpas::getWelcomeAdvantagesForPassholder()
 */
  public function getWelcomeAdvantagesForPassholder($uitpas_number) {
    // TODO Auto-generated method stub

  }

/* (non-PHPdoc)
 * @see CultureFeed_Uitpas::checkinPassholder()
 */
  public function checkinPassholder($cdbid, $uitpas_number, $chip_number) {
    // TODO Auto-generated method stub

  }

/* (non-PHPdoc)
 * @see CultureFeed_Uitpas::cashInWelcomeAdvantage()
 */
  public function cashInWelcomeAdvantage($uitpas_number, $welcome_advantage_id) {
    // TODO Auto-generated method stub

  }

/* (non-PHPdoc)
 * @see CultureFeed_Uitpas::getPointsPromotions()
 */
  public function getPointsPromotions(CultureFeed_Uitpas_SearchPointsPromotionsOptionsQuery $query) {
    // TODO Auto-generated method stub

  }

/* (non-PHPdoc)
 * @see CultureFeed_Uitpas::cashInPointsPromotion()
 */
  public function cashInPointsPromotion($uitpas_number, $points_promotion_id, $bar) {
    // TODO Auto-generated method stub

  }

/* (non-PHPdoc)
 * @see CultureFeed_Uitpas::uploadPicture()
 */
  public function uploadPicture($id, $file_data) {
    // TODO Auto-generated method stub

  }

/* (non-PHPdoc)
 * @see CultureFeed_Uitpas::updatePassholder()
 */
  public function updatePassholder(CultureFeed_Uitpas_Passholder $passholder) {
    // TODO Auto-generated method stub

  }

/* (non-PHPdoc)
 * @see CultureFeed_Uitpas::blockUitpas()
 */
  public function blockUitpas($uitpas_number) {
    // TODO Auto-generated method stub

  }

/* (non-PHPdoc)
 * @see CultureFeed_Uitpas::searchWelcomeAdvantages()
 */
  public function searchWelcomeAdvantages(CultureFeed_Uitpas_SearchWelcomeAdvantagesQuery $query) {
    // TODO Auto-generated method stub

  }

/* (non-PHPdoc)
 * @see CultureFeed_Uitpas::getPassholderForChipNumber()
 */
  public function getPassholderForChipNumber($chip_number) {
    // TODO Auto-generated method stub

  }

/* (non-PHPdoc)
 * @see CultureFeed_Uitpas::getEventsForPassholder()
 */
  public function getEventsForPassholder($uitpas_number, $date_from, $date_to) {
    // TODO Auto-generated method stub

  }

/* (non-PHPdoc)
 * @see CultureFeed_Uitpas::registerTicketSale()
 */
  public function registerTicketSale($uitpas_number, $cdbid) {
    // TODO Auto-generated method stub

  }

/* (non-PHPdoc)
 * @see CultureFeed_Uitpas::cancelTicketSale()
 */
  public function cancelTicketSale($uitpas_number, $cdbid) {
    // TODO Auto-generated method stub

  }

/* (non-PHPdoc)
 * @see CultureFeed_Uitpas::getAccumulatedPoints()
 */
  public function getAccumulatedPoints(CultureFeed_Uitpas_AccumulatedPointsQuery $query) {
    // TODO Auto-generated method stub

  }

/* (non-PHPdoc)
 * @see CultureFeed_Uitpas::searchEvents()
 */
  public function searchEvents(CultureFeed_Uitpas_SearchEventsQuery $query) {
    // TODO Auto-generated method stub

  }

/* (non-PHPdoc)
 * @see CultureFeed_Uitpas::searchPointOfSales()
 */
  public function searchPointOfSales(CultureFeed_Uitpas_SearchPointOfSalesQuery $query) {
    // TODO Auto-generated method stub

  }

/* (non-PHPdoc)
 * @see CultureFeed_Uitpas::addMemberToBar()
 */
  public function addMemberToBar($id, $uid) {
    // TODO Auto-generated method stub

  }

/* (non-PHPdoc)
 * @see CultureFeed_Uitpas::searchBarsForMember()
 */
  public function searchBarsForMember($uid) {
    // TODO Auto-generated method stub

  }


}