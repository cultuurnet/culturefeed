<?php

/**
 * @class
 * Representation of an event on the culturefeed.
 */
class CultureFeed_Cdb_Event implements ICultureFeed_Cdb_Element {

  /**
   * External id from an event.
   *
   * @var string
   */
  protected $externalId;

  /**
   * Calendar information for the event.
   * @var CultureFeed_Cdb_Calendar
   */
  protected $calendar;

  /**
   * Details from an event.
   *
   * @var CultureFeed_Cdb_EventDetailList
   */
  protected $details;

  /**
   * Contact info for an event.
   *
   * @var CultureFeed_Cdb_ContactInfo
   */
  protected $contactInfo;

  /**
   * Location from an event.
   *
   * @var CultureFeed_Cdb_Location
   */
  protected $location;

  /**
   * Categories from the event.
   * @var CultureFeed_Cdb_CategorieList
   */
  protected $categories;

  /**
   * Get the external ID from this event.
   */
  public function getExternalId() {
    return $this->externalId;
  }

  /**
   * Get the calendar from this event.
   */
  public function getCalendar() {
    return $this->calendar;
  }

  /**
   * Get the details from this event.
   */
  public function getDetails() {
    return $this->details;
  }

  /**
   * Get the contact info from this event.
   */
  public function getContactInfo() {
    return $this->contactInfo;
  }

  /**
   * Get the categories from this event.
   */
  public function getCategories() {
    return $this->categories;
  }

  /**
   * Set the external id from this event.
   * @param string $id
   *   ID to set.
   */
  public function setExternalId($id) {
    $this->externalId = $id;
  }

  /**
   * Set the calendar data for the event.
   * @param CultureFeed_Cdb_Calendar $calendar
   *   Calendar data.
   */
  public function setCalendar(CultureFeed_Cdb_Calendar $calendar) {
    $this->calendar = $calendar;
  }

  /**
   * Set the categories from this event.
   * @param CultureFeed_Cdb_CategorieList $categories
   *   Categories to set.
   */
  public function setCategories(CultureFeed_Cdb_CategorieList $categories) {
    $this->categories = $categories;
  }

  /**
   * Set the contact info from this event.
   * @param CultureFeed_Cdb_ContactInfo $contactInfo
   *   Contact info to set.
   */
  public function setContactInfo(CultureFeed_Cdb_ContactInfo $contactInfo) {
    $this->contactInfo = $contactInfo;
  }

  /**
   * Set the location from this event.
   * @param CultureFeed_Cdb_Location $location
   *   Location to set.
   */
  public function setLocation(CultureFeed_Cdb_Location $location) {
    $this->location = $location;
  }

  /**
   * Set the details from this event.
   * @param CultureFeed_EventDetailList $details
   *   Detail information from the event.
   */
  public function setDetails(CultureFeed_Cdb_EventDetailList $details) {
    $this->details = $details;
  }

  /**
   * @see ICultureFeed_Cdb_Element::appendToDOM()
   */
  public function appendToDOM(DOMElement $element) {

    $dom = $element->ownerDocument;

    $eventElement = $dom->createElement('event');
    if ($this->externalId) {
      $eventElement->setAttribute('externalid', $this->externalId);
    }

    if ($this->calendar) {
      $this->calendar->appendToDOM($eventElement);
    }

    if ($this->categories) {
      $this->categories->appendToDOM($eventElement);
    }

    if ($this->contactInfo) {
      $this->contactInfo->appendToDOM($eventElement);
    }

    if ($this->details) {
      $this->details->appendToDOM($eventElement);
    }

    if ($this->location) {
      $this->location->appendToDOM($eventElement);
    }

    $organiser = $dom->createElement('organiser');
    $label = $dom->createElement('label', 'jco_test_api_01_organiser_labexxxxx');
    $organiser->appendChild($label);
    $eventElement->appendChild($organiser);

    $element->appendChild($eventElement);

  }

}
