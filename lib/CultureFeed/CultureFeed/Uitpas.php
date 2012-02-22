<?php

/**
 *
 */
interface CultureFeed_Uitpas {

  const CONSUMER_REQUEST = 'ConsumeRequestr';
  const USER_ACCESS_TOKEN = 'UserAccessToken';

  /**
   * Get the associations.
   *
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function getAssociations($consumer_key_counter);

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
   */
  public function getDistributionKeysForOrganizer($cdbid);

  /**
   * Get the price of the UitPas.
   */
  public function getPrice();

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
  public function resendActivationEmail($uitpas_number, $consumer_key_counter);

  /**
   * Get a passholder based on the UitPas number.
   *
   * @param string $uitpas_number The UitPas number
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function getPassholderByUitpasNumber($uitpas_number, $consumer_key_counter);

  /**
   * Get a passholder based on the user ID
   *
   * @param string $user_id The user ID
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function getPassholderByUser($user_id, $consumer_key_counter);

  /**
   * Search for passholders.
   *
   * @param CultureFeed_Uitpas_Passholder_Query_SearchPassholdersOptions $query The query
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   * @param string $method The request method
   */
  public function searchPassholders(CultureFeed_Uitpas_Passholder_Query_SearchPassholdersOptions $query, $consumer_key_counter, $method = CultureFeed_Uitpas::CONSUMER_REQUEST);

  /**
   * Get the welcome advantages for a passholder.
   *
   * @param CultureFeed_Uitpas_Passholder_Query_WelcomeAdvantagesOptions $query The query
   * @param string $uitpas_number The UitPas number
   */
  public function getWelcomeAdvantagesForPassholder(CultureFeed_Uitpas_Passholder_Query_WelcomeAdvantagesOptions $query, $consumer_key_counter);

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
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   * @param int $welcome_advantage_id Identification welcome advantage
   */
  public function cashInWelcomeAdvantage($uitpas_number, $consumer_key_counter, $welcome_advantage_id);

  /**
   * Get the redeem options
   *
   * @param CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions $query The query
   */
  public function getPromotionPoints(CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions $query);

  /**
   * Cash in promotion points for a UitPas.
   *
   * @param string $uitpas_number The UitPas number
   * @param int $points_promotion_id The identification of the redeem option
   * @param string $counter The name of the UitPas counter
   */
  public function cashInPromotionPoints($uitpas_number, $consumer_key_counter, $points_promotion_id);

  /**
   * Upload a picture for a given passholder.
   *
   * @param string $id The user ID of the passholder
   * @param string $file_data The binary data of the picture
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function uploadPicture($id, $file_data, $consumer_key_counter);

  /**
   * Update a passholder.
   *
   * @param CultureFeed_Uitpas_Passholder $passholder The passholder to update.
   * 		The passholder is identified by ID. Only fields that are set will be updated.
   */
  public function updatePassholder(CultureFeed_Uitpas_Passholder $passholder);

  /**
   * Block a UitPas.
   *
   * @param string $uitpas_number The UitPas number
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function blockUitpas($uitpas_number, $consumer_key_counter);

  /**
   * Search for welcome advantages.
   *
   * @param CultureFeed_Uitpas_Promotion_Query_WelcomeAdvantagesOptions $query The query
   */
  public function searchWelcomeAdvantages(CultureFeed_Uitpas_Promotion_Query_WelcomeAdvantagesOptions $query);

  /**
   * Get a passholder based on the UitPas chip number.
   *
   * @param string $chip_number The chipnumber of the UitPas
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function getPassholderForChipNumber($chip_number, $consumer_key_counter);

  /**
   * Register a ticket sale for a passholder
   *
   * @param string $uitpas_number The UitPas number
   * @param string $cdbid The event CDBID
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function registerTicketSale($uitpas_number, $cdbid, $consumer_key_counter);

  /**
   * Cancel a ticket sale for a passholder
   *
   * @param string $uitpas_number The UitPas number
   * @param string $cdbid The event CDBID
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function cancelTicketSale($uitpas_number, $cdbid, $consumer_key_counter);

  /**
   * Search for checkins
   *
   * @param CultureFeed_Uitpas_Passholder_Query_SearchCheckinsOptions $query The query
   */
  public function searchCheckins(CultureFeed_Uitpas_Passholder_Query_SearchCheckinsOptions $query);

  /**
   * Search for Uitpas events
   *
   * @param CultureFeed_Uitpas_Passholder_Query_SearchEventsOptions $query The query
   */
  public function searchEvents(CultureFeed_Uitpas_Passholder_Query_SearchEventsOptions $query);

  /**
   * Search for point of sales
   *
   * @param CultureFeed_Uitpas_Counter_Query_SearchPointsOfSaleOptions $query The query
   */
  public function searchPointOfSales(CultureFeed_Uitpas_Counter_Query_SearchPointsOfSaleOptions $query);

  /**
   * Add a member to a counter.
   *
   * @param string $uid The Culturefeed user ID
   * @param string $consumer_key_counter The consumer key of the counter from where the request originates
   */
  public function addMemberToCounter($uid, $consumer_key_counter);

  /**
   * Search for counters for a given member
   *
   * @param string $uid The Culturefeed user ID
   */
  public function searchCountersForMember($uid);

}