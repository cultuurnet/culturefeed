<?php

/**
 * @file
 * Class to work with Culturefeeds Entry API.
 */

class CultureFeed_EntryApi implements CultureFeed_EntryApi_IEntryApi {

  /**
   * Status code when an item has been succesfully created.
   * @var string
   */
  const CODE_ITEM_CREATED = 'ItemCreated';

  /**
   * Status code when an item has been succesfully updated.
   * @var string
   */
  const CODE_ITEM_MODIFIED = 'ItemModified';

  /**
   * Status code when an item has bene succesfully deleted.
   * @var string
   */
  const CODE_ITEM_DELETED = 'ItemWithdrawn';

  /**
   * Status code when the keywords are succesfully updated.
   * @var string
   */
  const CODE_KEYWORDS_CREATED = 'KeywordsCreated';

  /**
   * Status code when the keyword is succesfully deleted.
   * @var string
   */
  const CODE_KEYWORD_DELETED = 'KeywordWithdrawn';

  /**
   * Status code when the keyword can only be used by admins.
   * @var string
   */
  const CODE_KEYWORD_PRIVATE = 'PrivateKeyword';

  /**
   * Constructor for a new CultureFeed_EntryApi instance.
   *
   * @param CultureFeed_OAuthClient $oauth_client
   *   A OAuth client to make requests.
   *
   */
  public function __construct(CultureFeed_OAuthClient $oauth_client) {
    $this->oauth_client = $oauth_client;
  }

  /**
   * Get an event.
   *
   * @param string $id
   *   ID of the event to load.
   *
   * @return CultureFeed_Cdb_Item_Event
   * @throws CultureFeed_ParseException
   */
  public function getEvent($id) {

    $result = $this->oauth_client->authenticatedGetAsXml('event/' . $id);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $eventXml = $xml->events->event;

    return CultureFeed_Cdb_Item_Event::parseFromCdbXml($eventXml);

  }

  /**
   * Create a new event.
   *
   * @param CultureFeed_Cdb_Item_Event $event
   *   The event to create.
   *
   * @return string
   *   The id from the newly created event.
   *
   */
  public function createEvent(CultureFeed_Cdb_Item_Event $event) {

    $cdb = new CultureFeed_Cdb_Default();
    $cdb->addItem($event);
    $cdb_xml = $cdb->__toString();

    $result = $this->oauth_client->authenticatedPostAsXml('event', array('raw_data' => $cdb_xml), TRUE);
    $xml = $this->validateResult($result, self::CODE_ITEM_CREATED);

    return basename($xml->xpath_str('/rsp/link'));

  }

  /**
   * Update an event.
   *
   * @param CultureFeed_Cdb_Item_Event $event
   *   The event to update.
   */
  public function updateEvent(CultureFeed_Cdb_Item_Event $event) {

    $cdb = new CultureFeed_Cdb_Default();
    $cdb->addItem($event);

    $result = $this->oauth_client->authenticatedPostAsXml('event/' . $event->getCdbId(), array('raw_data' => $cdb->__toString()), TRUE);
    $xml = $this->validateResult($result, self::CODE_ITEM_MODIFIED);

  }

  /**
   * Delete an event.
   *
   * @param string $id
   *   ID from the event.
   */
  public function deleteEvent($id) {

    $result = $this->oauth_client->authenticatedDeleteAsXml('event/' . $id);
    $xml = $this->validateResult($result, self::CODE_ITEM_DELETED);

  }

  /**
   * Add tags to an event.
   *
   * @param CultureFeed_Cdb_Item_Event $event
   *   Event where the tags will be added to.
   * @param array $keywords
   *   Tags to add.
   */
  public function addTagToEvent(CultureFeed_Cdb_Item_Event $event, $keywords) {
    $this->addTags('event', $event->getCdbId(), $keywords);
  }

  /**
   * Remove tags from an event.
   *
   * @param CultureFeed_Cdb_Item_Event $event
   *   Event where the tags will be removed from.
   * @param string $keyword
   *   Tag to remove.
   */
  public function removeTagFromEvent(CultureFeed_Cdb_Item_Event $event, $keyword) {
    $this->removeTag('event', $event->getCdbId(), $keyword);
  }

  /**
   * Get an actor.
   *
   * @param string $id
   *   ID of the actor to load.
   *
   * @return CultureFeed_Cdb_Item_Actor
   * @throws CultureFeed_ParseException
   */
  public function getActor($id) {

    $result = $this->oauth_client->authenticatedGetAsXml('actor/' . $id);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $actorXml = $xml->actors->actor;

    return CultureFeed_Cdb_Item_Actor::parseFromCdbXml($actorXml);

  }

  /**
   * Create a new actor.
   *
   * @param CultureFeed_Cdb_Item_Actor $actor
   *   The actor to create.
   *
   * @return string
   *   The id from the newly created actor.
   *
   */
  public function createActor(CultureFeed_Cdb_Item_Actor $actor) {

    $cdb = new CultureFeed_Cdb_Default();
    $cdb->addItem($actor);
    $cdb_xml = $cdb->__toString();

    $result = $this->oauth_client->authenticatedPostAsXml('actor', array('raw_data' => $cdb_xml), TRUE);
    $xml = $this->validateResult($result, self::CODE_ITEM_CREATED);

    return basename($xml->xpath_str('/rsp/link'));

  }

