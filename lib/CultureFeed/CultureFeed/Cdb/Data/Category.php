<?php

/**
 * @class
 * Representation of a category element in the cdb xml.
 */
class CultureFeed_Cdb_Data_Category implements CultureFeed_Cdb_IElement {

  /**
   * Category type event_type.
   */
  const CATEGORY_TYPE_EVENT_TYPE = 'eventtype';

  /**
   * Category type theme.
   */
  const CATEGORY_TYPE_THEME = 'theme';

  /**
   * Category type publicscope.
   */
  const CATEGORY_TYPE_PUBLICSCOPE = 'publicscope';

  /**
   * Type of category.
   * @var string
   */
  protected $type;

  /**
   * ID from the category.
   * @var string
   */
  protected $id;

  /**
   * Name from the category.
   * @var string
   */
  protected $name;

  /**
   * Construct a new category.
   * @param string $type
   *   Type of category.
   * @param $id
   *   ID from the category.
   * @param $name
   *   Name from the category.
   */
  public function __construct($type, $id, $name) {
    $this->type = $type;
    $this->id = $id;
    $this->name = $name;
  }

  /**
   * Get the type of category.
   */
  public function getType() {
    return $this->type;
  }

  /**
   * Get the ID from the category.
   */
  public function getId() {
    return $this->id;
  }

  /**
   * Get the name from the category.
   */
  public function getName() {
    return $this->name;
  }

  /**
   * Set the type of category.
   * @param string $type
   *   Type of category.
   */
  public function setType($type) {
    $this->type = $type;
  }

  /**
   * Set the ID from the category.
   * @param string $id
   *   Id from the category.
   */
  public function setId($id) {
    $this->id = $id;
  }

  /**
   * Set the name from the category.
   * @param string $name
   *   Name from the category.
   */
  public function setName($name) {
    $this->name = $name;
  }

  /**
   * @see CultureFeed_Cdb_IElement::appendToDOM()
   */
  public function appendToDOM(DOMElement $element) {

    $dom = $element->ownerDocument;

    $categoryElement = $dom->createElement('category', htmlentities($this->name));
    $categoryElement->setAttribute('type', $this->type);
    $categoryElement->setAttribute('catid', $this->id);

    $element->appendChild($categoryElement);

  }

  /**
   * @see CultureFeed_Cdb_IElement::parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement)
   * @return CultureFeed_Cdb_Data_Category
   */
  public static function parseFromCdbXml(CultureFeed_SimpleXMLElement $xmlElement) {

    $attributes = $xmlElement->attributes();
    if (!isset($attributes['type'])) {
      throw new CultureFeed_ParseException("Category type missing for category element");
    }

    if (!isset($attributes['catid'])) {
      throw new CultureFeed_ParseException("Category ID missing for category element");
    }

    return new CultureFeed_Cdb_Data_Category((string)$attributes['type'], (string)$attributes['catid'], (string)$xmlElement);

  }

}
