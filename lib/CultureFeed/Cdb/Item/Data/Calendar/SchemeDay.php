<?php

/**
 * @class
 * Representation of a scheme day element in the cdb xml.
 */
class CultureFeed_Cdb_Calendar_SchemeDay implements ICultureFeed_Cdb_Element {

  /**
   * Name of the week day
   * @var string
   */
  protected $dayName;

  /**
   * Open type for the day.
   * @var string
   */
  protected $openType;

  /**
   * Openingtime: from
   * @var string
   */
  protected $openFrom;

  /**
   * Openingtime: till
   * @var string
   */
  protected $openTill;

  /**
   * Construct the scheme day.
   * @param string $dayName
   *   Name of the day.
   * @param string $openType
   *   Open type for the day.
   */
  public function __construct($dayName, $openType) {

    $this->dayName = $dayName;
    $this->openType = $openType;

  }

  /**
   * Get the name from the day.
   */
  public function getDayName() {
    return $this->dayName;
  }

  /**
   * Get the opening type for this day.
   */
  public function getOpenType() {
    return $this->openType;
  }

  /**
   * Get the opening from time.
   */
  public function getOpenFrom() {
    return $this->openFrom;
  }

  /**
   * Get the opening from time.
   */
  public function getOpenTill() {
    return $this->openTill;
  }

  /**
   * Set the day name.
   * @param string $dayName
   *   Name of the day to set.
   */
  public function setDayName($dayName) {
    $this->dayName = $dayName;
  }

  /**
   * Set the opening type.
   * @param string $type
   *   Opening type to set.
   */
  public function setOpenType($type) {
    $this->type = $type;
  }

  /**
   * Set the opening from time.
   * @param string $openFrom
   *   Opening time to set.
   */
  public function setOpenFrom($openFrom) {
    CultureFeed_Cdb_Calendar::validateTime($openFrom);
    $this->openFrom = $openFrom;
  }

  /**
   * Set the open till time.
   * @param string $openTill
   *   Open till time to set.
   */
  public function setOpenTill($openTill) {
    CultureFeed_Cdb_Calendar::validateTime($openTill);
    $this->openTill = $openTill;
  }

  /**
   * @see ICultureFeed_Cdb_Element::appendToDOM()
   */
  public function appendToDOM(DOMELement $element) {

    $dom = $element->ownerDocument;

    $dayElement = $dom->createElement($this->dayName);
    if ($this->openType) {
      $dayElement->setAttribute('opentype', $this->openType);
    }

    if ($this->openFrom || $this->openTill) {
      $openingElement = $dom->createElement('openingtime');
      if ($this->openFrom) {
        $openingElement->setAttribute('from', $this->openFrom);
        $openingElement->setAttribute('to', $this->openTill);
      }
      $dayElement->appendChild($openingElement);
    }

    $element->appendChild($dayElement);

  }

}
