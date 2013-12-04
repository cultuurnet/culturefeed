<?php

/**
 *
 */
interface CultureFeed_Uitpas {

  const CONSUMER_REQUEST = 'ConsumerRequest';
  const USER_ACCESS_TOKEN = 'UserAccessToken';

  /**
   * Get the associations.
   *
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   *
   * @return CultureFeed_ResultSet
   */
  public function getAssociations($consumer_key_counter = NULL);

  /**
   * Register a set of distribution keys for an organizer. The entire set (including existing)
   * of distribution keys must be provided.
   *
   * @param string $cdbid The CDBID of the organizer
   * @param array $distribution_keys The identification of the distribution key
   */
  public function registerDistributionKeysForOrganizer($cdbid, $distribution_keys);

  /**
   * Get the distribution keys for a given organizer.
   *
   * @param string $cdbid The CDBID of the given organizer
   * @return CultureFeed_ResultSet The set of distribution keys
   */
  public function getDistributionKeysForOrganizer($cdbid);

  /**
   * Get the price of the UitPas.
   */
  public function getPrice($consumer_key_counter = NULL);

  /**
   * Create a new UitPas passholder.
   *
   * @param CultureFeed_Uitpas_Passholder $passholder The new passholder
   */
  public function createPassholder(CultureFeed_Uitpas_Passholder $passholder);

  /**
   * Create a new membership for a UitPas passholder.
   *
   * @param string $id The user ID of the passholder
   * @param string $organization The name of the organization
   * @param DateTime $end_date The membership's organization end date
   */
  public function createMembershipForPassholder(CultureFeed_Uitpas_Passholder_Membership $membership);

  /**
   * Resend the activation e-mail for a passholder
   *
   * @param string $uitpas_number The UitPas number
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function resendActivationEmail($uitpas_number, $consumer_key_counter = NULL);

  /**
   * Get a passholder based on the UitPas number.
   *
   * @param string $uitpas_number The UitPas number
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   *
   * @return CultureFeed_Uitpas_Passholder
   */
  public function getPassholderByUitpasNumber($uitpas_number, $consumer_key_counter = NULL);

  /**
   * Get a passholder based on the user ID
   *
   * @param string $user_id The user ID
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   *
   * @return CultureFeed_Uitpas_Passholder
   */
  public function getPassholderByUser($user_id, $consumer_key_counter = NULL);

  /**
   * Search for passholders.
   *
   * @param CultureFeed_Uitpas_Passholder_Query_SearchPassholdersOptions $query The query
   * @param string $method The request method
   *
   * @return CultureFeed_ResultSet
   */
  public function searchPassholders(CultureFeed_Uitpas_Passholder_Query_SearchPassholdersOptions $query, $method = CultureFeed_Uitpas::CONSUMER_REQUEST);

  /**
   * Get the welcome advantages for a passholder.
   *
   * @param CultureFeed_Uitpas_Passholder_Query_WelcomeAdvantagesOptions $query The query
   * @param string $uitpas_number The UitPas number
   */
  public function getWelcomeAdvantagesForPassholder(CultureFeed_Uitpas_Passholder_Query_WelcomeAdvantagesOptions $query);

  /**
   * Check in a passholder.
   *
   * Provide either a UitPas number or chip number. You cannot provide both.
   *
   * @param CultureFeed_Uitpas_Passholder_Query_CheckInPassholderOptions $event The event data object
   */
  public function checkinPassholder(CultureFeed_Uitpas_Passholder_Query_CheckInPassholderOptions $event);

  /**
   * Cash in a welcome advantage.
   *
   * @param string $uitpas_number The UitPas number
   * @param int $welcome_advantage_id Identification welcome advantage
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function cashInWelcomeAdvantage($uitpas_number, $welcome_advantage_id, $consumer_key_counter = NULL);

  /**
   * Get the redeem options
   *
   * @param CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions $query The query
   *
   * @return CultureFeed_ResultSet
   */
  public function getPromotionPoints(CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions $query);

  public function getCashedInPromotionPoints(CultureFeed_Uitpas_Passholder_Query_SearchCashedInPromotionPointsOptions $query);

  /**
   * Cash in promotion points for a UitPas.
   *
   * @param string $uitpas_number The UitPas number
   * @param int $points_promotion_id The identification of the redeem option
   * @param string $counter The name of the UitPas counter
   */
  public function cashInPromotionPoints($uitpas_number, $points_promotion_id, $consumer_key_counter = NULL);

  /**
   * Upload a picture for a given passholder.
   *
   * @param string $id The user ID of the passholder
   * @param string $file_data The binary data of the picture
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function uploadPicture($id, $file_data, $consumer_key_counter = NULL);

  /**
   * Update a passholder.
   *
   * @param CultureFeed_Uitpas_Passholder $passholder The passholder to update.
   * 		The passholder is identified by ID. Only fields that are set will be updated.
   */
  public function updatePassholder(CultureFeed_Uitpas_Passholder $passholder);

  /**
   * Update a passholder's card system preferences.
   *
   * @param CultureFeed_Uitpas_Passholder_CardSystemPreferences $preferences The passholder's card preferences to update.
   *        The card system preferences are identified by user id and card system id. Only fields that are set will be updated.
   */
  public function updatePassholderCardSystemPreferences(CultureFeed_Uitpas_Passholder_CardSystemPreferences $preferences);

