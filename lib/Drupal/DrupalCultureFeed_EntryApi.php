<?php

/**
 * @class
 * Drupal layer for the CultureFeed_EntryApi class.
 */
class DrupalCultureFeed_EntryApi extends DrupalCultureFeedBase {

  protected static $consumer_instance;
  protected static $user_instance;
  protected static $logged_in_user;

  public static function getInstance($token, $secret, $application_key = NULL, $shared_secret = NULL) {

    $endpoint = variable_get('culturefeed_api_location', CULTUREFEED_API_LOCATION);
    $endpoint .= variable_get('culturefeed_entry_api_path', CULTUREFEED_ENTRY_API_PATH);

    $oauth_client = self::getOAuthClient($endpoint, $token, $secret, $application_key, $shared_secret);

    return new CultureFeed_EntryApi($oauth_client);

  }

  /**
   * @see CultureFeed_EntryApi::getEvent()
   */
  public static function getEvent($id) {
    return self::getLoggedInUserInstance()->getEvent($id);
  }

  /**
   * @see CultureFeed_EntryApi::getActor()
   */
  public static function getActor($id) {
    return self::getLoggedInUserInstance()->getActor($id);
  }

  /**
   * @see CultureFeed_EntryApi::getProduction()
   */
  public static function getProduction($id) {
    return self::getLoggedInUserInstance()->getProduction($id);
  }

  /**
   * @see CultureFeed_EntryApi::createEvent()
   */
  public static function createEvent(CultureFeed_Cdb_Item_Event $event) {
    return self::getLoggedInUserInstance()->createEvent($event);
  }

  /**
   * @see CultureFeed_EntryApi::createActor()
   */
  public static function createActor(CultureFeed_Cdb_Item_Actor $actor) {
    return self::getLoggedInUserInstance()->createActor($event);
  }

  /**
   * @see CultureFeed_EntryApi::createProduction()
   */
  public static function createProduction(CultureFeed_Cdb_Item_Production $production) {
    return self::getLoggedInUserInstance()->createProduction($production);
  }

  /**
   * @see CultureFeed_EntryApi::updateEvent()
   */
  public static function updateEvent(CultureFeed_Cdb_Item_Event $event) {
    self::getLoggedInUserInstance()->updateEvent($event);
  }

  /**
   * @see CultureFeed_EntryApi::updateActor()
   */
  public static function updateActor(CultureFeed_Cdb_Item_Actor $actor) {
    self::getLoggedInUserInstance()->updateActor($actor);
  }

  /**
   * @see CultureFeed_EntryApi::updateProduction()
   */
  public static function updateProduction(CultureFeed_Cdb_Item_Production $production) {
    self::getLoggedInUserInstance()->updateProduction($production);
  }

  /**
   * @see CultureFeed_EntryApi::deleteEvent()
   */
  public static function deleteEvent($id) {
    self::getLoggedInUserInstance()->deleteEvent($id);
  }

  /**
   * @see CultureFeed_EntryApi::deleteActor()
   */
  public static function deleteActor($id) {
    self::getLoggedInUserInstance()->deleteActor($id);
  }

  /**
   * @see CultureFeed_EntryApi::deleteProduction()
   */
  public static function deleteProduction($id) {
    self::getLoggedInUserInstance()->deleteProduction($id);
  }

  /**
   * @see CultureFeed_EntryApi::addTagToEvent()
   */
  public static function addTagToEvent(CultureFeed_Cdb_Item_Event $event, $keywords) {
    self::getLoggedInUserInstance()->addTagToEvent($event, $keywords);
  }

  /**
   * @see CultureFeed_EntryApi::removeTagFromEvent()
   */
  public static function removeTagFromEvent(CultureFeed_Cdb_Item_Event $event, $keyword) {
    self::getLoggedInUserInstance()->removeTagFromEvent($event, $keyword);
  }

  /**
   * @see CultureFeed_EntryApi::addLinkToEvent()
   */
  public static function addLinkToEvent(CultureFeed_Cdb_Item_Event $event, $link, $linktype = '', $lang = '') {
    self::getLoggedInUserInstance()->addLinkToEvent($event, $link, $linktype, $lang);
  }

  /**
   * @see CultureFeed_EntryApi::removeLinkFromEvent()
   */
  public static function removeLinkFromEvent(CultureFeed_Cdb_Item_Event $event, $link) {
    self::getLoggedInUserInstance()->removeLinkFromEvent($event, $link);
  }

