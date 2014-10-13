<?php

/**
 * Class to represent a recommendation.
 */
class CultureFeed_Recommendation {

  /**
   * Default algorithm (selected by redaction).
   */
  const RECOMMENDATION_ALGORITHM_DEFAULT = 'defaultRecommender';

  /**
   * Algorithm based on already liked items.
   */
  const RECOMMENDATION_ALGORITHM_POPULAR = 'PopRecommender';

  /**
   * Algorithm based on profile.
   */
  const RECOMMENDATION_ALGORITHM_PROFILE = 'PofileRecommender';

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