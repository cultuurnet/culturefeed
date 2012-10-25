<?php

/**
 * @class
 * Representation of a weekscheme element in the cdb xml.
 */
class CultureFeed_Cdb_Data_Calendar_Weekscheme implements CultureFeed_Cdb_IElement {

  /**
   * Opening information for all days.
   * @var array
   */
  protected $days = array(
    'monday' => NULL,
    'tuesday' => NULL,
    'wednesday' => NULL,
    'thursday' => NULL,
    'friday' => NULL,
    'saturday' => NULL,
    'sunday' => NULL,
  );

  /**
   * Set the opening info for a given day.
   * @param string $dayName
   *   Name of the day to get.
   * @param CultureFeed_Cdb_Data_Calendar_SchemeDay $openingInfo
   *
   * @throws Exception
   */
  public function setDay($dayName, CultureFeed_Cdb_Data_Calendar_SchemeDay $openingInfo) {

    if (!array_key_exists($dayName, $this->days)) {
      throw new Exception('Trying to set unexisting day ' . $dayName);
    }

    $this->days[$dayName] = $openingInfo;

  }

  /**
   * Get the openings info for a given day.
   * @param string $dayName
   *
   *
   * @throws Exception
   */
  public function getDay($dayName) {

    if (!array_key_exists($dayName, $this->days)) {
      throw new Exception('Trying to access unexisting day ' . $dayName);
    }

    return $this->days[$dayName];

  }

  /**
   * @see CultureFeed_Cdb_IElement::appendToDOM()
   */
  public function appendToDOM(DOMELement $element) {

    $dom = $element->ownerDocument;

    $schemeElement = $dom->createElement('weekscheme');
    foreach ($this->days as $day) {
      if ($day) {
        $day->appendToDom($schemeElement);
      }
    }

    $element->appendChild($schemeElement);

  }

  /**
   * @see CultureFeed_Cdb_IElement::parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement)
   * @return CultureFeed_Cdb_Data_Calendar_Weekscheme
   */
  public static function parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement) {

    foreach (CultureFeed_Cdb_Data_Calendar_SchemeDay::$allowedDays as $day) {
      if (!isset($xmlElement->{$day})) {
        throw new CultureFeed_ParseException("Missing required data for " . $day);
      }
    }

    $weekscheme = new CultureFeed_Cdb_Data_Calendar_Weekscheme();

    $weekscheme->setDay('monday', CultureFeed_Cdb_Data_Calendar_SchemeDay::parseFromCdbXml($xmlElement->monday));
    $weekscheme->setDay('tuesday', CultureFeed_Cdb_Data_Calendar_SchemeDay::parseFromCdbXml($xmlElement->tuesday));
    $weekscheme->setDay('wednesday', CultureFeed_Cdb_Data_Calendar_SchemeDay::parseFromCdbXml($xmlElement->wednesday));
    $weekscheme->setDay('thursday', CultureFeed_Cdb_Data_Calendar_SchemeDay::parseFromCdbXml($xmlElement->thursday));
    $weekscheme->setDay('friday', CultureFeed_Cdb_Data_Calendar_SchemeDay::parseFromCdbXml($xmlElement->friday));
    $weekscheme->setDay('saturday', CultureFeed_Cdb_Data_Calendar_SchemeDay::parseFromCdbXml($xmlElement->saturday));
    $weekscheme->setDay('sunday', CultureFeed_Cdb_Data_Calendar_SchemeDay::parseFromCdbXml($xmlElement->sunday));

    return $weekscheme;

  }

}