  /**
   * @see CultureFeed_EntryApi::addTranslationToEvent()
   */
  public static function addTranslationToEvent(CultureFeed_Cdb_Item_Event $event, $lang, $title = '', $shortDescription = '', $longDescription = '') {
    self::getLoggedInUserInstance()->addTranslationToEvent($event, $lang, $title, $shortDescription, $longDescription);
  }

  /**
   * @see CultureFeed_EntryApi::removeTranslationFromEvent()
   */
  public static function removeTranslationFromEvent(CultureFeed_Cdb_Item_Event $event, $lang) {
    self::getLoggedInUserInstance()->removeTranslationFromEvent($event, $lang);
  }

  /**
   * @see CultureFeed_EntryApi::addTagToProduction()
   */
  public static function addTagToProduction(CultureFeed_Cdb_Item_Production $production, $keywords) {
    self::getLoggedInUserInstance()->addTagToProduction($production, $keywords);
  }

  /**
   * @see CultureFeed_EntryApi::removeTagFromProduction()
   */
  public static function removeTagFromProduction(CultureFeed_Cdb_Item_Production $production, $keyword) {
    self::getLoggedInUserInstance()->removeTagFromProduction($production, $keyword);
  }

  /**
   * @see CultureFeed_EntryApi::addLinkToProduction()
   */
  public static function addLinkToProduction(CultureFeed_Cdb_Item_Production $production, $link, $linktype = '', $lang = '') {
    self::getLoggedInUserInstance()->addLinkToProduction($production, $link, $linktype, $lang);
  }

  /**
   * @see CultureFeed_EntryApi::removeLinkFromProduction()
   */
  public static function removeLinkFromProduction(CultureFeed_Cdb_Item_Production $production, $link) {
    self::getLoggedInUserInstance()->removeLinkFromProduction($production, $link);
  }

  /**
   * @see CultureFeed_EntryApi::addTranslationToProduction()
   */
  public static function addTranslationToProduction(CultureFeed_Cdb_Item_Production $production, $lang, $title = '', $shortDescription = '', $longDescription = '') {
    self::getLoggedInUserInstance()->addTranslationToProduction($production, $lang, $title, $shortDescription, $longDescription);
  }

  /**
   * @see CultureFeed_EntryApi::removeTranslationFromProduction()
   */
  public static function removeTranslationFromProduction(CultureFeed_Cdb_Item_Production $production, $lang) {
    self::getLoggedInUserInstance()->removeTranslationFromProduction($production, $lang);
  }

  /**
   * @see CultureFeed_EntryApi::addTagToActor()
   */
  public static function addTagToActor(CultureFeed_Cdb_Item_Actor $actor, $keywords) {
    self::getLoggedInUserInstance()->addTagToActor($actor, $keywords);
  }

  /**
   * @see CultureFeed_EntryApi::removeTagFromActor()
   */
  public static function removeTagFromActor(CultureFeed_Cdb_Item_Actor $actor, $keyword) {
    self::getLoggedInUserInstance()->removeTagFromActor($actor, $keyword);
  }

  /**
   * @see CultureFeed_EntryApi::addLinkToActor()
   */
  public static function addLinkToActor(CultureFeed_Cdb_Item_Actor $actor, $link, $linktype = '', $lang = '') {
    self::getLoggedInUserInstance()->addLinkToActor($actor, $link, $linktype, $lang);
  }

  /**
   * @see CultureFeed_EntryApi::removeLinkFromActor()
   */
  public static function removeLinkFromActor(CultureFeed_Cdb_Item_Actor $actor, $link) {
    self::getLoggedInUserInstance()->removeLinkFromActor($actor, $link);
  }

  /**
   * @see CultureFeed_EntryApi::addTranslationToActor()
   */
  public static function addTranslationToActor(CultureFeed_Cdb_Item_Actor $actor, $lang, $title = '', $shortDescription = '', $longDescription = '') {
    self::getLoggedInUserInstance()->addTranslationToActor($actor, $lang, $title, $shortDescription, $longDescription);
  }

  /**
   * @see CultureFeed_EntryApi::removeTranslationFromActor()
   */
  public static function removeTranslationFromActor(CultureFeed_Cdb_Item_Actor $actor, $lang) {
    self::getLoggedInUserInstance()->removeTranslationFromActor($actor, $lang);
  }

}