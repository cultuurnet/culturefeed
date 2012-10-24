<?php

/**
 * @class
 * Representation of a Detail element in the cdb xml.
 */
abstract class CultureFeed_Cdb_Data_Detail {

  /**
   * Title from the detail item.
   *
   * @var string
   */
  protected $title;

  /**
   * Short description from an item.
   *
   * @var string
   */
  protected $shortDescription;

  /**
   * Language from current detail.
   * @var string
   */
  protected $language;

  /**
   * Get the title from current detail.
   */
  public function getTitle() {
    return $this->title;
  }

  /**
   * Get the short description from current detail.
   */
  public function getShortDescription() {
    return $this->shortDescription;
  }

  /**
   * Get the language from current detail.
   */
  public function getLanguage() {
    return $this->language;
  }

  /**
   * Set the title from current detail.
   * @param $title
   *   Title to set.
   */
  public function setTitle($title) {
    $this->title = $title;
  }

  /**
   * Set the short description from current detail.
   */
  public function setShortDescription($description) {
    $this->shortDescription = $description;
  }

  /**
   * Get the language from current detail.
   */
  public function setLanguage($language) {
    $this->language = $language;
  }

}