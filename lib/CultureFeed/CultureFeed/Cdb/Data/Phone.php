<?php

/**
 * @class
 * Representation of a phone element in the cdb xml.
 */
class CultureFeed_Cdb_Data_Phone implements CultureFeed_Cdb_IElement {

  /**
   * Current phone is a phone.
   */
  const PHONE_TYPE_PHONE = 'phone';

  /**
   * Current phone is a fax.
   */
  const PHONE_TYPE_FAX = 'fax';

  /**
   * Indicate if this is the main phone.
   * @var bool
   */
  protected $main;

  /**
   * Indicate if this phone can be used for reservations.
   * @var bool
   */
  protected $reservation;

  /**
   * Type of phone.
   * @var string
   */
  protected $type = self::PHONE_TYPE_PHONE;

  /**
   * The actual phone number.
   * @var string
   */
  protected $number;

  /**
   * Construct the phone object.
   * @param string $number
   *   The phone number address.
   * @param string $type
   *   The type of phone.
   * @param $bool $isMain
   *   Main phone or not.
   * @param $bool $forReservations
   *   Usable for reservations or not
   */
  public function __construct($number, $type, $isMain, $forReservations) {
    $this->number = $number;
    $this->type = $type;
    $this->main = $isMain;
    $this->reservation = $forReservations;
  }

  /**
   * Is the current phone the main phone, or not
   * @return bool
   */
  public function isMainPhone() {
    return $this->main;
  }

  /**
   * Can the current phone be used for reservations.
   * @return bool
   */
  public function isForReservations() {
    return $this->reservation;
  }

  /**
   * Get the type of phone.
   */
  public function getType() {
    return $this->type;
  }

  /**
   * Get the phone number.
   */
  public function getNumber() {
    return $this->number;
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
   * Set the type of phone.
   * @param string $type
   *   Type of phone to set.
   */
  public function setType($type) {
    $this->type = $type;
  }

  /**
   * Set the phone number.
   * @param string $number
   *   Phone number to set.
   */
  public function setNumber($number) {
    $this->number = $number;
  }

  /**
   * @see CultureFeed_Cdb_IElement::appendToDOM()
   */
  public function appendToDOM(DOMELement $element) {

    $dom = $element->ownerDocument;

    $phoneElement = $dom->createElement('phone', $this->number);
    $phoneElement->setAttribute('type', $this->type);

    if ($this->main) {
      $phoneElement->setAttribute('main', 'true');
    }

    if ($this->reservation) {
      $phoneElement->setAttribute('reservation', 'true');
    }

    $element->appendChild($phoneElement);

  }

  /**
   * @see CultureFeed_Cdb_IElement::parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement)
   * @return CultureFeed_Cdb_Data_Phone
   */
  public static function parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement) {

    $attributes = $xmlElement->attributes();
    $is_main = isset($attributes['main']) && $attributes['main'] == 'true';
    $for_reservations = isset($attributes['reservation']) && $attributes['reservation'] == 'true';

    return new CultureFeed_Cdb_Data_Phone((string)$xmlElement, (string)$attributes['type'], $is_main, $for_reservations);

  }

}
