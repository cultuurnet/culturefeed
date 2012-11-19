<?php

/**
 * @class
 * Representation of an event on the culturefeed.
 */
class CultureFeed_Cdb_Item_Event implements CultureFeed_Cdb_IElement {

  /**
   * External id from an event.
   *
   * @var string
   */
  protected $externalId;

  /**
   * Publication date for the event.
   *
   * @var string
   */
  protected $publicationDate;

  /**
   * Publication hour for the event.
   *
   * @var string
   */
  protected  $publicationTime = '00:00:00';

  /**
   * Minimum age for the event.
   * @var int
   */
  protected $ageFrom;

  /**
   * Calendar information for the event.
   * @var CultureFeed_Cdb_Data_Calendar
   */
  protected $calendar;

  /**
   * Details from an event.
   *
   * @var CultureFeed_Cdb_Data_EventDetailList
   */
  protected $details;

  /**
   * Contact info for an event.
   *
   * @var CultureFeed_Cdb_Data_ContactInfo
   */
  protected $contactInfo;

  /**
   * Location from an event.
   *
   * @var CultureFeed_Cdb_Data_Location
   */
  protected $location;

  /**
   * Organiser from an event.
   *
   * @var CultureFeed_Cdb_Data_Organiser
   */
  protected $organiser;


  /**
   * Categories from the event.
   * @var CultureFeed_Cdb_Data_CategoryList
   */
  protected $categories;

  /**
   * Keywords from the event
   * @var array List with keywords.
   */
  protected $keywords;

  /**
   * Get the external ID from this event.
   */
  public function getExternalId() {
    return $this->externalId;
  }

  /**
   * Get the publication date for this event.
   */
  public function getPublicationDate() {
    return $this->publicationDate;
  }

  /**
   * Get the publication time for this event.
   */
  public function getPublicationTime() {
    return $this->publicationTime;
  }

  /**
   * Get the minimum age for this event.
   */
  public function getAgeFrom() {
    return $this->ageFrom;
  }

  /**
   * Get the calendar from this event.
   */
  public function getCalendar() {
    return $this->calendar;
  }

  /**
   * Get the location from this event.
   */
  public function getLocation() {
    return $this->location;
  }
  
