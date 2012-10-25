<?php

/**
 * @class
 * Representation of an url element in the cdb xml.
 */
class CultureFeed_Cdb_Data_Url implements CultureFeed_Cdb_IElement {

  /**
   * Indicate if this is the main url.
   * @var bool
   */
  protected $main;

  /**
   * Indicate if this url can be used for reservations.
   * @var bool
   */
  protected $reservation;

  /**
   * The actual url.
   * @var string
   */
  protected $url;

  /**
   * Construct the url object.
   * @param string $url
   *   The url.
   * @param $bool $isMain
   *   Main address or not.
   * @param $bool $forReservations
   *   Usable for reservations or not
   */
  public function __construct($url, $isMain, $forReservations) {
    $this->url = $url;
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
   * Can the current url be used for reservations.
   * @return bool
   */
  public function isForReservations() {
    return $this->reservation;
  }

  /**
   * Get the actual url.
   */
  public function getUrl() {
    return $this->url;
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
   * Set the url.
   * @param string $url
   *   Url to set.
   */
  public function setMailAddress($url) {
    $this->url = $url;
  }

  /**
   * @see CultureFeed_Cdb_IElement::appendToDOM()
   */
  public function appendToDOM(DOMELement $element) {

    $dom = $element->ownerDocument;

    $urlElement = $dom->createElement('url', $this->url);

    if ($this->main) {
      $urlElement->setAttribute('main', 'true');
    }

    if ($this->reservation) {
      $urlElement->setAttribute('reservation', 'true');
    }

    $element->appendChild($urlElement);

  }

  /**
   * @see CultureFeed_Cdb_IElement::parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement)
   * @return CultureFeed_Cdb_Data_Url
   */
  public static function parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement) {

    $attributes = $xmlElement->attributes();
    $is_main = isset($attributes['main']) && $attributes['main'] == 'true';
    $for_reservations = isset($attributes['reservation']) && $attributes['reservation'] == 'true';

    return new CultureFeed_Cdb_Data_Url((string)$xmlElement, $is_main, $for_reservations);

  }

}
