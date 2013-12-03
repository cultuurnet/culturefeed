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
   * {@inheritdoc}
   */
  public function getAssociations($consumer_key_counter = NULL) {
    $data = array();

    if ($consumer_key_counter) {
      $data['balieConsumerKey'] = $consumer_key_counter;
    }

    $result = $this->oauth_client->authenticatedGetAsXML('uitpas/association/list', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $associations = array();
    $objects = $xml->xpath('/response/associations/association');
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
   * @return CultureFeed_ResultSet The set of distribution keys
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
  public function getPrice($consumer_key_counter = NULL) {
    $data = array();

    if ($consumer_key_counter) {
      $data['balieConsumerKey'] = $consumer_key_counter;
    }

    $result = $this->oauth_client->authenticatedGetAsXml('uitpas/passholder/uitpasPrice', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $prices = array();

    foreach ($xml->xpath('uitpasPrices/uitpasPrice') as $price_xml) {
      $prices[] = CultureFeed_Uitpas_Passholder_UitpasPrice::createFromXML($price_xml);
    }

    $total = count($prices);

    return new CultureFeed_ResultSet($total, $prices);
  }

  /**
   * Create a new UitPas passholder.
   *
   * @param CultureFeed_Uitpas_Passholder $passholder The new passholder
   * @return Passholder user ID
   */
  public function createPassholder(CultureFeed_Uitpas_Passholder $passholder) {
    $data = $passholder->toPostData();
    $result = $this->oauth_client->authenticatedPostAsXml('uitpas/passholder/register', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    return $xml->xpath_str('/response/message');
  }

  /**
   * Create a new membership for a UitPas passholder.
   *
   * @param CultureFeed_Uitpas_Membership $membership The membership object of the UitPas passholder
   */
  public function createMembershipForPassholder(CultureFeed_Uitpas_Passholder_Membership $membership) {
    $data = $membership->toPostData();
    $result = $this->oauth_client->authenticatedPostAsXml('uitpas/passholder/createMembership', $data);

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
   * Register an event.
   *
   * @param CultureFeed_Uitpas_Event_CultureEvent $event The event data that needs to be sent over.
   * @return CultureFeed_Uitpas_Response
   */
   public function registerEvent(CultureFeed_Uitpas_Event_CultureEvent $event) {

     $path = "/uitpas/cultureevent/register";

     $data = $event->toPostData();

     //dpm($data, 'Data sent to uitpas call');




     try {
       // Call the path.
       $result = $this->oauth_client->consumerPostAsXml( $path, $data );
     } catch(CultureFeed_Exception $e) {
       throw $e;
     }


     try {
       $xml = new CultureFeed_SimpleXMLElement($result);
     }
     catch (Exception $e) {
       throw new CultureFeed_ParseException($result);
     }

     //dpm( print_r( $xml, true ) , 'xml return from register call' );

     $response = CultureFeed_Uitpas_Response::createFromXML($xml->xpath('/response', false));

     //dpm( $response, 'Response from register uitpas call' );

     return $response;


   }

  /**
   * Resend the activation e-mail for a passholder
   *
   * @param string $uitpas_number The UitPas number
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function resendActivationEmail($uitpas_number, $consumer_key_counter = NULL) {
    $data = array();

    if ($consumer_key_counter) {
      $data['balieConsumerKey'] = $consumer_key_counter;
    }

    $result = $this->oauth_client->authenticatedPostAsXml('uitpas/passholder/' . $uitpas_number . '/resend_activation_mail', $data);

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
   * Get a passholder based on the UitPas number.
   *
   * @param string $uitpas_number The UitPas number
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function getPassholderByUitpasNumber($uitpas_number, $consumer_key_counter = NULL) {
    $data = array();

    if ($consumer_key_counter) {
      $data['balieConsumerKey'] = $consumer_key_counter;
    }

    $result = $this->oauth_client->authenticatedGetAsXml('uitpas/passholder/' . $uitpas_number, $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $object = $xml->xpath('/passHolder', false);

    return CultureFeed_Uitpas_Passholder::createFromXml($object);
  }

  /**
   * Get a passholder based on the user ID
   *
   * @param string $user_id The user ID
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function getPassholderByUser($user_id, $consumer_key_counter = NULL) {
    $data = array();

    if ($consumer_key_counter) {
      $data['balieConsumerKey'] = $consumer_key_counter;
    }

    $result = $this->oauth_client->authenticatedGetAsXml('uitpas/passholder/uid/' . $user_id, $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    return CultureFeed_Uitpas_Passholder::createFromXml($xml);
  }

  /**
   * Search for passholders.
   *
   * @param CultureFeed_Uitpas_Passholder_Query_SearchPassholdersOptions $query The query
   * @param string $method The request method
   */
  public function searchPassholders(CultureFeed_Uitpas_Passholder_Query_SearchPassholdersOptions $query, $method = CultureFeed_Uitpas::CONSUMER_REQUEST) {
    $data = $query->toPostData();

    if ($method == CultureFeed_Uitpas::CONSUMER_REQUEST) {
      $result = $this->oauth_client->consumerGetAsXml('uitpas/passholder/search', $data);
    }
    else {
      $result = $this->oauth_client->authenticatedGetAsXml('uitpas/passholder/search', $data);
    }

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $passholders = array();
    $objects = $xml->xpath('/response/passholders/passholder');
    $total = $xml->xpath_int('/response/total');

    foreach ($objects as $object) {
      $passholders[] = CultureFeed_Uitpas_Passholder::createFromXML($object);
    }

    return new CultureFeed_ResultSet($total, $passholders);
  }

  /**
   * Get the welcome advantages for a passholder.
   *
   * @param CultureFeed_Uitpas_Passholder_Query_WelcomeAdvantagesOptions $query The query
   */
  public function getWelcomeAdvantagesForPassholder(CultureFeed_Uitpas_Passholder_Query_WelcomeAdvantagesOptions $query) {
    $data = $query->toPostData();
    $result = $this->oauth_client->authenticatedGetAsXml('uitpas/passholder/' . $query->uitpas_number . '/welcomeadvantages', $data);

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
  public function checkinPassholder(CultureFeed_Uitpas_Passholder_Query_CheckInPassholderOptions $query) {
    $data = $query->toPostData();
    $result = $this->oauth_client->authenticatedPostAsXml('uitpas/passholder/checkin', $data);

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
   * @param int $welcome_advantage_id Identification welcome advantage
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function cashInWelcomeAdvantage($uitpas_number, $welcome_advantage_id, $consumer_key_counter = NULL) {
     $data = array(
       'welcomeAdvantageId' => $welcome_advantage_id,
     );

     if ($consumer_key_counter) {
       $data['balieConsumerKey'] = $consumer_key_counter;
     }

     $result = $this->oauth_client->authenticatedPostAsXml('uitpas/passholder/' . $uitpas_number . '/cashInWelcomeAdvantage', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $promotion = CultureFeed_Uitpas_Passholder_WelcomeAdvantage::createFromXML($xml->xpath('/promotionTO', false));
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
    $objects = $xml->xpath('/response/promotions/promotion');
    $total = $xml->xpath_int('/response/total');

    foreach ($objects as $object) {
      $promotions[] = CultureFeed_Uitpas_Passholder_PointsPromotion::createFromXML($object);
    }

    return new CultureFeed_ResultSet($total, $promotions);
  }

  public function getCashedInPromotionPoints(CultureFeed_Uitpas_Passholder_Query_SearchCashedInPromotionPointsOptions $query) {
    $data = $query->toPostData();
    $result = $this->oauth_client->authenticatedGetAsXml('uitpas/passholder/' . $query->uitpasNumber . '/cashedPointsPromotions', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $promotions = array();
    $objects = $xml->xpath('/response/cashedPromotions/cachedPromotion');
    $total = $xml->xpath_int('/response/total');

    foreach ($objects as $object) {
      $promotions[] = CultureFeed_Uitpas_Passholder_CashedInPointsPromotion::createFromXML($object);
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
  public function cashInPromotionPoints($uitpas_number, $points_promotion_id, $consumer_key_counter = NULL) {
    $data = array(
      'pointsPromotionId' => $points_promotion_id,
    );

    if ($consumer_key_counter) {
      $data['balieConsumerKey'] = $consumer_key_counter;
    }

    $result = $this->oauth_client->authenticatedPostAsXml('uitpas/passholder/' . $uitpas_number . '/cashInPointsPromotion', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $promotion = CultureFeed_Uitpas_Passholder_PointsPromotion::createFromXML($xml->xpath('/promotionTO', false));
    return $promotion;
  }

  /**
   * Upload a picture for a given passholder.
   *
   * @param string $id The user ID of the passholder
   * @param string $file_data The binary data of the picture
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function uploadPicture($id, $file_data, $consumer_key_counter = NULL) {
    $data = array(
      'picture' => $file_data,
    );

    if ($consumer_key_counter) {
      $data['balieConsumerKey'] = $consumer_key_counter;
    }

    $this->oauth_client->authenticatedPostAsXml('uitpas/passholder/' . $id . '/uploadPicture', $data, TRUE, TRUE);
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
   * {@inheritdoc}
   */
  public function updatePassholderCardSystemPreferences(CultureFeed_Uitpas_Passholder_CardSystemPreferences $preferences) {

    $data = $preferences->toPostData();
    $result = $this->oauth_client->authenticatedPostAsXml('uitpas/passholder/' . $preferences->id . '/' . $preferences->cardSystemId, $data);

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
  public function blockUitpas($uitpas_number, $consumer_key_counter = NULL) {
    $data = array();

    if ($consumer_key_counter) {
      $data['balieConsumerKey'] = $consumer_key_counter;
    }

    $result = $this->oauth_client->authenticatedPostAsXml('uitpas/passholder/block/' . $uitpas_number, $data);

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
   * Search for welcome advantages.
   *
   * @param CultureFeed_Uitpas_Promotion_Query_WelcomeAdvantagesOptions $query The query
   * @param string $method The request method
   */
  public function searchWelcomeAdvantages(CultureFeed_Uitpas_Promotion_Query_WelcomeAdvantagesOptions $query, $method = CultureFeed_Uitpas::CONSUMER_REQUEST) {
    $path = 'uitpas/promotion/welcomeAdvantages';

    $data = $query->toPostData();

    if ($method == CultureFeed_Uitpas::CONSUMER_REQUEST) {
      $result = $this->oauth_client->consumerGetAsXml($path, $data);
    }
    else {
      $result = $this->oauth_client->authenticatedGetAsXml($path, $data);
    }

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $promotions = array();
    $objects = $xml->xpath('/response/promotions/promotion');
    $total = $xml->xpath_int('/response/total');

    foreach ($objects as $object) {
      $promotions[] = CultureFeed_Uitpas_Passholder_WelcomeAdvantage::createFromXML($object);
    }

    return new CultureFeed_ResultSet($total, $promotions);
  }

  /**
   * {@inheritdoc}
   */
  public function getCard(CultureFeed_Uitpas_CardInfoQuery $card_query) {
    $data = $card_query->toPostData();

    $result = $this->oauth_client->authenticatedGetAsXml('uitpas/card', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $card = CultureFeed_Uitpas_CardInfo::createFromXML($xml->xpath('/response', FALSE));
    return $card;
  }

  /**
   * {@inheritdoc}
   */
  public function getPassholderActivationLink(CultureFeed_Uitpas_Passholder_Query_ActivationData $activation_data, $destination_callback = NULL) {
    $path = "uitpas/passholder/{$activation_data->uitpasNumber}/activation";

    $params = array(
      'dob' => $activation_data->dob->format('Y-m-d'),
    );

    $result = $this->oauth_client->consumerGetAsXml($path, $params);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $link = $xml->xpath_str('/response/activationLink');

    $query = array();

    if ($destination_callback) {
      $query['destination'] = call_user_func($destination_callback);
    }

    if (!empty($query)) {
      $link .= '?' . http_build_query($query);
    }

    return $link;
  }

  /**
   * (non-PHPdoc)
   * @see CultureFeed_Uitpas::constructPassHolderActivationLink()
   */
  public function constructPassHolderActivationLink($uid, $activation_code, $destination_callback = NULL) {
    $path = "uitpas/activate/{$uid}/{$activation_code}";

    $query = array();

    if ($destination_callback) {
      $query['destination'] = call_user_func($destination_callback);
    }

    $link = $this->oauth_client->getUrl($path, $query);

    return $link;
  }

  public function getPassholderActivationLinkChainedWithAuthorization($uitpas_number, DateTime $date_of_birth, $callback_url) {
    $c = $this->culturefeed;

    $link = $this->getPassholderActivationLink($uitpas_number, $date_of_birth, function () use ($c, $callback_url) {
      $token = $c->getRequestToken($callback_url);

      $auth_url = $c->getUrlAuthorize($token, $callback_url, CultureFeed::AUTHORIZE_TYPE_REGULAR, TRUE);

      return $auth_url;
    });

    return $link;
  }

  /**
   * Register a new Uitpas
   *
   * @param CultureFeed_Uitpas_Passholder_Query_RegisterUitpasOptions $query The query
   */
  public function registerUitpas(CultureFeed_Uitpas_Passholder_Query_RegisterUitpasOptions $query) {
    $data = $query->toPostData();
    $result = $this->oauth_client->authenticatedPostAsXml('uitpas/passholder/newCard', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    return $xml->xpath_str('/response/message');
  }

  /**
   * Register a ticket sale for a passholder
   *
   * @param string $uitpas_number The UitPas number
   * @param string $cdbid The event CDBID
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function registerTicketSale($uitpas_number, $cdbid, $consumer_key_counter = NULL) {
    $data = array();

    if ($consumer_key_counter) {
      $data['balieConsumerKey'] = $consumer_key_counter;
    }

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
  public function cancelTicketSale($uitpas_number, $cdbid, $consumer_key_counter = NULL) {
    $data = array();

    if ($consumer_key_counter) {
      $data['balieConsumerKey'] = $consumer_key_counter;
    }

    try {
      $this->oauth_client->authenticatedPostAsXml('uitpas/cultureevent/' . $cdbid . '/cancel/' . $uitpas_number, $data);
      return true;
    }
    catch (Exception $e) {
      return false;
    }
  }

  public function getPassholderForTicketSale( CultureFeed_Uitpas_Event_TicketSale $ts, $consumer_key_counter = NULL ) {
    $user_id = $ts->userId;
    return $this->getPassholderByUser($user_id, $consumer_key_counter);
  }

  /**
   * {@inheritdoc}
   */
  public function searchCheckins(CultureFeed_Uitpas_Event_Query_SearchCheckinsOptions $query, $consumer_key_counter = NULL, $method = CultureFeed_Uitpas::USER_ACCESS_TOKEN) {
    $data = $query->toPostData();

    if ($consumer_key_counter) {
      $data['balieConsumerKey'] = $consumer_key_counter;
    }

    $path = 'uitpas/cultureevent/searchCheckins';

    if ($method == CultureFeed_Uitpas::USER_ACCESS_TOKEN) {
      $result = $this->oauth_client->authenticatedGetAsXml($path, $data);
    }
    else {
      $result = $this->oauth_client->consumerGetAsXml($path, $data);
    }

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $checkins = array();
    $objects = $xml->xpath('/response/checkinActivities/checkinActivity');
    $total = $xml->xpath_int('/response/total');

    foreach ($objects as $object) {
      $checkins[] = CultureFeed_Uitpas_Event_CheckinActivity::createFromXML($object);
    }

    return new CultureFeed_ResultSet($total, $checkins);
  }
  
  /**
   * Search for checkins
   *
   * @param CultureFeed_Uitpas_Passholder_Query_SearchCheckinsOptions $query The query
   */
  public function searchPassholderCheckins(CultureFeed_Uitpas_Passholder_Query_SearchCheckinsOptions $query) {
    $data = $query->toPostData();
    $result = $this->oauth_client->authenticatedGetAsXml('uitpas/cultureevent/searchCheckins', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $checkins = array();
    $objects = $xml->xpath('/response/checkinActivities/checkinActivity');
    $total = $xml->xpath_int('/response/total');

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
  public function searchEvents(CultureFeed_Uitpas_Event_Query_SearchEventsOptions $query) {
    $data = $query->toPostData();
    //$data['email'] = "sven@cultuurnet.be";

    $result = $this->oauth_client->consumerGetAsXml('uitpas/cultureevent/search', $data);
    //dpm( $data );
    //dpm( $result );

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

  public function searchCounters(CultureFeed_Uitpas_Counter_Query_SearchCounterOptions $query, $method = CultureFeed_Uitpas::CONSUMER_REQUEST) {
    $data = $query->toPostData();

    if ($method == CultureFeed_Uitpas::CONSUMER_REQUEST) {
      $result = $this->oauth_client->consumerGetAsXml('uitpas/balie/search', $data);
    }
    else {
      $result = $this->oauth_client->authenticatedGetAsXml('uitpas/balie/search', $data);
    }

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $counters = array();
    $objects = $xml->xpath('/response/balies/balie');
    $total = $xml->xpath_int('/response/total');

    foreach ($objects as $object) {
      $counters[] = CultureFeed_Uitpas_Counter::createFromXML($object);
    }

    return new CultureFeed_ResultSet($total, $counters);
  }

  /**
   * Search for point of sales
   *
   * @param CultureFeed_Uitpas_Counter_Query_SearchPointsOfSaleOptions $query The query
   * @param string $method The request method
   */
  public function searchPointOfSales(CultureFeed_Uitpas_Counter_Query_SearchPointsOfSaleOptions $query, $method = CultureFeed_Uitpas::CONSUMER_REQUEST) {
    $data = $query->toPostData();

    if ($method == CultureFeed_Uitpas::CONSUMER_REQUEST) {
      $result = $this->oauth_client->consumerGetAsXml('uitpas/balie/pos', $data);
    }
    else {
      $result = $this->oauth_client->authenticatedGetAsXml('uitpas/balie/pos', $data);
    }

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $pos = array();
    $objects = $xml->xpath('/response/balies/balie');
    $total = $xml->xpath_int('/response/total');

    foreach ($objects as $object) {
      $pos[] = CultureFeed_Uitpas_Counter::createFromXML($object);
    }

    return new CultureFeed_ResultSet($total, $pos);
  }

  public function searchTicketSales(CultureFeed_Uitpas_Event_Query_SearchTicketSalesOptions $query) {
    $data = $query->toPostData();


    $result = $this->oauth_client->authenticatedGetAsXml('uitpas/cultureevent/searchTicketsales', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $total = $xml->xpath_int('/response/total');
    $objects = $xml->xpath('/response/ticketSales/ticketSale');
    $ticket_sales = array();

    foreach ($objects as $object) {
      $ticket_sales[] = CultureFeed_Uitpas_Event_TicketSale::createFromXML($object);
    }

    return new CultureFeed_ResultSet($total, $ticket_sales);
  }

  /**
   * Add a member to a counter.
   *
   * @param string $uid The Culturefeed user ID
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function addMemberToCounter($uid, $consumer_key_counter = NULL) {
    $data = array(
      'uid' => $uid,
    );

    if ($consumer_key_counter) {
      $data['balieConsumerKey'] = $consumer_key_counter;
    }

    $this->oauth_client->authenticatedPost('uitpas/balie/member', $data);
  }

  public function removeMemberFromCounter($uid, $consumer_key_counter = NULL) {
    $data = array(
      'uid' => $uid,
    );

    if ($consumer_key_counter) {
      $data['balieConsumerKey'] = $consumer_key_counter;
    }

    $this->oauth_client->authenticatedPost('uitpas/balie/removeMember', $data);
  }

  public function getCardCounters($consumer_key_counter = NULL) {
    $data = array();

    if ($consumer_key_counter) {
      $data['balieConsumerKey'] = $consumer_key_counter;
    }

    $result = $this->oauth_client->authenticatedGetAsXml('uitpas/balie/countCards', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $counters = array();
    $objects = $xml->xpath('/response/counters/counter');

    foreach ($objects as $object) {
      $counters[] = CultureFeed_Uitpas_Counter_CardCounter::createFromXML($object);
    }

    return $counters;

  }

  public function getMembersForCounter($consumer_key_counter = NULL) {
    $data = array();

    if ($consumer_key_counter) {
      $data['balieConsumerKey'] = $consumer_key_counter;
    }

    $result = $this->oauth_client->authenticatedGetAsXml('uitpas/balie/listEmployees', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $data = array();

    foreach ($xml->xpath('/response/admins/admin') as $object) {
      $data['admins'][] = CultureFeed_Uitpas_Counter_Member::createFromXML($object);
    }

    foreach ($xml->xpath('/response/members/member') as $object) {
      $data['members'][] = CultureFeed_Uitpas_Counter_Member::createFromXML($object);
    }

    return $data;
  }

  /**
   * {@inheritdoc}
   */
  public function searchCountersForMember($uid) {
    $data = array(
      'uid' => $uid,
    );

    $result = $this->oauth_client->authenticatedGetAsXml('uitpas/balie/list', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $counters = array();
    $objects = $xml->xpath('balies/balie');
    $total = count($objects);

    foreach ($objects as $object) {
      $counters[] = CultureFeed_Uitpas_Counter_Employee::createFromXML($object);
    }

    return new CultureFeed_ResultSet($total, $counters);
  }

  public function getDevices($consumer_key_counter = NULL) {
    $data = array();

    if ($consumer_key_counter) {
      $data['balieConsumerKey'] = $consumer_key_counter;
    }

    $result = $this->oauth_client->authenticatedGetAsXml('uitpas/cid/list', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $devices = array();
    $objects = $xml->xpath('/response/cids/cid');

    foreach ($objects as $object) {
      $devices[] = CultureFeed_Uitpas_Counter_Device::createFromXML($object);
    }

    return $devices;
  }

  public function getEventsForDevice($consumer_key_device, $consumer_key_counter = NULL) {
    $data = array();

    if ($consumer_key_counter) {
      $data['balieConsumerKey'] = $consumer_key_counter;
    }

    $result = $this->oauth_client->authenticatedGetAsXml('uitpas/cid/' . $consumer_key_device, $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    return CultureFeed_Uitpas_Counter_Device::createFromXml($xml->xpath('/response', FALSE));
  }

  public function connectDeviceWithEvent($consumer_key_device, $cdbid, $consumer_key_counter = NULL) {
    $data = array(
      'cdbid' => $cdbid,
      'cidConsumerKey' => $consumer_key_device,
    );

    if ($consumer_key_counter) {
      $data['balieConsumerKey'] = $consumer_key_counter;
    }

    $result = $this->oauth_client->authenticatedPostAsXml('uitpas/cid/connect', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    return CultureFeed_Uitpas_Counter_Device::createFromXml($xml->xpath('/response', FALSE));
  }

  /**
   * (non-PHPdoc)
   * @see CultureFeed_Uitpas::getWelcomeAdvantage()
   */
  public function getWelcomeAdvantage($id, CultureFeed_Uitpas_Promotion_PassholderParameter $passholder = NULL) {
    $path = 'uitpas/promotion/welcomeAdvantage/' . $id;

    $params = array();

    if ($passholder) {
      $params += $passholder->params();
    }

    $result = $this->oauth_client->consumerGetAsXml($path, $params);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $advantage = CultureFeed_Uitpas_Passholder_WelcomeAdvantage::createFromXML($xml);

    return $advantage;
  }

  public function getPointsPromotion($id, CultureFeed_Uitpas_Promotion_PassholderParameter $passholder = NULL) {
    $path = 'uitpas/promotion/pointsPromotion/' . $id;

    $params = array();

    if ($passholder) {
      $params += $passholder->params();
    }

    $result = $this->oauth_client->consumerGetAsXml($path, $params);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $promotion = CultureFeed_Uitpas_Passholder_PointsPromotion::createFromXML($xml);

    return $promotion;
  }
}
