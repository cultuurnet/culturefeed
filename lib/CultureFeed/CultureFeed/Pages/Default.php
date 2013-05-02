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
   * Status code when a page was successfully created
   * Invalid codes: [MISSING_REQUIRED_FIELDS, UNKNOWN_CATEGORY]
   * @var string
   */
  const CODE_PAGE_CREATED = 'PAGE_CREATED';

  /**
   * Status code when an image upload was successful
   * Invalid codes: [ACTION_FAILED]
   * @var string
   */
  const IMAGE_UPLOADED = 'IMAGE_UPLOADED';
  
  /**
   * Status code when a page was successfully updated.
   * Invalid codes: [ACCESS_DENIED, MISSING_REQUIRED_FIELDS, UNKNOWN_CATEGORY]
   * @var string
   */
  const PAGE_MODIFIED = 'PAGE_MODIFIED';

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
   * @see CultureFeed_Pages::addPage()
   */
  public function addPage(array $params) {
    
    foreach ($params as $key => $value) {
      if (is_string($value) && $value === "") {
        unset($params[$key]);
      }
    }
    
    $result = $this->oauth_client->authenticatedPost('page', $params);
    $xmlElement = $this->validateResult($result, CultureFeed_Pages_Default::CODE_PAGE_CREATED);
    
    return $xmlElement->xpath_str('uid');
  
  }

  /**
   * (non-PHPdoc)
   * @see CultureFeed_Pages::updatePage()
   */
  public function updatePage($id, array $params) {
    
    foreach ($params as $key => $value) {
      if (is_string($value) && $value === "") {
        unset($params[$key]);
      }
    }
    
    $result = $this->oauth_client->authenticatedPost('page/' . $id, $params);
    $xmlElement = $this->validateResult($result, CultureFeed_Pages_Default::PAGE_MODIFIED);
    
    return $xmlElement->xpath_str('uid');
  
  }

  /**
   * (non-PHPdoc)
   * @see CultureFeed_Pages::removePage()
   */
  public function removePage($id) {
    return $this->updatePage($id, array('visible' => "false"));
  }

  /**
   * (non-PHPdoc)
   * @see CultureFeed_Pages::publishPage()
   */
  public function publishPage($id) {
    return $this->updatePage($id, array('visible' => "true"));
  }
  
  /**
   * Implements CultureFeed_Pages::addImage().
   * @see CultureFeed_Pages::addImage()
   */
  public function addImage($id, array $params) {

    $result = $this->oauth_client->authenticatedPostAsXml('page/' . $id . '/uploadImage', $params, TRUE, TRUE);
    $xmlElement = $this->validateResult($result, CultureFeed_Pages_Default::IMAGE_UPLOADED);
    
    return $xmlElement->xpath_str('uid');
  }
  
  /**
   * Implements CultureFeed_Pages::changePermissions()
   * @see CultureFeed_Pages::changePermissions()
   */ 
  public function changePermissions($id, array $params) {

    $result = $this->oauth_client->authenticatedPostAsXml('page/' . $id . '/permissions', $params, TRUE, FALSE);
    $xmlElement = $this->validateResult($result, $id, 'uid');
    
    return $xmlElement->xpath_str('uid');
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
   * @param string $status_xml_tag
   *   Xml tag where the status code can be checked.
   * @return CultureFeed_SimpleXMLElement The parsed xml.
   *
   * @throws CultureFeed_ParseException
   *   If the result could not be parsed.
   * @throws CultureFeed_InvalidCodeException
   *   If no valid result status code.
   */
  private function validateResult($result, $valid_status_code, $status_xml_tag = 'code') {
    
    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $status_code = $xml->xpath_str($status_xml_tag);
    
    if ($status_code == $valid_status_code) {
      return $xml;
    }

    throw new CultureFeed_InvalidCodeException($xml->xpath_str('message'), $status_code);

  }

}
