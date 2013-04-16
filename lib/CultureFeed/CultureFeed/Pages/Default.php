<?php

/**
 * @class
 * Contains all methods for pages management.
 */
class CultureFeed_Pages_Default implements CultureFeed_Pages {

  /**
   * Status code when an action was succeeded
   * @var string
   */
  const CODE_ACTION_SUCCEEDED = 'ACTION_SUCCEEDED';

  /**
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

  public function __construct(ICultureFeed $culturefeed) {
    $this->culturefeed = $culturefeed;
    $this->oauth_client = $culturefeed->getClient();
  }

  /**
   * (non-PHPdoc)
   * @see CultureFeed_Pages::getPage()
   */
  public function getPage($id) {

    $result = $this->oauth_client->consumerGetAsXml('page/' . $id);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    return CultureFeed_Cdb_Item_Page::parseFromCdbXml($xml);

  }

  /**
   * (non-PHPdoc)
   * @see CultureFeed_Pages::getUserList()
   */
  public function getUserList($id, $roles = array(), $use_auth = TRUE) {

    $query = array();
    if (!empty($roles)) {
      $query['roles'] = $roles;
    }

    if ($use_auth) {
      $result = $this->oauth_client->authenticatedGetAsXml('page/' . $id . '/user/list', $query);
    }
    else {
      $result = $this->oauth_client->consumerGetAsXml('page/' . $id . '/user/list', $query);
    }

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $userList = new CultureFeed_Pages_UserList();

    $memberships = $xml->xpath('/response/pageMemberships/pageMembership');
    foreach ($memberships as $object) {

      $membership = new CultureFeed_Pages_Membership();

      $account = new CultureFeed_User();
      $account->id          = $object->xpath_str('user/rdf:id');
      $account->nick        = $object->xpath_str('user/foaf:nick');
      $account->depiction   = $object->xpath_str('user/foaf:depiction');
      $membership->user     = $account;

      $membership->role          = $object->xpath_str('pageRole');
      $membership->creationDate  = $object->xpath_time('creationDate');

      $userList->memberships[] = $membership;

    }

    $followers = $xml->xpath('/response/followers/follower');
    foreach ($followers as $object) {

      $follower = new CultureFeed_Pages_Follower();
      $follower->userId        = $object->xpath_str('rdf:id');
      $follower->userName      = $object->xpath_str('foaf:nick');
      $follower->picture       = $object->xpath_str('foaf:depiction');

      $userList->followers[] = $follower;

    }

    return $userList;


  }

  /**
   * (non-PHPdoc)
   * @see CultureFeed_Pages::addMember()
   */
  public function addMember($id, $userId, $relation = CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_MEMBER, $activityPrivate = TRUE) {

    $data = array(
      'userId' => $userId,
      'relation' => $relation,
      'activityPrivate' => $activityPrivate,
    );

    $result = $this->oauth_client->authenticatedPostAsXml('page/' . $id . '/member/add', $data);

    $this->validateResult($result, CultureFeed_Pages_Default::CODE_ACTION_SUCCEEDED);

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

    $status_code = $xml->xpath_str('code');
    if ($status_code == $valid_status_code) {
      return $xml;
    }

    throw new CultureFeed_InvalidCodeException($xml->xpath_str('/rsp/message'), $status_code);

  }

}
