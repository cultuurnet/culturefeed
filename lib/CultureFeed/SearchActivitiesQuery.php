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
   * Convert a CultureFeed_SearchActivitiesQuery object to an array that can be used as data in POST requests that expect search user query info.
   *
   * @return array
   *   Associative array representing the object. For documentation of the structure, check the Culture Feed API documentation.
   */
  public function toPostData() {
    // For most properties we can rely on get_object_vars.
    $data = array_filter(get_object_vars($this));

    return $data;
  }

}