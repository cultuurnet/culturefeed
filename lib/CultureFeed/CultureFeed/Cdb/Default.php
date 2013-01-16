<?php

/**
 * @class
 * Representation of the cdb xml on the culturefeed.
 */
class CultureFeed_Cdb_Default {

  /**
   * Url to the cdb xml scheme.
   */
  const CDB_SCHEME_URL = 'http://www.cultuurdatabank.com/XMLSchema/CdbXSD/3.1/FINAL';

  /**
   * Name from the xml scheme.
   */
  const CDB_SCHEME_NAME = 'cdbxml';

  /**
   * List of possible items in the CDB.
   * @var array
   */
  private $items = array(
    'events' => array(),
    'actors' => array(),
    'productions' => array(),
  );

  /**
   * Add an item from a given type to the items list.
   * @param string $type
   *   Type of item to add.
   * @param $item
   *  Item to add
   * @throws Exception.
   */
  public function addItem($type, $item) {
    if (!array_key_exists($type, $this->items)) {
      throw new Exception("Trying to add an unknown item type '$type'");
    }

    $this->items[$type][] = $item;

  }

  public function getXml() {

    $dom = new DOMDocument('1.0', 'UTF-8');
    $dom->formatOutput = true;
    $dom->preserveWhiteSpace = false;

    $cdbElement = $dom->createElementNS(self::CDB_SCHEME_URL, self::CDB_SCHEME_NAME);
    $cdbElement->setAttributeNS('http://www.w3.org/2001/XMLSchema-instance', 'xsi:schemaLocation', self::CDB_SCHEME_URL .' ' . self::CDB_SCHEME_URL . '/CdbXSD.xsd');
    $dom->appendChild($cdbElement);

    foreach ($this->items as $type => $itemsFromType) {

      if ($itemsFromType) {
        $typeElement = $dom->createElement($type);
        $cdbElement->appendChild($typeElement);

        foreach ($itemsFromType as $item) {
          $item->appendToDOM($typeElement);
        }

      }

    }

    return $dom->saveXML();

  }

}