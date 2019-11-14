<?php

/**
 * Class to represent an external curator article.
 */
class CultureFeed_CuratorArticle {

  /**
   * ID of the article object.
   *
   * @var string
   */
  public $id;

  /**
   * Headline of the article object.
   *
   * @var string
   */
  public $headline;

  /**
   * Language of the article object.
   *
   * @var string
   */
  public $inLanguage;

  /**
   * Summary text of the article object.
   *
   * @var string
   */
  public $text;

  /**
   * The CDB ID of the item this article relates to.
   *
   * @var string
   */
  public $about;

  /**
   * The publisher of the article object.
   *
   * @var string
   */
  public $publisher;

  /**
   * The publisher logo of the article object.
   *
   * @var string|NULL
   */
  public $publisherLogo;

  /**
   * URL of the full external article.
   * @var string
   */
  public $url;

}

