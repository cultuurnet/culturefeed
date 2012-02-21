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
   * Get the associations.
   *
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function getAssociations($consumer_key_counter) {
    $data = array(
      'balieConsumerKey' => $consumer_key_counter,
    );

    $result = $this->oauth_client->consumerGetAsXML('uitpas/association/list', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $associations = array();
    $objects = $xml->xpath('/response/associations/assocation');
    $total = count($objects);

    foreach ($objects as $object) {
      $associations[] = CultureFeed_Uitpas_Association::createFromXML($object);
    }

    return new CultureFeed_ResultSet($total, $associations);
  }

  /**
   * Get the distribution keys for a given organizer.
   *
   * @param string $cdbid The CDBID of the given organizer
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
    $objects = $xml->xpath('/response/distributionkeys/distributionkey');
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
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function getPassholderByUitpasNumber($uitpas_number, $consumer_key_counter) {
    $data = array(
      'balieConsumerKey' => $consumer_key_counter,
    );

    $result = $this->oauth_client->authenticatedGetAsXml('uitpas/passholder/' . $uitpas_number, $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $object = $xml->xpath('/passHolder');

    return CultureFeed_Uitpas_Passholder::createFromXml($object);
  }

  /**
   * Get a passholder based on the user ID
   *
   * @param string $user_id The user ID
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function getPassholderByUser($user_id, $consumer_key_counter) {
    $data = array(
      'balieConsumerKey' => $consumer_key_counter,
    );

    $result = $this->oauth_client->authenticatedGetAsXml('uitpas/passholder/uid/' . $user_id, $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $object = $xml->xpath('/passHolder');

    return CultureFeed_Uitpas_Passholder::createFromXml($object);
  }

  /**
   * Search for passholders.
   *
   * @param CultureFeed_Uitpas_Passholder_Query_SearchPassholdersOptions $query The query
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function searchPassholders(CultureFeed_Uitpas_Passholder_Query_SearchPassholdersOptions $query, $consumer_key_counter) {
    $data = $query->toPostData();
    $data['balieConsumerKey'] = $consumer_key_counter;

    $result = $this->oauth_client->consumerPostAsXml('uiitpas/passholder/search', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $passholders = array();
    $objects = $xml->xpath('/passholders/passholder');
    $total = $xml->xpath_int('/passholders/total');

    foreach ($objects as $object) {
      $passholders[] = CultureFeed_Uitpas_Passholder::createFromXML($object);
    }

    return new CultureFeed_ResultSet($total, $passholders);
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
   * @param CultureFeed_Uitpas_Passholder_Query_CheckInPassholderOptions $query The event data object
   * @return The total amount of points of the user
   */
  public function checkinPassholder(CultureFeed_Uitpas_Passholder_Query_CheckInPassholderOptions $event) {
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
      $promotions[] = CultureFeed_Uitpas_Passholder_PointsPromotion::createFromXML($object);
    }

    return new CultureFeed_ResultSet($total, $promotions);
  }

  /**
   * Cash in promotion points for a UitPas.
   *
   * @param string $uitpas_number The UitPas number
   * @param int $points_promotion_id The identification of the redeem option
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function cashInPromotionPoints($uitpas_number, $consumer_key_counter, $points_promotion_id) {
    $data = array(
      'pointsPromotionId' => $points_promotion_id,
      'balieConsumerKey' => $consumer_key_counter,
    );

    $result = $this->oauth_client->authenticatedPostAsXml('uitpas/passholder/' . $uitpas_number . '/cashInPointsPromotion', $data);

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
   * Upload a picture for a given passholder.
   *
   * @param string $id The user ID of the passholder
   * @param string $file_data The binary data of the picture
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function uploadPicture($id, $file_data, $consumer_key_counter) {
    $data = array(
      'picture' => $file_data,
      'balieConsumerKey' => $consumer_key_counter,
    );

    $this->oauth_client->authenticatedPostAsXml('uitpas/passholder/' . $id . '/uploadPicture', TRUE, TRUE);
  }

  /**
   * Update a passholder.
   *
   * @param CultureFeed_Uitpas_Passholder $passholder The passholder to update.
   * 		The passholder is identified by ID. Only fields that are set will be updated.
   */
  public function updatePassholder(CultureFeed_Uitpas_Passholder $passholder) {
    $data = $passholder->toPostData();
    $result = $this->oauth_client->authenticatedPostAsXml('uitpas/passholder/' . $passholder->uitpasNumber, $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $response = CultureFeed_Uitpas_Response::createFromXML($xml->xpath('/response', false));
    return $response;
  }

  /**
   * Block a UitPas.
   *
   * @param string $uitpas_number The UitPas number
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function blockUitpas($uitpas_number, $consumer_key_counter) {
    $data = array(
      'balieConsumerKey' => $consumer_key_counter,
    );

    $result = $this->oauth_client->authenticatedPostAsXml('uitpas/passholder/block/' . $uitpas_number, $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $response = CultureFeed_Uitpas_Response::createFromXML($xml->xpath('/uitpasRestResponse', false));
    return $response;
  }

  /**
   * Search for welcome advantages.
   *
   * @param CultureFeed_Uitpas_Promotion_Query_WelcomeAdvantagesOptions $query The query
   */
  public function searchWelcomeAdvantages(CultureFeed_Uitpas_Promotion_Query_WelcomeAdvantagesOptions $query) {
    $data = $query->toPostData();
    $result = $this->oauth_client->authenticatedGetAsXml('uitpas/promotion/welcomeAdvantages', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $promotions = array();
    $objects = $xml->xpath('/welcomeAdvantagesRestResponse/promotions/promotion');
    $total = $xml->xpath_int('/welcomeAdvantagesRestResponse/total');

    foreach ($objects as $object) {
      $promotions[] = CultureFeed_Uitpas_Passholder_WelcomeAdvantage::createFromXML($object);
    }

    return new CultureFeed_ResultSet($total, $promotions);
  }

  /**
   * Get a passholder based on the UitPas chip number.
   *
   * @param string $chip_number The chipnumber of the UitPas
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function getPassholderForChipNumber($chip_number, $consumer_key_counter) {
    $data = array(
      'chipNummer' => $chip_number,
      'balieServiceConumer' => $consumer_key_counter,
    );

    $result = $this->oauth_client->authenticatedGetAsXml('uitpas/passholder/uitpasNumber', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    return $xml->xpath_str('/uitpasRestResponse/message');
  }

  /**
   * Register a ticket sale for a passholder
   *
   * @param string $uitpas_number The UitPas number
   * @param string $cdbid The event CDBID
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function registerTicketSale($uitpas_number, $cdbid, $consumer_key_counter) {
    $data = array(
      'balieConsumerKey' => $consumer_key_counter,
    );

    $result = $this->oauth_client->authenticatedPostAsXml('uitpas/cultureevent/' . $cdbid . '/buy/' . $uitpas_number, $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $ticket_sale = CultureFeed_Uitpas_Event_TicketSale::createFromXML($xml->xpath('/ticketSale', false));
    return $ticket_sale;
  }

  /**
   * Cancel a ticket sale for a passholder
   *
   * @param string $uitpas_number The UitPas number
   * @param string $cdbid The event CDBID
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function cancelTicketSale($uitpas_number, $cdbid, $consumer_key_counter) {
    $data = array(
      'balieConsumerKey' => $consumer_key_counter,
    );

    $this->oauth_client->authenticatedPostAsXml('uitpas/cultureevent/' . $cdbid . '/cancel/' . $uitpas_number, $data);
  }

  /**
   * Search for checkins
   *
   * @param CultureFeed_Uitpas_Passholder_Query_SearchCheckinsOptions $query The query
   */
  public function searchCheckins(CultureFeed_Uitpas_Passholder_Query_SearchCheckinsOptions $query) {
    $data = $query->toPostData();
    $result = $this->oauth_client->authenticatedGetAsXml('uitpas/cultureevent/searchCheckins', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $checkins = array();
    $objects = $xml->xpath('/response/checkinActivities/checkinActivitiy');
    $total = count($objects);

    foreach ($objects as $object) {
      $checkins[] = CultureFeed_Uitpas_Event_CheckinActivity::createFromXML($object);
    }

    return new CultureFeed_ResultSet($total, $checkins);
  }

  /**
   * Search for Uitpas events
   *
   * @param CultureFeed_Uitpas_SearchEventsQuery $query The query
   */
  public function searchEvents(CultureFeed_Uitpas_Passholder_Query_SearchEventsOptions $query) {
    $data = $query->toPostData();
    $result = $this->oauth_client->consumerGetAsXml('uitpas/cultureevent/search', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $events = array();
    $objects = $xml->xpath('/cultureEvents/event');
    $total = $xml->xpath_int('/cultureEvents/total');

    foreach ($objects as $object) {
      $events[] = CultureFeed_Uitpas_Event_CultureEvent::createFromXML($object);
    }

    return new CultureFeed_ResultSet($total, $events);
  }

  /**
   * Search for point of sales
   *
   * @param CultureFeed_Uitpas_Counter_Query_SearchPointsOfSaleOptions $query The query
   */
  public function searchPointOfSales(CultureFeed_Uitpas_Counter_Query_SearchPointsOfSaleOptions $query) {
    $data = $query->toPostData();
    $result = $this->oauth_client->consumerGetAsXml('uitpas/balie/pos', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $pos = array();
    $objects = $xml->xpath('/balieRestResponse/balie');
    $total = $xml->xpath_int('/balieRestResponse/total');

    foreach ($objects as $object) {
      $pos[] = CultureFeed_Uitpas_Counter::createFromXML($object);
    }

    return new CultureFeed_ResultSet($total, $pos);
  }

  /**
   * Add a member to a counter.
   *
   * @param string $uid The Culturefeed user ID
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function addMemberToCounter($uid, $consumer_key_counter) {
    $data = array(
      'balieConsumerKey' => $consumer_key_counter,
      'uid' => $uid,
    );

    $this->oauth_client->authenticatedPost('uitpas/balie/member', $data);
  }

  /**
   * Search for counters for a given member
   *
   * @param string $uid The Culturefeed user ID
   */
  public function searchCountersForMember($uid) {
    $data = array(
      'uid' => $uid,
    );

    $result = $this->oauth_client->authenticatedGetAsXml('uitpas/balie/pos', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $counters = array();
    $objects = $xml->xpath('/employeeBaliesRestResponse/balie');
    $total = count($objects);

    foreach ($objects as $object) {
      $counters[] = CultureFeed_Uitpas_Counter_Employee::createFromXML($object);
    }

    return new CultureFeed_ResultSet($total, $counters);
  }


}