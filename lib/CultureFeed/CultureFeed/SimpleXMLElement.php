<?php

/**
 * Class that extends SimpleXMLElement to add some parsing helpers.
 */
class CultureFeed_SimpleXMLElement extends SimpleXMLElement {

  /**
   * Runs XPath query on XML data and casts it to a string or an array of string values.
   * @see xpath_cast for more documentation on the arguments.
   */
  public function xpath_str($path, $multiple = FALSE, $trim = TRUE) {
    $tmp = $this->xpath_cast('strval', $path, $multiple);
    if ($tmp && !$multiple && $trim) {
      return trim($tmp);
    }
    return $tmp;
  }

  /**
   * Runs XPath query on XML data and casts it to an integer or an array of integer values.
   * @see xpath_cast for more documentation on the arguments.
   */
  public function xpath_int($path, $multiple = FALSE) {
    return $this->xpath_cast('intval', $path, $multiple);
  }

  /**
   * Runs XPath query on XML data and casts it to a float or an array of float values.
   * @see xpath_cast for more documentation on the arguments.
   */
  public function xpath_float($path, $multiple = FALSE) {
    return $this->xpath_cast('floatval', $path, $multiple);
  }

  /**
   * Runs XPath query on XML data and casts it to a UNIX timestamp or an array of timestamps.
   * @see xpath_cast for more documentation on the arguments.
   */
  public function xpath_time($path, $multiple = FALSE) {
    $val = $this->xpath_cast('strval', $path, $multiple);
    if (!$val) {
      return NULL;
    }

    if ($multiple) {
      foreach ($val as $key => $value) {
        $val[$key] = strtotime($value);
      }
    }

    return strtotime($val);
  }

  /**
   * Runs XPath query on XML data and casts it to a bool or an array of bool values.
   * @see xpath_cast for more documentation on the arguments.
   */
  public function xpath_bool($path, $multiple = FALSE) {
    $val = $this->xpath_cast('strval', $path, $multiple);
    if (!$val) {
      return NULL;
    }

    if ($multiple) {
      foreach ($val as $key => $value) {
        $val[$key] = strtolower($value) == 'true' ? TRUE : FALSE;
      }
    }

    return strtolower($val) == 'true' ? TRUE : FALSE;
  }

  /**
   * Runs XPath query on XML data and casts it using a type casting function.
   *
   * @param string $cast_function
   * @param string $path
   *   The XPath query.
   * @param string $multiple
   *   Does the query direct to a path where multiple values are possible?
   * @return array|undefined
   *   In case $multiple is TRUE, an array is returned with the type casted elements.
   *   In case $multiple is FALSE, the result of the XPath query is casted using the $cast_function and type depends on type of that function.
   *   In case no nodes were found with the query, NULL is returned.
   */
  private function xpath_cast($cast_function, $path, $multiple = FALSE) {
    $objects = $this->xpath($path);

    if (!is_array($objects)) return $objects;

    if ($multiple) {
      $result = array();
      foreach ($objects as $object) {
        $result[] = is_null($object) || ($cast_function != 'strval' && empty($object)) ? NULL : call_user_func($cast_function, $object);
      }
      return array_filter($result);
    }
    else {
      return empty($objects) || is_null($objects[0]) || ($cast_function != 'strval' && empty($objects[0])) ? NULL : call_user_func($cast_function, $objects[0]);
    }
  }

