<?php

/**
 * Class to represent a recommendation.
 */
class CultureFeed_Recommendation {

  /**
   * ID of the recommendation.
   *
   * @var string
   */
  public $id;

  /**
   * ID of the recommendation item (event).
   *
   * @var string
   */
  public $itemid;

  /**
   * Recommendation score.
   *
   * @var string
   */
  public $score;

  /**
   * Algorithm used to generate the recommendation.
   *
   * @var string
   */
  public $algorithm;

  /**
   * The information about the item this recommendation is about.
   *
   * @var CultureFeed_RecommendationItem
   */
  public $recommendationItem;

  /**
   * Creation date of this recommendation represented as a UNIX timestamp.
   *
   * @var integer
   */
  public $creationDate;

}