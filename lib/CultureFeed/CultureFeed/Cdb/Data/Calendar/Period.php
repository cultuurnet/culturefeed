<?php

/**
 * @class
 * Representation of a period element in the cdb xml.
 */
class CultureFeed_Cdb_Data_Calendar_Period implements CultureFeed_Cdb_IElement {

  /**
   * Start date for this period.
   * @var string
   */
  protected $dateFrom;

  /**
   * End date for this period.
   * @var string
   */
  protected $dateTo;

  /**
   * Exceptions for this period.
   * @var CultureFeed_Cdb_Data_Calendar_Exceptions
   */
  protected $exceptions;

  /**
   * Week scheme for this period.
   * @var CultureFeed_Cdb_Data_Calendar_WeekScheme
   */
  protected $weekScheme;

  /**
   * Construct a new period.
   * @param string $dateFrom
   *   Start date for the period.
   * @param string $dateTo
   *   End date for the period.
   */
  public function __construct($dateFrom, $dateTo) {
    $this->setdateFrom($dateFrom);
    $this->setdateTo($dateTo);
  }

  /**
   * Get the start date for this period.
   */
  public function getDateFrom() {
    return $this->dateFrom;
  }

  /**
   * Get the end date for this period.
   */
  public function getDateTo() {
    return $this->dateTo;
  }

  /**
   * Get the exceptions for this period.
   */
  public function getExceptions() {
    return $this->exceptions;
  }

  /**
   * Get the weekscheme for this period.
   */
  public function getWeekScheme() {
    return $this->weekScheme;
  }

  /**
   * Set the start date for this period.
   * @param string $dateFrom
   *   Start date to set.
   */
  public function setDateFrom($dateFrom) {
    CultureFeed_Cdb_Data_Calendar::validateDate($dateFrom);
    $this->dateFrom = $dateFrom;
  }

  /**
   * Set the to date for this period.
   * @param string $dateTo
   *   End date to set.
   */
  public function setDateTo($dateTo) {
    CultureFeed_Cdb_Data_Calendar::validateDate($dateTo);
    $this->dateTo = $dateTo;
  }

  /**
   * Set the exceptions for this period.
   * @param CultureFeed_Cdb_Data_Calendar_Exceptions $exceptions
   *   Exceptions to set.
   */
  public function setExceptions(CultureFeed_Cdb_Data_Calendar_Exceptions $exceptions) {
    $this->exceptions = $exceptions;
  }

  /**
   * Set the week scheme for this period.
   * @param CultureFeed_Cdb_Data_Calendar_WeekScheme $scheme
   *   Week scheme to set.
   */
  public function setWeekScheme(CultureFeed_Cdb_Data_Calendar_WeekScheme $scheme) {
    $this->weekScheme = $scheme;
  }

  /**
   * @see CultureFeed_Cdb_IElement::appendToDOM()
   */
  public function appendToDOM(DOMELement $element) {

    $dom = $element->ownerDocument;

    $periodElement = $dom->createElement('period');
    $periodElement->appendChild($dom->createElement('datefrom', $this->dateFrom));
    $periodElement->appendChild($dom->createElement('dateto', $this->dateTo));

    if ($this->exceptions) {
      $this->exceptions->appendToDOM($periodElement);
    }

    if ($this->weekScheme) {
      $this->weekScheme->appendToDom($periodElement);
    }

    $element->appendChild($periodElement);

  }

  /**
   * @see CultureFeed_Cdb_IElement::parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement)
   * @return CultureFeed_Cdb_Data_Calendar_Period
   */
  public static function parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement) {

    if (empty($xmlElement->datefrom)) {
      throw new CultureFeed_ParseException("Date from is missing for period");
    }

    if (empty($xmlElement->dateto)) {
      throw new CultureFeed_ParseException("Date to is missing for period");
    }

    $period = new CultureFeed_Cdb_Data_Calendar_Period((string)$xmlElement->datefrom, (string)$xmlElement->dateto);

    if (!empty($xmlElement->weekscheme)) {
      $period->setWeekScheme(CultureFeed_Cdb_Data_Calendar_Weekscheme::parseFromCdbXml($xmlElement->weekscheme));
    }

    if (!empty($xmlElement->exceptions)) {
      $period->setExceptions(CultureFeed_Cdb_Data_Calendar_Exceptions::parseFromCdbXml($xmlElement->exceptions));
    }

    return $period;

  }

}
