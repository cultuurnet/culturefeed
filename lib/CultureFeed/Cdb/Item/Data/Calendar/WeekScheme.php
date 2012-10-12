<?php

/**
 * @class
 * Representation of a weekscheme element in the cdb xml.
 */
class CultureFeed_Cdb_Calendar_Weekscheme implements ICultureFeed_Cdb_Element {

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
   * @param CultureFeed_Cdb_Calendar_SchemeDay $openingInfo
   *
   * @throws Exception
   */
  public function setDay($dayName, CultureFeed_Cdb_Calendar_SchemeDay $openingInfo) {

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
   * @see ICultureFeed_Cdb_Element::appendToDOM()
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

}
