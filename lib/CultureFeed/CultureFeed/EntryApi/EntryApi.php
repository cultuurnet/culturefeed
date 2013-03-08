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
   * Status code when a translation has been succesfully created.
   * @var string
   */
  const CODE_TRANSLATION_CREATED = 'TranslationCreated';

  /**
   * Status code when a translation has been succesfully deleted/withdrawn.
   * @var string
   */
  const CODE_TRANSLATION_WITHDRAWN = 'TranslationWithdrawn';
  
  /**
   * Status code when a link has been succesfully created.
   * @var string
   */
  const CODE_LINK_CREATED = 'LinkCreated';
  
  /**
   * Status code when a link has been succesfully withdrawn.
   * @var string
   */
  const CODE_LINK_WITHDRAWN = 'LinkWithdrawn';
  
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
   * Search events on the entry api.
   *
   * @param string $query
   *   String to search for.
   * @param int $page
   *   Page number to get.
   * @param int $page_length
   *   Items requested for current page.
   * @param string $sort
   *   Sort type.
   * @param string $updated_since
   *   Correct ISO date format (yyyy-m-dTH): example 2012-12-20T12:21.
   *
   * @return CultureFeed_Cdb_List_Results
   */
  public function getEvents($query, $page = NULL, $page_length = NULL, $sort = NULL, $updated_since = NULL) {
    return $this->search('event', $query, $page, $page_length, $sort, $updated_since);
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
  public function getProductions($query, $page = NULL, $page_length = NULL, $sort = NULL, $updated_since = NULL) {
    return $this->search('production', $query, $page, $page_length, $sort, $updated_since);
  }

  /**
   * Search actors on the entry api.
   *
   * @param string $query
   *   String to search for.
   * @param int $page
   *   Page number to get.
   * @param int $page_length
   *   Items requested for current page.
   * @param string $sort
   *   Sort type.
  * @param string $updated_since
   *   Correct ISO date format (yyyy-m-dTH): example 2012-12-20T12:21.
   *
   * @return CultureFeed_Cdb_List_Results
   */
  public function getActors($query, $page = NULL, $page_length = NULL, $sort = NULL, $updated_since = NULL) {
    return $this->search('actor', $query, $page, $page_length, $sort, $updated_since);
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
   * Get an production.
   *
   * @param string $id
   *   ID of the production to load.
   *
   * @return CultureFeed_Cdb_Item_Event
   * @throws CultureFeed_ParseException
   */
  public function getProduction($id) {

    $result = $this->oauth_client->authenticatedGetAsXml('production/' . $id);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $productionXml = $xml->productions->production;

    return CultureFeed_Cdb_Item_Production::parseFromCdbXml($productionXml);
  }

  /**
   * Create a new production.
   *
   * @param CultureFeed_Cdb_Item_Production $production
   *   The production to create.
   *
   * @return string
   *   The id from the newly created production.
   */
  public function createProduction(CultureFeed_Cdb_Item_Production $production) {

    $cdb = new CultureFeed_Cdb_Default();
    $cdb->addItem($production);
    $cdb_xml = $cdb->__toString();

    $result = $this->oauth_client->authenticatedPostAsXml('production', array('raw_data' => $cdb_xml), TRUE);
    $xml = $this->validateResult($result, self::CODE_ITEM_CREATED);

    return basename($xml->xpath_str('/rsp/link'));

  }

  /**
   * Update an production.
   *
   * @param CultureFeed_Cdb_Item_Production $production
   *   The production to update.
   */
  public function updateProduction(CultureFeed_Cdb_Item_Production $production) {
    $cdb = new CultureFeed_Cdb_Default();
    $cdb->addItem($production);
  
    $result = $this->oauth_client->authenticatedPostAsXml('production/' . $production->getCdbId(), array('raw_data' => $cdb->__toString()), TRUE);
    $xml = $this->validateResult($result, self::CODE_ITEM_MODIFIED);
  }

  /**
   * Delete an production.
   *
   * @param string $id
   *   ID from the production.
   */
  public function deleteProduction($id) {
    $result = $this->oauth_client->authenticatedDeleteAsXml('production/' . $id);
    $xml = $this->validateResult($result, self::CODE_ITEM_DELETED);
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

    $productionXml = $xml->actors->actor;

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
   * Add tags to a production.
   * 
   * @param CultureFeed_Cdb_Item_Production $production
   *   Production where the tags will be added to.
   * @param array $keywords
   *   Tags to add.
   */
  public function addTagToProduction(CultureFeed_Cdb_Item_Production $production, $keywords) {
    $this->addTags('production', $production->getCdbId(), $keywords);
  }

  /**
   * Add tags to a actor.
   * 
   * @param CultureFeed_Cdb_Item_Actor $actor
   *   Actor where the tags will be added to.
   * @param array $keywords
   *   Tags to add.
   */
  public function addTagToActor(CultureFeed_Cdb_Item_Actor $actor, $keywords) {
    $this->addTags('actor', $production->getCdbId(), $keywords);
  }

  /**
   * Add translation to an event.
   *
   * @param CultureFeed_Cdb_Item_Event $event
   *   Event where the translation will be added to.
   * @param String $lang
   *   Language to add.
   * @param String $title
   *   Title of the translation.
   * @param String $shortDescription
   *   Short description of the translation.
   * @param String $longDescription
   *   Long description of the translation.
   */
  public function addTranslationToEvent(CultureFeed_Cdb_Item_Event $event, $lang, $title = '', $shortDescription = '', $longDescription = '') {
    $this->addTranslation('event', $event->getCdbId(), $lang, $title, $shortDescription, $longDescription);
  }

  /**
   * Add translation to an actor.
   *
   * @param CultureFeed_Cdb_Item_Actor $actor
   *   Actor where the translation will be added to.
   * @param String $lang
   *   Language to add.
   * @param String $title
   *   Title of the translation.
   * @param String $shortDescription
   *   Short description of the translation.
   * @param String $longDescription
   *   Long description of the translation.
   */
  public function addTranslationToActor(CultureFeed_Cdb_Item_Actor $actor, $lang, $title = '', $shortDescription = '', $longDescription = '') {
    $this->addTranslation('actor', $actor->getCdbId(), $lang, $title, $shortDescription, $longDescription);
  }

  /**
   * Add translation to an production.
   *
   * @param CultureFeed_Cdb_Item_Production $production
   *   Production where the translation will be added to.
   * @param String $lang
   *   Language to add.
   * @param String $title
   *   Title of the translation.
   * @param String $shortDescription
   *   Short description of the translation.
   * @param String $longDescription
   *   Long description of the translation.
   */
  public function addTranslationToProduction(CultureFeed_Cdb_Item_Production $production, $lang, $title = '', $shortDescription = '', $longDescription = '') {
    $this->addTranslation('production', $production->getCdbId(), $lang, $title, $shortDescription, $longDescription);
  }
  
  /**
   * Add link to a production.
   *
   * @param CultureFeed_Cdb_Item_Production $production
   *   Production where the link will be added to.
   * @param String $link
   *   Link to add.
   * @param String $linktype
   *   Link type.["video", "text", "imageweb", "webresource", "reservations"]
   * @param String $lang
   *   Language of the link ["NL", "FR", "DE", "EN"]
   */
  public function addLinkToProduction(CultureFeed_Cdb_Item_Production $production, $link, $linktype = '', $lang = '') {
    $this->addLink('production', $production->getCdbId(), $link, $linktype, $lang);
  }  
  
  /**
   * Add link to an event.
   *
   * @param CultureFeed_Cdb_Item_Event $event
   *   Event where the link will be added to.
   * @param String $link
   *   Link to add.
   * @param String $linktype
   *   Link type.["video", "text", "imageweb", "webresource", "reservations"]
   * @param String $lang
   *   Language of the link ["NL", "FR", "DE", "EN"]
   */
  public function addLinkToEvent(CultureFeed_Cdb_Item_Event $event, $link, $linktype = '', $lang = '') {
    $this->addLink('event', $event->getCdbId(), $link, $linktype, $lang);
  }
  
  /**
   * Add link to an actor.
   *
   * @param CultureFeed_Cdb_Item_Actor $actor
   *   Actor where the link will be added to.
   * @param String $link
   *   Link to add.
   * @param String $linktype
   *   Link type.["video", "text", "imageweb", "webresource", "reservations"]
   * @param String $lang
   *   Language of the link ["NL", "FR", "DE", "EN"]
   */
  public function addLinkToActor(CultureFeed_Cdb_Item_Actor $actor, $link, $linktype = '', $lang = '') {
    $this->addLink('actor', $actor->getCdbId(), $link, $linktype, $lang);
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
<<<<<<< HEAD
   * Remove tags from an production.
   *
   * @param CultureFeed_Cdb_Item_Production $production
   *   Event where the tags will be removed from.
   * @param string $keyword
   *   Tag to remove.
   */
  public function removeTagFromProduction(CultureFeed_Cdb_Item_Production $production, $keyword) {
    $this->removeTag('production', $production->getCdbId(), $keyword);
  }

  /**
   * Remove tags from an actor.
   *
   * @param CultureFeed_Cdb_Item_Actor $actor
   *   Actor where the tags will be removed from.
   * @param string $keyword
   *   Tag to remove.
   */
  public function removeTagFromActor(CultureFeed_Cdb_Item_Actor $actor, $keyword) {
    $this->removeTag('actor', $actor->getCdbId(), $keyword);
  }

  /**
   * Withdraw translation for an event.
   *
   * @param CultureFeed_Cdb_Item_Event $event
   *   Event where the translation will be removed for.
   * @param string $lang
   *   Language of the translation to remove.
   */
  public function removeTranslationFromEvent(CultureFeed_Cdb_Item_Event $event, $lang) {
    $this->removeTranslation('event', $event->getCdbId(), $lang);
  }

  /**
   * Withdraw translation for an actor.
   *
   * @param CultureFeed_Cdb_Item_Actor $actor
   *   Actor where the translation will be removed for.
   * @param string $lang
   *   Language of the translation to remove.
   */
  public function removeTranslationFromActor(CultureFeed_Cdb_Item_Actor $actor, $lang) {
    $this->removeTranslation('actor', $actor->getCdbId(), $lang);
  }

  /**
   * Withdraw translation for an production.
   *
   * @param CultureFeed_Cdb_Item_Production $production
   *   Production where the translation will be removed for.
   * @param string $lang
   *   Language of the translation to remove.
   */
  public function removeTranslationFromProduction(CultureFeed_Cdb_Item_Production $production, $lang) {
    $this->removeTranslation('production', $production->getCdbId(), $lang);
  }

  /**
   * Withdraw link for an event.
   *
   * @param CultureFeed_Cdb_Item_Event $event
   *   Event where the translation will be removed for.
   * @param string $link
   *   Link to remove.
   */
  public function removeLinkFromEvent(CultureFeed_Cdb_Item_Event $event, $link) {
    $this->removeLink('event', $event->getCdbId(), $link);
  }

  /**
   * Withdraw link for an production.
   *
   * @param CultureFeed_Cdb_Item_Production $production
   *   Production where the link will be removed for.
   * @param string $link
   *   Link to remove.
   */
  public function removeLinkFromProduction(CultureFeed_Cdb_Item_Production $production, $link) {
    $this->removeLink('production', $production->getCdbId(), $link);
  }

  /**
   * Withdraw link for an actor.
   *
   * @param CultureFeed_Cdb_Item_Actor $actor
   *   Actor where the link will be removed for.
   * @param string $link
   *   Link to remove.
   */
  public function removeLinkFromActor(CultureFeed_Cdb_Item_Actor $actor, $link) {
    $this->removeLink('actor', $actor->getCdbId(), $link);
  }

  /**
   * Search items on the entry api.
   *
   * @param string $query
   *   Query to search.
   * @param int $page
   *   Page number to get.
   * @param int $page_length
   *   Items requested for current page.
   * @param string $sort
   *   Sort type.
   * @param string $updated_since
   *   Correct ISO date format (yyyy-m-dTH): example 2012-12-20T12:21
   *
   * @return CultureFeed_Cdb_List_Results
   */
  private function search($type, $query, $page, $page_length, $sort, $updated_since) {
  
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
      $args['pagelength'] = $page_length;
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
   *   Id from the event / actor / production to add keywords for.
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
   * Add Translation for an item.
   *
   * @param string $type
   *   Type of item to translate.
   * @param $id
   *   Id of the CultureFeed_Cdb_Item_Base (E.g. event, actor, production) to update.
   * @param String $lang
   *   Language to add.
   * @param String $title
   *   Title of the translation.
   * @param String $shortDescription
   *   Short description of the translation.
   * @param String $longDescription
   *   Long description of the translation.
   */
  private function addTranslation($type, $id, $lang, $title = '', $shortDescription = '', $longDescription = '') {
    $result = $this->oauth_client->authenticatedPostAsXml($type . '/' . $id . '/translations', array(
      'lang' => $lang,
      'title' => $title,
      'shortdescription' => $shortDescription,
      'longdescription' => $longDescription,
    ));
    $xml = $this->validateResult($result, self::CODE_TRANSLATION_CREATED);
  }

  /**
   * Remove Translation from an item.
   *
   * @param string $type
   *   Type of item to update.
   * @param string $id
   *   Id of the CultureFeed_Cdb_Item_Base to remove.
   * @param String $lang
   *   Language to add.
   */
  private function removeTranslation($type, $id, $lang) {
    $result = $this->oauth_client->authenticatedDeleteAsXml($type . '/' . $id . '/translations', array('lang' => $lang));
    $xml = $this->validateResult($result, self::CODE_TRANSLATION_WITHDRAWN);
  }
  
  /**
   * Add Link for an item.
   *
   * @param string $type
   *   Type of item to translate.
   * @param $id
   *   Id of the CultureFeed_Cdb_Item_Base (E.g. event, actor, production) to update with a link.
   * @param String $link
   *   Link itself.
   * @param String $linktype
   *   Link type.
   * @param String $lang
   *   Language to add.
   */
  private function addLink($type, $id, $link, $linktype, $lang) {
    $result = $this->oauth_client->authenticatedPostAsXml($type . '/' . $id . '/link', array(
      'link' => $link,
      'linktype' => $linktype,
      'lang' => $lang,
    ));
    $xml = $this->validateResult($result, self::CODE_LINK_CREATED);
  }

  /**
   * Remove Link from an item.
   *
   * @param string $type
   *   Type of item to update.
   * @param string $id
   *   Id of the CultureFeed_Cdb_Item_Base to remove.
   * @param String $link
   *   Link itself.
   */
  private function removeLink($type, $id, $link) {
    $result = $this->oauth_client->authenticatedDeleteAsXml($type . '/' . $id . '/link', array('link' => $link));
    $xml = $this->validateResult($result, self::CODE_LINK_WITHDRAWN);
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
   *   If no valid result status code.
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
