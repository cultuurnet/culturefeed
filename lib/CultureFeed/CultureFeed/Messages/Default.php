<?php

/**
 * @class
 * Contains all methods for sending / receiving messages.
 */
class CultureFeed_Messages_Default implements CultureFeed_Messages {

  /**
   * Status code when the call was succesfull
   * @var string
   */
  const CODE_SUCCESS = 'SUCCESS';

  /**
   * Type key for the count of messages that the user has read.
   * @var string
   */
  const MESSAGE_COUNT_READ = 'READ';

  /**
   * Type key for they count of unread messages.
   * @var string
   */
  const MESSAGE_COUNT_UNREAD = 'UNREAD';

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
   * @see CultureFee_Messages::getMessageCount().
   */
  public function getMessageCount() {

    $result = $this->oauth_client->authenticatedGetAsXml('message/totals');

    try {
      $xmlElement = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $message_count = array();
    $total = $xmlElement->xpath('/response/total');
    if (!$total) {
      return array();
    }

    foreach ($total as $count) {
      $attributes = $count->attributes();
      $message_count[(string)$attributes['type']] = (string) $count;
    }

    return $message_count;

  }

  /**
   * @see CultureFeed_Messages::getMessages()
   */
  public function getMessages($recipientPage = NULL, $type = NULL) {

    $params = array();
    if (!empty($recipientPage)) {
      $params['recipientPage'] = $recipientPage;
    }

    if (!empty($type)) {
      $params['type'] = $type;
    }

    $result = $this->oauth_client->authenticatedGetAsXml('message/list', $params);
    $xmlElement = $this->validateResult($result, CultureFeed_Messages_Default::CODE_SUCCESS);

    $messages = array();
    $messageElements = $xmlElement->xpath('/response/messages/message');

    foreach ($messageElements as $element) {
      $messages[] = CultureFeed_Messages_Message::parseFromXml($element);
    }

    return new CultureFeed_ResultSet(count($messages), $messages);

  }

  /**
   * @see CultureFeed_Messages::getMessage()
   */
  public function getMessage($id) {

    $result = $this->oauth_client->authenticatedGetAsXml('message/' . $id);
    $xmlElement = $this->validateResult($result, CultureFeed_Messages_Default::CODE_SUCCESS);

    $threadElement = $xmlElement->xpath('/response/thread');

    return CultureFeed_Messages_Message::parseFromXml($threadElement[0]);

  }

  /**
   * @see CultureFeed_Messages::sendMessages()
   */
  public function sendMessage($params) {

    $result = $this->oauth_client->authenticatedPostAsXml('message', $params);
    $xmlElement = $this->validateResult($result, CultureFeed_Messages_Default::CODE_SUCCESS);

    return $xmlElement->xpath_str('id');

  }

  /**
   * @see CultureFeed_Messages::deleteMessage()
   */
  public function deleteMessage($id) {

    $result = $this->oauth_client->authenticatedPostAsXml('message/' . $id . '/delete');
    $xmlElement = $this->validateResult($result, CultureFeed_Messages_Default::CODE_SUCCESS);

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
      $xmlElement = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    $status_code = $xmlElement->xpath_str($status_xml_tag);

    if ($status_code == $valid_status_code) {
      return $xmlElement;
    }

    throw new CultureFeed_InvalidCodeException($xmlElement->xpath_str('message'), $status_code);

  }

}
