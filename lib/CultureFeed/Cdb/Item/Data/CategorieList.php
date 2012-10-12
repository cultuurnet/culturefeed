<?php

/**
 * @class
 * Representation of a list of categories in the cdb xml.
 */
class CultureFeed_Cdb_CategorieList implements ICultureFeed_Cdb_Element, Iterator {

  /**
   * Current position in the list.
   * @var int
   */
  protected $position = 0;

  /**
   * The list of categories.
   * @var array
   */
  protected $categories = array();

  /**
   * Add a new category to the list.
   * @param CultureFeed_Cdb_Category $category
   *   Category to add.
   */
  public function add(CultureFeed_Cdb_Category $category) {
    $this->categories[] = $category;
  }

  /**
   * @see Iterator::rewind()
   */
  function rewind() {
    $this->position = 0;
  }

  /**
   * @see Iterator::current()
   */
  function current() {
    return $this->categories[$this->position];
  }

  /**
   * @see Iterator::key()
   */
  function key() {
    return $this->position;
  }

  /**
   * @see Iterator::next()
   */
  function next() {
    ++$this->position;
  }

  /**
   * @see Iterator::valid()
   */
  function valid() {
    return isset($this->categories[$this->position]);
  }

  /**
   * @see ICultureFeed_Cdb_Element::appendToDOM()
   */
  public function appendToDOM(DOMElement $element) {

    $dom = $element->ownerDocument;

    $categoriesElement = $dom->createElement('categories');
    foreach ($this as $category) {
      $category->appendToDom($categoriesElement);
    }

    $element->appendChild($categoriesElement);

  }

}