  /**
   * Parse the SimpleXML element as a CultureFeed_User.
   *
   * @return CultureFeed_User
   */
  public function parseUser() {
    $user = new CultureFeed_User();

    $user->id           = $this->xpath_str('/foaf:person/rdf:id');
    $user->nick         = $this->xpath_str('/foaf:person/foaf:nick');
    $user->givenName    = $this->xpath_str('/foaf:person/foaf:givenName');
    $user->familyName   = $this->xpath_str('/foaf:person/foaf:familyName');
    $user->mbox         = $this->xpath_str('/foaf:person/foaf:mbox');
    $user->mboxVerified = $this->xpath_bool('/foaf:person/mboxVerified');
    $user->gender       = $this->xpath_str('/foaf:person/foaf:gender');
    $user->dob          = $this->xpath_time('/foaf:person/foaf:dob');
    $user->depiction    = $this->xpath_str('/foaf:person/foaf:depiction');
    $user->bio          = $this->xpath_str('/foaf:person/bio');
    $user->homeAddress  = $this->xpath_str('/foaf:person/foaf:homeAddress');
    $user->status       = $this->xpath_str('/foaf:person/status');
    if ($user->status) {
      $user->status = strtolower($user->status);
    }
    $user->openid       = $this->xpath_str('/foaf:person/foaf:openid');

    $lat = $this->xpath_float('/foaf:person/homeLocation/geo:lat');
    $lng = $this->xpath_float('/foaf:person/homeLocation/geo:long');

    if ($lat && $lng) {
      $user->homeLocation = new CultureFeed_Location($lat, $lng);
    }

    $lat = $this->xpath_float('/foaf:person/currentLocation/geo:lat');
    $lng = $this->xpath_float('/foaf:person/currentLocation/geo:long');

    if ($lat && $lng) {
      $user->currentLocation = new CultureFeed_Location($lat, $lng);
    }

    $accounts = array();

    $objects = $this->xpath('/foaf:person/foaf:holdsAccount/foaf:onlineAccount');

    foreach ($objects as $object) {
      $account = new CultureFeed_OnlineAccount();

      $account->accountType            = $object->xpath_str('accountType');
      $account->accountServiceHomepage = $object->xpath_str('foaf:accountServiceHomepage');
      $account->accountName            = $object->xpath_str('foaf:accountName');
      $account->private                = $object->xpath_bool('private');
      $account->publishActivities      = $object->xpath_bool('publishActivities');

      $accounts[] = $account;
    }

    if ($this->xpath_str('/foaf:person/privateNick') !== NULL) {
      $privacy_config = new CultureFeed_UserPrivacyConfig();

      $vars = array('nick', 'givenName', 'familyName', 'mbox', 'gender', 'dob', 'depiction', 'bio', 'homeAddress', 'homeLocation', 'currentLocation', 'openId');

      foreach ($vars as $var) {
        $privacy = $this->xpath_bool('/foaf:person/private' . ucfirst($var));

        if (is_bool($privacy)) {
          $privacy_config->{$var} = $privacy ? CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE : CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;
        }
      }

      $user->privacyConfig = $privacy_config;
    }

    if (!empty($accounts)) {
      $user->holdsAccount = $accounts;
    }

    return $user;
  }

  /**
   * Parse the SimpleXML element as a CultureFeed_ResultSet.
   *
   * @return CultureFeed_ResultSet
   *   CultureFeed_ResultSet where the objects are of the CultureFeed_User type.
   */
  public function parseUsers() {
    $total = $this->xpath_int('/response/total');

    $users = array();

    $objects = $this->xpath('/response/users/user');

    foreach ($objects as $object) {
      $user = new CultureFeed_SearchUser();

      $user->id        = $object->xpath_str('rdf:id');
      $user->nick      = $object->xpath_str('foaf:nick');
      $user->depiction = $object->xpath_str('foaf:depiction');
      $user->sortValue = $object->xpath_int('sortValue');

      $users[] = $user;
    }

    return new CultureFeed_ResultSet($total, $users);
  }

  /**
   * Parse the SimpleXML element as an array of CultureFeed_Consumer objects.
   *
   * @return CultureFeed_Consumer[]
   *   Array of CultureFeed_Consumer objcts.
   */
  public function parseServiceConsumers() {
    $consumers = array();

    $objects = $this->xpath('/response/serviceconsumers/serviceconsumer');

    foreach ($objects as $object) {
      $consumer = new CultureFeed_Consumer();

      $consumer->consumerKey  = $object->xpath_str('consumerKey');
      $consumer->creationDate = $object->xpath_time('creationDate');
      $consumer->id           = $object->xpath_int('id');
      $consumer->name         = $object->xpath_str('name');
      $consumer->organization = $object->xpath_str('organization');
      $consumer->description  = $object->xpath_str('description');
      $consumer->logo         = $object->xpath_str('logo');

      $consumers[] = $consumer;
    }

    return $consumers;
  }