 /**
   * Update an actor.
   *
   * @param CultureFeed_Cdb_Item_Actor $actor
   *   The actor to update.
   */
  public function updateActor(CultureFeed_Cdb_Item_Actor $actor) {

    $cdb = new CultureFeed_Cdb_Default();
    $cdb->addItem($actor);

    $result = $this->oauth_client->authenticatedPostAsXml('actor/' . $actor->getCdbId(), array('raw_data' => $cdb->__toString()), TRUE);
    $xml = $this->validateResult($result, self::CODE_ITEM_MODIFIED);

  }

  /**
   * Delete an actor.
   *
   * @param string $id
   *   ID from the actor.
   */
  public function deleteActor($id) {

    $result = $this->oauth_client->authenticatedDeleteAsXml('actor/' . $id);
    $xml = $this->validateResult($result, self::CODE_ITEM_DELETED);

  }

  /**
   * Search events on the entry api.
   *
   * @param string $query
   *   String to search for.
   * @param string $updated_since
   *   Correct ISO date format (yyyy-m-dTH): example 2012-12-20T12:21.
   * @param int $page
   *   Page number to get.
   * @param int $page_length
   *   Items requested for current page.
   * @param string $sort
   *   Sort type.
   *
   * @return CultureFeed_Cdb_List_Results
   */
  public function getEvents($query, $updated_since = NULL, $page = NULL, $page_length = NULL, $sort = NULL) {
    return $this->search('event', $query, $updated_since, $page, $page_length, $sort);
  }

  /**
   * Search productions on the entry api.
   *
   * @param string $query
   *   Query to search.
   * @param string $updated_since
   *   Correct ISO date format (yyyy-m-dTH): example 2012-12-20T12:21.
   * @param int $page
   *   Page number to get.
   * @param int $page_length
   *   Items requested for current page.
   * @param string $sort
   *   Sort type.
   *
   * @return CultureFeed_Cdb_List_Results
   */
  public function getProductions($query, $updated_since = NULL, $page = NULL, $page_length = NULL, $sort = NULL) {
    return $this->search('production', $query, $updated_since, $page, $page_length, $sort);
  }

  /**
   * Search actors on the entry api.
   *
   * @param string $query
   *   String to search for.
   * @param string $updated_since
   *   Correct ISO date format (yyyy-m-dTH): example 2012-12-20T12:21.
   * @param int $page
   *   Page number to get.
   * @param int $page_length
   *   Items requested for current page.
   * @param string $sort
   *   Sort type.
   *
   * @return CultureFeed_Cdb_List_Results
   */
  public function getActors($query, $updated_since = NULL, $page = NULL, $page_length = NULL, $sort = NULL) {
    return $this->search('actor', $query, $updated_since, $page, $page_length, $sort);
  }

  /**
   * Search items on the entry api.
   *
   * @param string $query
   *   Query to search.
   * @param string $updated_since
   *   Correct ISO date format (yyyy-m-dTH): example 2012-12-20T12:21
   * @param int $page
   *   Page number to get.
   * @param int $page_length
   *   Items requested for current page.
   * @param string $sort
   *   Sort type.
   *
   * @return CultureFeed_Cdb_List_Results
   */
  private function search($type, $query, $updated_since, $page, $page_length, $sort) {

    $args = array(
      'q' => $query
    );

    if ($updated_since) {
      $args['updatedsince'] = $updated_since;
    }

    if ($page) {
      $args['page'] = $page;
    }

    if ($page_length) {
      $args['pagelength'] = $pagelength;
    }

    if ($sort) {
      $args['sort'] = $sort;
    }

    $result = $this->oauth_client->authenticatedGetAsXml($type, $args);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    return CultureFeed_Cdb_List_Results::parseFromCdbXml($xml);

  }

  /**
   * Add tags to an item.
   *
   * @param string $type
   *   Type of item to update.
   * @param $id
   *   Id from the event / actor / production to update.
   * @param array $keywords
   *   Keywords to add.
   */
  private function addTags($type, $id, $keywords) {

    $result = $this->oauth_client->authenticatedPostAsXml($type . '/' . $id . '/keywords', array('keywords' => implode(';', $keywords)));
    $xml = $this->validateResult($result, self::CODE_KEYWORDS_CREATED);

  }

  /**
   * Remove tags from an item.
   *
   * @param string $type
   *   Type of item to update.
   * @param string $id
   *   Id from the event / actor / production to update.
   * @param string $keyword
   *   Tag to remove.
   */
  private function removeTag($type, $id, $keyword) {

    $result = $this->oauth_client->authenticatedDeleteAsXml($type . '/' . $id . '/keywords', array('keyword' => $keyword));
    $xml = $this->validateResult($result, self::CODE_KEYWORD_DELETED);
  }

  /**
   * Validate the request result.
   *
   * @param string $result
   *   Result from the request.
   * @param string $valid_status_code
   *   Status code if this is a valid request.
   * @return CultureFeed_SimpleXMLElement The parsed xml.
   *
   * @throws CultureFeed_ParseException
   *   If the result could not be parsed.
   * @throws CultureFeed_InvalidCodeException
   *   If the result code was not itemCreated.
   */
  private function validateResult($result, $valid_status_code) {

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $status_code = $xml->xpath_str('/rsp/code');
    if ($status_code == $valid_status_code) {
      return $xml;
    }

    throw new CultureFeed_InvalidCodeException($xml->xpath_str('/rsp/message'), $status_code);

  }

}
