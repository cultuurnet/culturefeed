<?php

/**
 * @class
 * Representation of a organiser element in the cdb xml.
 */
class CultureFeed_Cdb_Data_Organiser implements CultureFeed_Cdb_IElement {

  /**
   * Organiser label.
   * @var string
   */
  protected $label;

  /**
   * Cdbid from organiser actor.
   */
  protected $cdbid;
  
  
  /**
   * Construct a new location.
   * @param CultureFeed_Cdb_Data_Address $address
   *   Address from the location.
   */
  public function __construct($organiser_label, $organiser_cdbid) {
    
    $this->label = $organiser_label;
    $this->cdbid = $organiser_cdbid;

  }

  /**
   * Get the cdbid for this organiser.
   */
  public function getCdbid() {
    return $this->cdbid;
  }

  /**
   * Get the label.
   */
  public function getLabel() {
    return $this->label;
  }

  /**
   * Set the cdbid for this organiser.
   * @param string $cdbid
   */
  public function setCdbid($cdbid) {
    $this->cdbid = $cdbid;
  }

  /**
   * Set the label
   * @param string $label
   *   Label to set.
   */
  public function setLabel($label) {
    $this->label = $label;
  }

  /**
   * @see CultureFeed_Cdb_IElement::appendToDOM()
   */
  public function appendToDOM(DOMELement $element) {

    $dom = $element->ownerDocument;
    
    $organiserElement = $dom->createElement('organiser');

    if ($this->label) {
      $labelElement = $dom->createElement('label', htmlentities($this->label));
      if ($this->cdbid) {
        $labelElement->setAttribute('cdbid', $this->cdbid);
      }
      $organiserElement->appendChild($labelElement);
    }

    $element->appendChild($organiserElement);

  }

  /**
   * @see CultureFeed_Cdb_IElement::parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement)
   * @return CultureFeed_Cdb_Data_Organiser
   */
  public static function parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement) {

    $organiser = new CultureFeed_Cdb_Data_Organiser();

    if (!empty($xmlElement->label)) {
      $attributes = $xmlElement->label->attributes();
      $organiser->setLabel((string)$xmlElement->label);
      if (isset($attributes['cdbid'])) {
        $organiser->setCdbid((string)$attributes['cdbid']);
      }
    }

    return $organiser;

  }

}