  /**
   * Parse the SimpleXML element as a CultureFeed_ResultSet.
   *
   * @return CultureFeed_ResultSet
   *   CultureFeed_ResultSet where the objects are of the CultureFeed_Activity type.
   */
  public function parseActivities() {
    $type_mapping = array(
      'VIEW'     => CultureFeed_Activity::TYPE_VIEW,
      'DETAIL'   => CultureFeed_Activity::TYPE_DETAIL,
      'LIKE'     => CultureFeed_Activity::TYPE_LIKE,
      'MAIL'     => CultureFeed_Activity::TYPE_MAIL,
      'PRINT'    => CultureFeed_Activity::TYPE_PRINT,
      'FACEBOOK' => CultureFeed_Activity::TYPE_FACEBOOK,
      'TWITTER'  => CultureFeed_Activity::TYPE_TWITTER,
      'IK_GA'    => CultureFeed_Activity::TYPE_IK_GA,
    );

    $total = $this->xpath_int('/response/total');

    $activities = array();

    $objects = $this->xpath('/response/activities/activity');

    foreach ($objects as $object) {
      $activity = new CultureFeed_Activity();

      $activity->nodeId       = $object->xpath_str('nodeID');
      $activity->private      = $object->xpath_str('private');
      $activity->createdVia   = $object->xpath_str('createdVia');
      $activity->points       = $object->xpath_str('points');
      $activity->contentType  = $object->xpath_str('contentType');
      $activity->type         = isset($type_mapping[$object->xpath_str('type')]) ? $type_mapping[$object->xpath_str('type')] : $object->xpath_str('type');
      $activity->value        = $object->xpath_str('value');
      $activity->userId       = $object->xpath_str('userId');
      $activity->depiction    = $object->xpath_str('depiction');
      $activity->nick         = $object->xpath_str('nick');
      $activity->creationDate = $object->xpath_time('creationDate');

      $activities[] = $activity;
    }

    return new CultureFeed_ResultSet($total, $activities);
  }

  /**
   * Parse the SimpleXML element as an array of CultureFeed_Consumer objects.
   *
   * @return CultureFeed_Recommendation[]
   *   Array of CultureFeed_Recommendation objcts.
   */
  public function parseRecommendations() {
    $recommendations = array();

    $objects = $this->xpath('/response/recommendations/recommendation');

    foreach ($objects as $object) {
      $recommendation = new CultureFeed_Recommendation();

      $recommendation->id           = $object->xpath_str('id');
      $recommendation->itemid       = $object->xpath_str('itemid');
      $recommendation->score        = $object->xpath_float('score');
      $recommendation->algorithm    = $object->xpath_str('algorithm');
      $recommendation->creationDate = $object->xpath_time('creationDate');

      $recommendation_item = new CultureFeed_RecommendationItem();

      $recommendation_item->id                = $object->xpath_str('item/id');
      $recommendation_item->permalink         = $object->xpath_str('item/permalink');
      $recommendation_item->title             = $object->xpath_str('item/title');
      $recommendation_item->description_short = $object->xpath_str('item/description_short');
      $recommendation_item->from              = $object->xpath_time('item/from');
      $recommendation_item->to                = $object->xpath_time('item/to');
      $recommendation_item->location_simple   = $object->xpath_str('item/location_simple');

      $coord = $object->xpath_str('item/location_latlong');
      if ($coord) {
        list($lat, $lng) = explode(',', $coord);
        $recommendation_item->location_latlong = new CultureFeed_Location((float)$lat, (float)$lng);
      }

      $recommendation->recommendationItem = $recommendation_item;

      $recommendations[] = $recommendation;
    }

    return $recommendations;
  }

}