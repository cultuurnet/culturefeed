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
      $distribution_keys[] = CultureFeed_Uitpas_DistributionKey::createFromXML($object);
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
    $price = $this->oauth_client->consumerGet('uitpas/passholder/uitpasPrice', array());

    return $price;
  }

  /**
   * Create a new UitPas passholder.
   *
   * @param CultureFeed_Uitpas_Passholder $passholder The new passholder
   * @return Passholder user ID
   */
  public function createPassholder(CultureFeed_Uitpas_Passholder $passholder) {
    $data = $passholder->toPostData();
    $culturefeed_uid = $this->oauth_client->consumerPost('uitpas/passholder/register', $data);

    return $culturefeed_uid;
  }

  /**
   * Create a new membership for a UitPas passholder.
   *
   * @param CultureFeed_Uitpas_Membership $membership The membership object of the UitPas passholder
   */
  public function createMembershipForPassholder(CultureFeed_Uitpas_Passholder_Membership $membership) {
    $data = $membership->toPostData();
    $this->oauth_client->consumerPostAsXml('uitpas/passholder/createMembership', $data);
  }

  /**
   * Get a passholder based on the UitPas number.
   *
   * @param string $uitpas_number The UitPas number
   */
  public function getPassholder($uitpas_number) {
    $this->oauth_client->consumerGetAsXml('uitpas/passholder/' . $uitpas_number, array());
  }

  /**
   * Search for passholders.
   *
   * @param CultureFeed_Uitpas_SearchPassHoldersQuery $query The query
   */
  public function searchPassholders(CultureFeed_Uitpas_SearchPassHoldersQuery $query) {
    // TODO Auto-generated method stub

  }

  /**
   * Get the welcome advantages for a passholder.
   *
   * @param string $uitpas_number The UitPas number
   */
  public function getWelcomeAdvantagesForPassholder($uitpas_number) {
    $result = $this->oauth_client->consumerGetAsXml('uitpas/passholder/' . $uitpas_number . '/welcomeadvantages', array());

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $advantages = array();
    $objects = $xml->xpath('/promotions/promotion');
    $total = count($objects);

    foreach ($objects as $object) {
      $advantages[] = CultureFeed_Uitpas_Passholder_WelcomeAdvantage::createFromXML($object);
    }

    return new CultureFeed_ResultSet($total, $advantages);
  }

  /**
   * Check in a passholder.
   *
   * Provide either a UitPas number or chip number. You cannot provide both.
   *
   * @param CultureFeed_Uitpas_Passholder_Event $event The event data object
   * @return The total amount of points of the user
   */
  public function checkinPassholder(CultureFeed_Uitpas_Passholder_Event $event) {
    $data = $event->toPostData();
    $result = $this->oauth_client->consumerPostAsXml('uitpas/passholder/checkin', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $points = $xml->xpath_int('/response/points');
    return $points;
  }

  /**
   * Cash in a welcome advantage.
   *
   * @param string $uitpas_number The UitPas number
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   * @param int $welcome_advantage_id Identification welcome advantage
   */
  public function cashInWelcomeAdvantage($uitpas_number, $consumer_key_counter, $welcome_advantage_id) {
     $data = array(
       'welcomeAdvantageId' => $welcome_advantage_id,
       'balieConsumerKey' => $consumer_key_counter,
     );

     $result = $this->oauth_client->consumerPostAsXml('uitpas/passholder/' . $uitpas_number . '/cashInWelcomeAdvantage', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $promotion = CultureFeed_Uitpas_Passholder_WelcomeAdvantage::createFromXML($xml->xpath('/promotion', false));
    return $promotion;
  }

  /**
   * Get the redeem options
   *
   * @param CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions $query The query
   */
  public function getPromotionPoints(CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions $query) {
    $data = $query->toPostData();
    $result = $this->oauth_client->consumerGetAsXml('uitpas/passholder/pointsPromotions', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $promotions = array();
    $objects = $xml->xpath('/pointsPromotionsRestResponse/promotions/promotion');
    $total = $xml->xpath_int('/pointsPromotionsRestResponse/total');

    foreach ($objects as $object) {
      $promotions[] = CultureFeed_Uitpas_Passholder_Promotion::createFromXML($object);
    }

    return new CultureFeed_ResultSet($total, $promotions);
  }

  /**
   * Cash in promotion points for a UitPas.
   *
   * @param string $uitpas_number The UitPas number
   * @param int $points_promotion_id The identification of the redeem option
   * @param string $counter The name of the UitPas counter
   */
  public function cashInPromotionPoints($uitpas_number, $points_promotion_id, $counter) {
    // TODO Auto-generated method stub

  }

  /**
   * Upload a picture for a given passholder.
   *
   * @param string $id The user ID of the passholder
   * @param string $file_data The binary data of the picture
   */
  public function uploadPicture($id, $file_data) {
    // TODO Auto-generated method stub

  }

  /**
   * Update a passholder.
   *
   * @param CultureFeed_Uitpas_Passholder $passholder The passholder to update.
   * 		The passholder is identified by ID. Only fields that are set will be updated.
   */
  public function updatePassholder(CultureFeed_Uitpas_Passholder $passholder) {
    // TODO Auto-generated method stub

  }

  /**
   * Block a UitPas.
   *
   * @param string $uitpas_number The UitPas number
   */
  public function blockUitpas($uitpas_number) {
    // TODO Auto-generated method stub

  }

  /**
   * Search for welcome advantages.
   *
   * @param CultureFeed_Uitpas_SearchWelcomeAdvantagesQuery $query The query
   */
  public function searchWelcomeAdvantages(CultureFeed_Uitpas_SearchWelcomeAdvantagesQuery $query) {
    // TODO Auto-generated method stub

  }

  /**
   * Get a passholder based on the UitPas chip number.
   *
   * @param string $chip_number The chipnumber of the UitPas
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
 * @see CultureFeed_Uitpas::addMemberToCounter()
 */
  public function addMemberToCounter($id, $uid) {
    // TODO Auto-generated method stub

  }

/* (non-PHPdoc)
 * @see CultureFeed_Uitpas::searchCountersForMember()
 */
  public function searchCountersForMember($uid) {
    // TODO Auto-generated method stub

  }


}