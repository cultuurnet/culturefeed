<?php

/**
 * Class to represent a message.
 */
class CultureFeed_Messages_Message {

  /**
   * Message type key when the message is send to a page.
   * @var string
   */
  const TYPE_MEMBERS = 'contactmembers';

  /**
   * Message ID.
   * @var string
   */
  public $id;

  /**
   * Sender.
   * @var CultureFeed_User
   */
  public $sender;

  /**
   * Message type
   * @var string
   */
  public $type;

  /**
   * Timestamp of the creation date.
   * @var string
   */
  public $creationDate;

  /**
   * Timestamp of the last reply date.
   * @var unknown
   */
  public $lastReply;

  /**
   * Status of this message (NEW or READ)
   * @var string
   */
  public $status;

  /**
   * Sender page if the page was send on a page.
   * @var CultureFeed_Cdb_Item_Page
   */
  public $senderPage;

  /**
   * Recipient page, if the message was sent to a page.
   * @var CultureFeed_Cdb_Item_Page
   */
  public $recipientPage;

  /**
   * Page role where the message is sent to (ADMIN / MEMBER / FOLLOWER)
   * @var string
   */
  public $role;

  /**
   * Subject of the message.
   * @var string
   */
  public $subject;

  /**
   * Body of the message
   * @var string
   */
  public $body;

  /**
   * If this message is a thread, it will also have child messages.
   * @var CultureFeed_Messages_Message[]
   */
  public $children;

  /**
   * Parse a message from an xml element.
   * @param unknown $xmlElement
   */
  public static function parseFromXml(CultureFeed_SimpleXMLElement $xmlElement) {

    $message = new self();

    $message->id           = $xmlElement->xpath_str('id');
    $message->type         = $xmlElement->xpath_str('type');
    $message->status       = $xmlElement->xpath_str('status');
    $message->creationDate = $xmlElement->xpath_time('creationDate');
    $message->lastReply    = $xmlElement->xpath_time('lastReply');
    $message->subject      = $xmlElement->xpath_str('subject');
    $message->body         = $xmlElement->xpath_str('body');
    $message->role         = $xmlElement->xpath_str('role');

    $user = new CultureFeed_User();
    $user->id        = $xmlElement->xpath_str('sender/rdf:id');
    $user->nick      = $xmlElement->xpath_str('sender/foaf:nick');
    $user->depiction = $xmlElement->xpath_str('sender/foaf:depiction');
    $message->sender = $user;

    $senderPageId = $xmlElement->xpath_str('senderPageId');
    if ($senderPageId) {
      $page = new CultureFeed_Cdb_Item_Page();
      $page->setId($senderPageId);
      $page->setName($xmlElement->xpath_str('senderPageName'));
      $message->senderPage = $page;
    }

    $recipientPageId = $xmlElement->xpath_str('recipientPageId');
    if ($recipientPageId) {
      $page = new CultureFeed_Cdb_Item_Page();
      $page->setId($recipientPageId);
      $page->setName($xmlElement->xpath_str('recipientPageName'));
      $message->recipientPage = $page;
    }

    // If this is a thread, parse also the child messages.
    $message->children = array();
    $childElements = $xmlElement->xpath('children/message');
    if ($childElements) {
      foreach ($childElements as $childElement) {
        $message->children[] = CultureFeed_Messages_Message::parseFromXml($childElement);
      }
    }

    return $message;

  }

}

