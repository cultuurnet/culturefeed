<?php

/**
 * @class
 * Representation of a mail element in the cdb xml.
 */
class CultureFeed_Cdb_Data_Mail implements CultureFeed_Cdb_IElement {

  /**
   * Indicate if this is the main mail.
   * @var bool
   */
  protected $main;

  /**
   * Indicate if this mail can be used for reservations.
   * @var bool
   */
  protected $reservation;

  /**
   * Mail address.
   * @var string
   */
  protected $address;

  /**
   * Construct the mail object.
   * @param string $address
   *   The Mail address.
   * @param $bool $isMain
   *   Main address or not.
   * @param $bool $forReservations
   *   Usable for reservations or not
   */
  public function __construct($address, $isMain, $forReservations) {
    $this->address = $address;
    $this->main = $isMain;
    $this->reservation = $forReservations;
  }

  /**
   * Is the current mail the main mail, or not
   * @return bool
   */
  public function isMainMail() {
    return $this->main;
  }

  /**
   * Can the current mail be used for reservations.
   * @return bool
   */
  public function isForReservations() {
    return $this->reservation;
  }

  /**
   * Get the mail address.
   */
  public function getMailAddress() {
    return $this->address;
  }

  /**
   * Set the main state.
   * @param bool $isMain
   *   State to set.
   */
  public function setMain($isMain) {
    $this->main = $isMain;
  }

  /**
   * Set the reservation state.
   * @param bool $forReservation
   *   State to set.
   */
  public function setReservation($forReservation) {
    $this->reservation = $forReservation;
  }

  /**
   * Set the mail address.
   * @param string $address
   *   Address to set.
   */
  public function setMailAddress($address) {
    $this->address = $address;
  }

  /**
   * @see CultureFeed_Cdb_IElement::appendToDOM()
   */
  public function appendToDOM(DOMELement $element) {

    $dom = $element->ownerDocument;

    $mailElement = $dom->createElement('mail', $this->address);

    if ($this->main) {
      $mailElement->setAttribute('main', 'true');
    }

    if ($this->reservation) {
      $mailElement->setAttribute('reservation', 'true');
    }

    $element->appendChild($mailElement);

  }

  /**
   * @see CultureFeed_Cdb_IElement::parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement)
   * @return CultureFeed_Cdb_Data_Mail
   */
  public static function parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement) {

    $attributes = $xmlElement->attributes();
    $is_main = isset($attributes['main']) && $attributes['main'] == 'true';
    $for_reservations = isset($attributes['reservation']) && $attributes['reservation'] == 'true';

    return new CultureFeed_Cdb_Data_Mail((string)$xmlElement, $is_main, $for_reservations);

  }

}