  /**
   * Block a UitPas.
   *
   * @param string $uitpas_number The UitPas number
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function blockUitpas($uitpas_number, $consumer_key_counter = NULL);

  /**
   * Search for welcome advantages.
   *
   * @param CultureFeed_Uitpas_Promotion_Query_WelcomeAdvantagesOptions $query The query
   * @param string $method The request method
   */
  public function searchWelcomeAdvantages(CultureFeed_Uitpas_Promotion_Query_WelcomeAdvantagesOptions $query, $method = CultureFeed_Uitpas::CONSUMER_REQUEST);

  /**
   * Get info regarding a UiTPAS card based on chipNumber of uitpasNumber.
   *
   * @param CultureFeed_Uitpas_CardInfoQuery $card_query
   * @return CultureFeed_Uitpas_CardInfo
   */
  public function getCard(CultureFeed_Uitpas_CardInfoQuery $card_query);

  /**
   * Get the activitation link for a passholder which is not activated online yet.
   *
   * @param CultureFeed_Uitpas_Passholder_Query_ActivationData $activation_data
   * @param mixed $destination_callback
   *
   * @return string
   */
  public function getPassholderActivationLink(CultureFeed_Uitpas_Passholder_Query_ActivationData $activation_data, $destination_callback = NULL);

  /**
   * Constructs an activation link,
   *
   * @param string $uid
   * @param string $activation_code
   */
  public function constructPassHolderActivationLink($uid, $activation_code, $destination_callback = NULL);

  /**
   * Get the activitation link for a passholder which is not activated online yet,
   * chained with an authorization.
   *
   * @param string $uitpas_number
   * @param DateTime $date_of_birth
   * @param string $callback_url
   */
  public function getPassholderActivationLinkChainedWithAuthorization($uitpas_number, DateTime $date_of_birth, $callback_url);

  /**
   * Register a ticket sale for a passholder
   *
   * @param string $uitpas_number The UitPas number
   * @param string $cdbid The event CDBID
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function registerTicketSale($uitpas_number, $cdbid, $consumer_key_counter = NULL);

  /**
   * Register a new Uitpas
   *
   * @param CultureFeed_Uitpas_Passholder_Query_RegisterUitpasOptions $query The query
   */
  public function registerUitpas(CultureFeed_Uitpas_Passholder_Query_RegisterUitpasOptions $query);

  /**
   * Cancel a ticket sale for a passholder
   *
   * @param string $uitpas_number The UitPas number
   * @param string $cdbid The event CDBID
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function cancelTicketSale($uitpas_number, $cdbid, $consumer_key_counter = NULL);

  /**
   * Search for checkins
   *
   * @param CultureFeed_Uitpas_Event_Query_SearchCheckinsOptions $query The query
   * @param string $consumer_key_counter Optional consumer key of the counter.
   * @param string $method The OAuth request method, either consumer request or
   *   user request.
   *
   * @return CultureFeed_ResultSet
   */
  public function searchCheckins(CultureFeed_Uitpas_Event_Query_SearchCheckinsOptions $query, $consumer_key_counter = NULL, $method = CultureFeed_Uitpas::USER_ACCESS_TOKEN);

  /**
   * Search for Uitpas events
   *
   * @param CultureFeed_Uitpas_Passholder_Query_SearchEventsOptions $query The query
   */
  public function searchEvents(CultureFeed_Uitpas_Event_Query_SearchEventsOptions $query);

  /**
   * Search for point of sales
   *
   * @param CultureFeed_Uitpas_Counter_Query_SearchPointsOfSaleOptions $query The query
   * @param string $method The request method
   */
  public function searchPointOfSales(CultureFeed_Uitpas_Counter_Query_SearchPointsOfSaleOptions $query, $method = CultureFeed_Uitpas::CONSUMER_REQUEST);

  public function searchCounters(CultureFeed_Uitpas_Counter_Query_SearchCounterOptions $query, $method = CultureFeed_Uitpas::CONSUMER_REQUEST);

  /**
   * Add a member to a counter.
   *
   * @param string $uid The Culturefeed user ID
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function addMemberToCounter($uid, $consumer_key_counter = NULL);

  public function removeMemberFromCounter($uid, $consumer_key_counter = NULL);

  public function getMembersForCounter($consumer_key_counter = NULL);

  public function getCardCounters($consumer_key_counter = NULL);

  /**
   * Search for counters for a given member
   *
   * @param string $uid The Culturefeed user ID
   *
   * @return CultureFeed_ResultSet
   */
  public function searchCountersForMember($uid);

  public function getDevices($consumer_key_counter = NULL);

  public function getEventsForDevice($consumer_key_device, $consumer_key_counter = NULL);

  public function connectDeviceWithEvent($device_id, $cdbid, $consumer_key_counter = NULL);

  /**
   *
   * @param integer $id
   * @param CultureFeed_Uitpas_Promotion_PassholderParameter $passholder
   */
  public function getWelcomeAdvantage($id, CultureFeed_Uitpas_Promotion_PassholderParameter $passholder = NULL);

  /**
   *
   * @param integer $id
   * @param CultureFeed_Uitpas_Promotion_PassholderParameter $passholder
   */
  public function getPointsPromotion($id, CultureFeed_Uitpas_Promotion_PassholderParameter $passholder = NULL);

  /**
   * Register an event.
   *
   * @param CultureFeed_Uitpas_Event_CultureEvent $event The event data that needs to be sent over.
   * @return CultureFeed_Uitpas_Response
   */
  public function registerEvent(CultureFeed_Uitpas_Event_CultureEvent $event);
}