  /**
   * Get the organiser from this event.
   */
  public function getOrganiser() {
    return $this->organiser;
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
   * Get the keywords from this event.
   */
  public function getKeywords() {
    return $this->keywords;
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
   * Set the publication date for this event.
   * @param string $date
   */
  public function setPublicationDate($date) {
    CultureFeed_Cdb_Data_Calendar::validateDate($date);
    $this->publicationDate = $date;
  }

  /**
   * Set the publication time for this event.
   * @param string $time
   */
  public function setPublicationTime($time) {
    CultureFeed_Cdb_Data_Calendar::validateTime($time);
    $this->publicationTime = $time;
  }

  /**
   * Set the minimum age for this event.
   * @param int $age
   *   Minimum age.
   *
   * @throws UnexpectedValueException
   */
  public function setAgeFrom($age) {

    if (!is_numeric($age)) {
      throw new UnexpectedValueException('Invalid age: ' . $age);
    }

    $this->ageFrom = $age;

  }

  /**
   * Set the calendar data for the event.
   * @param CultureFeed_Cdb_Data_Calendar $calendar
   *   Calendar data.
   */
  public function setCalendar(CultureFeed_Cdb_Data_Calendar $calendar) {
    $this->calendar = $calendar;
  }

  /**
   * Set the contact info from this event.
   * @param CultureFeed_Cdb_Data_Calendar $contactInfo
   *   Contact info to set.
   */
  public function setContactInfo(CultureFeed_Cdb_Data_ContactInfo $contactInfo) {
    $this->contactInfo = $contactInfo;
  }

  /**
   * Set the location from this event.
   * @param CultureFeed_Cdb_Data_Location $location
   *   Location to set.
   */
  public function setLocation(CultureFeed_Cdb_Data_Location $location) {
    $this->location = $location;
  }

  /**
   * Set the organiser from this event.
   * @param CultureFeed_Cdb_Data_Organiser $organiser
   *   Organiser to set.
   */
  public function setOrganiser(CultureFeed_Cdb_Data_Organiser $organiser) {
    $this->organiser = $organiser;
  }
  
  /**
   * Set the details from this event.
   * @param CultureFeed_Cdb_Data_EventDetailList $details
   *   Detail information from the event.
   */
  public function setDetails(CultureFeed_Cdb_Data_EventDetailList $details) {
    $this->details = $details;
  }

  /**
   * Set the categories from this event.
   * @param CultureFeed_Cdb_Data_CategoryList $categories
   *   Categories to set.
   */
  public function setCategories(CultureFeed_Cdb_Data_CategoryList $categories) {
    $this->categories = $categories;
  }

  /**
   * Add a keyword to this event.
   * @param string $keyword
   *   Add a keyword.
   */
  public function addKeyword($keyword) {
    $this->keywords[$keyword] = $keyword;
  }

  /**
   * Delete a keyword from this event.
   * @param string $keyword
   *   Keyword to remove.
   */
  public function deleteKeyword($keyword) {

    if (!isset($this->keywords[$keyword])) {
      throw new Exception('Trying to remove a non-existing keyword.');
    }

    unset($this->keywords[$keyword]);

  }

  /**
   * @see CultureFeed_Cdb_IElement::appendToDOM()
   */
  public function appendToDOM(DOMElement $element) {

    $dom = $element->ownerDocument;

    $eventElement = $dom->createElement('event');

    if ($this->ageFrom) {
      $eventElement->appendChild($dom->createElement('agefrom', $this->ageFrom));
    }

    if ($this->externalId) {
      $eventElement->setAttribute('externalid', $this->externalId);
    }

    /*if ($this->publicationDate) {
      $eventElement->setAttribute('availablefrom', $this->publicationDate . $this->publicationTime);
    }*/

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
    
    if ($this->organiser) {
      $this->organiser->appendToDOM($eventElement);
    }

    $element->appendChild($eventElement);

  }

  /**
   * @see CultureFeed_Cdb_IElement::parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement)
   * @return CultureFeed_Cdb_Item_Event
   */
  public static function parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement) {

    if (empty($xmlElement->events->event)) {
      throw new CultureFeed_ParseException('No event was found in the xml');
    }

    $xmlEvent = $xmlElement->events->event;
    if (empty($xmlEvent->calendar)) {
      throw new CultureFeed_ParseException('Calendar missing for event element');
    }

    if (empty($xmlEvent->categories)) {
      throw new CultureFeed_ParseException('Categories missing for event element');
    }

    if (empty($xmlEvent->contactinfo)) {
      throw new CultureFeed_ParseException('Contact info missing for event element');
    }

    if (empty($xmlEvent->eventdetails)) {
      throw new CultureFeed_ParseException('Eventdetails missing for event element');
    }

    if (empty($xmlEvent->location)) {
      throw new CultureFeed_ParseException('Location missing for event element');
    }

    $event_attributes = $xmlEvent->attributes();
    $event = new CultureFeed_Cdb_Item_Event();

    // Set ID.
    if (isset($event_attributes['cdbid'])) {
      $event->setExternalId((string)$event_attributes['cdbid']);
    }

    if (!empty($xmlEvent->agefrom)) {
      $event->setAgeFrom((int)$xmlEvent->agefrom);
    }

    // Set calendar information.
    $calendar_type = key($xmlEvent->calendar);
    if ($calendar_type == 'permanentopeningtimes') {
      $event->setCalendar(CultureFeed_Cdb_Data_Calendar_Permanent::parseFromCdbXml($xmlEvent->calendar));
    }
    elseif ($calendar_type == 'timestamps') {
      $event->setCalendar(CultureFeed_Cdb_Data_Calendar_TimestampList::parseFromCdbXml($xmlEvent->calendar->timestamps));
    }
    elseif ($calendar_type == 'periods') {
      $event->setCalendar(CultureFeed_Cdb_Data_Calendar_PeriodList::parseFromCdbXml($xmlEvent->calendar));
    }

    // Set categories
    $event->setCategories(CultureFeed_Cdb_Data_CategoryList::parseFromCdbXml($xmlEvent->categories));

    // Set contact information.
    $event->setContactInfo(CultureFeed_Cdb_Data_ContactInfo::parseFromCdbXml($xmlEvent->contactinfo));

    // Set event details.
    $event->setDetails(CultureFeed_Cdb_Data_EventDetailList::parseFromCdbXml($xmlEvent->eventdetails));

    // Set location.
    $event->setLocation(CultureFeed_Cdb_Data_Location::parseFromCdbXml($xmlEvent->location));

    // Set organiser
    $event->setOrganiser(CultureFeed_Cdb_Data_Organiser::parseFromCdbXml($xmlEvent->organiser));
    
    // Set the keywords.
    
    if (!empty($xmlEvent->keywords)) {
      $keywords = explode(';', $xmlEvent->keywords);
      foreach ($keywords as $keyword) {
        $event->addKeyword($keyword);
      }
    }

    return $event;

  }

}
