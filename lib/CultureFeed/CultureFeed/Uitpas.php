<?php

/**
 *
 */
interface CultureFeed_Uitpas {

  /**
   * Get the distribution keys for an organizer.
   *
   * @param string $cdbid The CDBID of the organizer
   */
  public function getDistributionKeysForOrganizer($cdbid);

  /**
   * Register a set of distribution keys for an organizer. The entire set (including existing)
   * of distribution keys must be provided.
   *
   * @param string $cdbid The CDBID of the organizer
   * @param array $distribution_keys The identification of the distribution key
   */
  public function registerDistributionKeysForOrganizer($cdbid, $distribution_keys);

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
   * Get a passholder based on the UitPas number.
   *
   * @param string $uitpas_number The UitPas number
   */
  public function getPassholder($uitpas_number);

  /**
   * Search for passholders.
   *
   * @param CultureFeed_Uitpas_SearchPassHoldersQuery $query The query
   */
  public function searchPassholders(CultureFeed_Uitpas_SearchPassHoldersQuery $query);

  /**
   * Get the welcome advantages for a passholder.
   *
   * @param string $uitpas_number The UitPas number
   */
  public function getWelcomeAdvantagesForPassholder($uitpas_number);

  /**
   * Check in a passholder.
   *
   * Provide either a UitPas number or chip number. You cannot provide both.
   *
   * @param CultureFeed_Uitpas_Passholder_Event $event The event data object
   */
  public function checkinPassholder(CultureFeed_Uitpas_Passholder_Event $event);

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
   * @param CultureFeed_Uitpas_SearchWelcomeAdvantagesQuery $query The query
   */
  public function searchWelcomeAdvantages(CultureFeed_Uitpas_SearchWelcomeAdvantagesQuery $query);

  /**
   * Get a passholder based on the UitPas chip number.
   *
   * @param string $chip_number The chipnumber of the UitPas
   */
  public function getPassholderForChipNumber($chip_number);

  /**
   * Get the events for a given passholder.
   *
   * @param string $uitpas_number The UitPas number
   * @param DateTime $date_from Start date
   * @param DateTime $date_to End date
   */
  public function getEventsForPassholder($uitpas_number, $date_from, $date_to);

  /**
   * Register a ticket sale for a passholder
   *
   * @param string $uitpas_number The UitPas number
   * @param string $cdbid The event CDBID
   */
  public function registerTicketSale($uitpas_number, $cdbid);

  /**
   * Cancel a ticket sale for a passholder
   *
   * @param string $uitpas_number The UitPas number
   * @param string $cdbid The event CDBID
   */
  public function cancelTicketSale($uitpas_number, $cdbid);

  /**
   * Get the accumulated points of a passholder.
   *
   * @param CultureFeed_Uitpas_AccumulatedPointsQuery $query The query
   */
  public function getAccumulatedPoints(CultureFeed_Uitpas_AccumulatedPointsQuery $query);

  /**
   * Search for Uitpas events
   *
   * @param CultureFeed_Uitpas_SearchEventsQuery $query The query
   */
  public function searchEvents(CultureFeed_Uitpas_SearchEventsQuery $query);

  /**
   * Search for point of sales
   *
   * @param CultureFeed_Uitpas_SearchPointOfSalesQuery $query The query
   */
  public function searchPointOfSales(CultureFeed_Uitpas_SearchPointOfSalesQuery $query);

  /**
   * Add a member to a counter.
   *
   * @param int $id The counter ID
   * @param string $uid The Culturefeed user ID
   */
  public function addMemberToCounter($id, $uid);

  /**
   * Search for counters for a given member
   *
   * @param string $uid The Culturefeed user ID
   */
  public function searchCountersForMember($uid);

}