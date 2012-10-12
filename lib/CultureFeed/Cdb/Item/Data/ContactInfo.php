<?php

/**
 * @class
 * Representation of a contactinfo element in the cdb xml.
 */
class CultureFeed_Cdb_ContactInfo implements ICultureFeed_Cdb_Element {

  /**
   * List of addresses.
   * @var array
   */
  protected $addresses = array();

  /**
   * List of phones.
   * @var array
   */
  protected $phones = array();

  /**
   * List of mails.
   * @var array
   */
  protected $mails = array();

  /**
   * List of urls.
   * @var array
   */
  protected $urls = array();

  /**
   * Get the list of addresses.
   */
  public function getAddresses() {
    return $this->addresses;
  }

  /**
   * Get the list of phones.
   */
  public function getPhones() {
    return $this->phones;
  }

  /**
   * Get the list of mails.
   */
  public function getMails() {
    return $this->mails;
  }

  /**
   * Get the list of urls.
   */
  public function getUrls() {
    return $this->urls;
  }

  /**
   * Add an address to the address list.
   * @param CultureFeed_Cdb_Address $address
   */
  public function addAddress(CultureFeed_Cdb_Address $address) {
    $this->addresses[] = $address;
  }

  /**
   * Remove an address at a given index.
   * @param int $index
   *   Index to remove.
   * @throws Exception
   */
  public function removeAddress($index) {

    if (!isset($this->addresses[$index])) {
      throw new Exception('Trying to remove an unexisting address.');
    }

    unset($this->addresses[$index]);

  }

  /**
   * Add a phone to the phone list.
   * @param CultureFeed_Cdb_Phone $phone
   */
  public function addPhone(CultureFeed_Cdb_Phone $phone) {
    $this->phones[] = $phone;
  }

  /**
   * Remove a phone at a given index.
   * @param int $index
   *   Index to remove.
   * @throws Exception
   */
  public function removePhone($index) {

    if (!isset($this->phones[$index])) {
      throw new Exception('Trying to remove an unexisting phone.');
    }

    unset($this->phones[$index]);

  }

  /**
   * Add a mail to the mail list.
   * @param CultureFeed_Cdb_Mail $mail
   */
  public function addMail(CultureFeed_Cdb_Mail $mail) {
    $this->mails[] = $mail;
  }

  /**
   * Remove a mail at a given index.
   * @param int $index
   *   Index to remove.
   * @throws Exception
   */
  public function removeMail($index) {

    if (!isset($this->mails[$index])) {
      throw new Exception('Trying to remove an unexisting mail.');
    }

    unset($this->mails[$index]);

  }

  /**
   * Add an url to the url list.
   * @param CultureFeed_Cdb_Url $url
   */
  public function addUrl(CultureFeed_Cdb_Url $url) {
    $this->urls[] = $url;
  }

  /**
   * Remove an url at a given index.
   * @param int $index
   *   Index to remove.
   * @throws Exception
   */
  public function removeUrl($index) {

    if (!isset($this->urls[$index])) {
      throw new Exception('Trying to remove an unexisting url.');
    }

    unset($this->urls[$index]);

  }

  /**
   * @see ICultureFeed_Cdb_Element::appendToDOM()
   */
  public function appendToDOM(DOMElement $element) {

    $dom = $element->ownerDocument;

    $contactElement = $dom->createElement('contactinfo');

    foreach ($this->addresses as $address) {
      $address->appendToDom($contactElement);
    }

    foreach ($this->phones as $phone) {
      $phone->appendToDom($contactElement);
    }

    foreach ($this->mails as $mail) {
      $mail->appendToDom($contactElement);
    }

    foreach ($this->urls as $url) {
      $url->appendToDom($contactElement);
    }

    $element->appendChild($contactElement);

  }

}
