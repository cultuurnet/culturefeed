<?php

/**
 * @file
 * Class to work with Culturefeeds Entry API.
 */

class CultureFeed_EntryApi implements ICultureFeed_EntryApi {

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
   * @return CultureFeed_Cdb_Event
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

    return CultureFeed_Cdb_Event::parseFromCdbXml($xml);

  }

  /**
   * Create a new event.
   *
   * @param CultureFeed_Cdb_Event $event
   *   The event to create.
   *
   * @return string
   *   The id from the newly created event.
   *
   */
  public function createEvent(CultureFeed_Cdb_Event $event) {

    $cdb = new CultureFeed_Cdb();
    $cdb->addItem('events', $event);
    $cdb_xml = $cdb->getXml();

    $result = $this->oauth_client->authenticatedPostAsXml('event', array('raw_data' => $cdb_xml), TRUE);
    $xml = $this->validateResult($result, self::CODE_ITEM_CREATED);

    return basename($xml->xpath_str('/rsp/link'));

  }

  /**
   * Update an event.
   *
   * @param CultureFeed_Cdb_Event $event
   *   The event to update.
   */
  public function updateEvent(CultureFeed_Cdb_Event $event) {

    $cdb = new CultureFeed_Cdb();
    $cdb->addItem('events', $event);
    $cdb_xml = $cdb->getXml();

    $result = $this->oauth_client->authenticatedPostAsXml('event/' . $event->getExternalId(), array('raw_data' => $cdb_xml), TRUE);
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
   * @param CultureFeed_Cdb_Event $event
   *   Event where the tags will be added to.
   * @param array $keywords
   *   Tags to add.
   */
  public function addTagToEvent(CultureFeed_Cdb_Event $event, $keywords) {
    $this->addTags('event', $event->getExternalId(), $keywords);
  }

  /**
   * Remove tags from an event.
   *
   * @param CultureFeed_Cdb_Event $event
   *   Event where the tags will be removed from.
   * @param string $keyword
   *   Tag to remove.
   */
  public function removeTagFromEvent(CultureFeed_Cdb_Event $event, $keyword) {
    $this->removeTag('event', $event->getExternalId(), $keyword);
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
   * @return The parsed xml.
   *
   * @throws CultureFeed_ParseException
   *   If the result could not be parsed.
   * @throws InvalidCodeException
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