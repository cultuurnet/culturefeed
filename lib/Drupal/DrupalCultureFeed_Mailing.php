<?php

/**
 * @class
 * Drupal layer for the CultureFeed_Mailing class.
 */
class DrupalCultureFeed_Mailing extends DrupalCultureFeedBase {

  protected static $consumer_instance;
  protected static $user_instance;
  protected static $logged_in_user;

  public static function getInstance($token, $secret, $application_key = NULL, $shared_secret = NULL) {
    $endpoint = str_replace('/uitid/', '/mailing/', variable_get('culturefeed_api_location', CULTUREFEED_API_LOCATION));
    $oauth_client = self::getOAuthClient($endpoint, $token, $secret, $application_key, $shared_secret);

    $cf = new CultureFeed($oauth_client);

    if (self::isCacheEnabled()) {
      $cf = new DrupalCultureFeed_Cache($cf, self::getLoggedInUserId());
    }

    return $cf;
  }

  public static function getTemplate($id) {
    return self::getLoggedInUserInstance()->getTemplate($id);
  }

  public static function createTemplate(CultureFeed_Template $template) {
    return self::getLoggedInUserInstance()->createTemplate($template);
  }

  public static function updateTemplate(CultureFeed_Template $template, $fields = array()) {
    self::getLoggedInUserInstance()->updateTemplate($template, $fields);
  }

  public static function getTemplateList() {
    return self::getLoggedInUserInstance()->getTemplateList();
  }

  public static function deleteTemplate($id) {
    self::getLoggedInUserInstance()->deleteTemplate($id);
  }

  public static function getMailingList(CultureFeed_SearchMailingsQuery $query) {
    return self::getLoggedInUserInstance()->getMailingList($query);
  }

  public static function sendTestMailing($user_id, $mailing_id) {
    self::getLoggedInUserInstance()->sendTestMailing($user_id, $mailing_id);
  }

  public static function sendMailing($id) {
    self::getLoggedInUserInstance()->sendMailing($id);
  }

  public static function searchMailings(CultureFeed_SearchMailingsQuery $query) {
    return self::getLoggedInUserInstance()->searchMailings($query);
  }

  public static function getMailing($id) {
    return self::getLoggedInUserInstance()->getMailing($id);
  }

  public static function disableMailing($id) {
    self::getLoggedInUserInstance()->disableMailing($id);
  }
  

}
