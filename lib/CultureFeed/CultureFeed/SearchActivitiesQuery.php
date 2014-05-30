<?php

/**
 * Class to represent an activities search query.
 */
class CultureFeed_SearchActivitiesQuery {

  /**
   * The type of content the activity handles.
   * Possible values are represented in the CultureFeed_Activity::CONTENT_TYPE_* constants.
   *
   * @var string
   */
  public $contentType;

  /**
   * The consumption type of the activity.
   * Possible values are represented in the CultureFeed_Activity::TYPE_* constants.
   *
   * @var integer
   */
  public $type;

  /**
   * NodeId.
   *
   * @var string
   */
  public $nodeId;

  /**
   * UserId.
   *
   * @var string
   */
  public $userId;

  /**
   * onBehalfOf.
   *
   * @var string
   */
  public $onBehalfOf;

  /**
   * Consumer.
   *
   * @var string
   */
  public $consumer;

  /**
   * ActivityId.
   *
   * @var string
   */
  public $activityId;

  /**
   * Include private activities?
   *
   * @var bool
   */
  public $private;

  /**
   * Start position.
   *
   * @var integer
   */
  public $start;

  /**
   * Maximum number of results to return.
   *
   * @var integer
   */
  public $max;

  /**
   * Depth to search.
   * @var int
   */
  public $depth;

  /**
   * Set to TRUE to disable cache for this query.
   * @var bool
   */
  public $skipCache = FALSE;

  /**
   * Convert a CultureFeed_SearchActivitiesQuery object to an array that can be used as data in POST requests that expect search user query info.
   *
   * @return array
   *   Associative array representing the object. For documentation of the structure, check the CultureFeed API documentation.
   */
  public function toPostData() {
    // For most properties we can rely on get_object_vars.
    $data = get_object_vars($this);

    // Represent private as a string (true/false);
    if (isset($data['private'])) {
      $data['private'] = $data['private'] ? 'true' : 'false';
    }

    $data = array_filter($data);

    return $data;
  }

}