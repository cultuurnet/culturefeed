<?php

/**
 * @class
 * Representation of a timestamp element in the cdb xml.
 */
class CultureFeed_Cdb_Calendar_Timestamp implements ICultureFeed_Cdb_Element {

  /**
   * Date from the timestamp.
   * @var string
   */
  protected $date;

  /**
   * Start time from the timestamp.
   * @var string
   */
  protected $startTime;

  /**
   * End time from the timestamp.
   * @var string
   */
  protected $endTime;

  /**
   * Open type for the timestamp.
   * @var string
   */
  protected $openType;

  /**
   * Construct a new calendar timestamp.
   * @param string $date
   *   Date from the timestamp.
   * @param string $startTime
   *   Start time from the timestamp.
   * @param string $endTime
   *   End time from the timestamp.
   */
  public function __construct($date, $startTime = NULL, $endTime = NULL) {

    $this->setDate($date);

    if ($startTime !== NULL) {
      $this->setStartTime($starTime);
    }

    if ($endTime !== NULL) {
      $this->setEndTime($endTime);
    }

  }

  /**
   * Get the date.
   */
  public function getDate() {
    return $this->endDate;
  }

  /**
   * Get the start time.
   */
  public function getStartTime() {
    return $this->startTime;
  }

  /**
   * Get the end time.
   */
  public function getEndTime() {
    return $this->endTime;
  }

  /**
   * Get the open type.
   */
  public function getOpenType() {
    return $this->openType;
  }

  /**
   * Set the date from the timestamp.
   * @param string $date
   *   Date to set.
   */
  public function setDate($date) {
    CultureFeed_Cdb_Calendar::validateDate($date);
    $this->date = $date;
  }

  /**
   * Set the start time from the timestamp.
   * @param string $time
   *   Start time to set.
   */
  public function setStartTime($time) {
    CultureFeed_Cdb_Calendar::validateTime($time);
    $this->startTime = $time;
  }

  /**
   * Set the end time from the timestamp.
   * @param string $time
   *   End time to set.
   */
  public function setEndTime($time) {
    CultureFeed_Cdb_Calendar::validateTime($time);
    $this->endTime = $time;
  }

  /**
   * Set the open type for the timestamp.
   * @param string $type
   */
  public function setOpenType($type) {
    $this->openType = $type;
  }

  /**
   * @see ICultureFeed_Cdb_Element::appendToDOM()
   */
  public function appendToDOM(DOMELement $element) {

    $dom = $element->ownerDocument;

    $timestampElement = $dom->createElement('timestamp');

    $dateElement = $dom->createElement('date', $this->date);
    if ($this->openType) {
      $dateElement->setAttribute('opentype', $this->openType);
    }

    $timestampElement->appendChild($dateElement);

    if ($this->startTime) {
      $timeStartElement = $dom->createElement('timestart', $this->startTime);
      $timestampElement->appendChild($timeStartElement);
    }

    if ($this->endTime) {
      $timeEndElement = $dom->createElement('timeend');
      $timeEndElement->appendChild($dom->createTextNode($this->endTime));
      $timestampElement->appendChild($timeEndElement);
    }

    $element->appendChild($timestampElement);

  }

}
