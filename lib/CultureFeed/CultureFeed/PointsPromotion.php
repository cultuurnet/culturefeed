<?php

/**
 * Class to represent a PointsPromotion.
 */
class CultureFeed_PointsPromotion {

  /**
   * The cashing in of a promotion failed.
   */
  const CASHIN_PROMOTION_NOT_ALLOWED = 'CASHIN_PROMOTION_NOT_ALLOWED';

  /**
   * cashInState: Not possible points constraint.
   */
  const NOT_POSSIBLE_POINTS_CONSTRAINT = 'NOT_POSSIBLE_POINTS_CONSTRAINT';

  /**
   * cashInState: Not possible user volume constraint.
   */
  const NOT_POSSIBLE_USER_VOLUME_CONSTRAINT = 'NOT_POSSIBLE_USER_VOLUME_CONSTRAINT';

  /**
   * cashInstate: Possible to exchange points.
   */
  const POSSIBLE = 'POSSIBLE';

  /**
   * Periodtype: ABSOLUTE
   */
  const ABSOLUTE = "ABSOLUTE";

  /**
   * @var String constant.
   */
  public $cashInState;

  /**
   * @var Boolean
   */
  public $cashedIn;

  /**
   * @var string.
   */
  public $id;

  /**
   * @var date.
   */
  public $cashingPeriodBegin;

  /**
   * @var date
   */
  public $cashingPeriodEnd;

  /**
   * @var date
   */
  public $creationDate;

  /**
   * @var integer
   */
  public $maxAvailableUnits;

  /**
   * @var CultureFeed_PeriodConstraint
   */
  public $periodConstraint = NULL;

  /**
   * @var double.
   */
  public $points;

  /**
   * @var Boolean.
   */
  public $inSpotlight;

  /**
   * @var string.
   */
  public $title;

  /**
   * @var string.
   */
  public $description1;

  /**
   * @var string.
   */
  public $description2;

  /**
   * @var array of picture urls.
   */
  public $pictures = array();

  /**
   * @var integer.
   */
  public $unitsTaken;

  /**
   * Extract an array useable as data in POST requests that expect PointsPromotion info.
   *
   * @return array
   *   Associative array representing the PointsPromotion object.
   *   For documentation of the structure, check the Culture Feed API documentation.
   */
  public function toPostData() {
    // For most properties we can rely on get_object_vars.
    $data = get_object_vars($this);

    // Represent cashingPeriodBegin as a W3C date.
    if (isset($data['cashingPeriodBegin'])) {
      $data['cashingPeriodBegin'] = date('c', $data['cashingPeriodBegin']);
    }

    // Represent creationDate as a W3C date.
    if (isset($data['creationDate'])) {
      $data['creationDate'] = date('c', $data['creationDate']);
    }

    // Booleans to string.
    $data['cashedIn'] = $data['cashedIn'] ? "true" : "false";
    $data['inSpotlight'] = $data['inSpotlight'] ? "true" : "false";

    $data = array_filter($data);

    return $data;
  }

  /**
   * Initializes a CultureFeed_PointsPromotion object from its XML representation.
   *
   * @param CultureFeed_SimpleXMLElement $element
   * @return CultureFeed_PointsPromotion
   */
  public static function parseFromXML(CultureFeed_SimpleXMLElement $element) {

    if (empty($element->id)) {
      throw new CultureFeed_ParseException('id missing for PointsPromotions element');
    }

    if (empty($element->title)) {
      throw new CultureFeed_ParseException('title missing for PointsPromotions element');
    }

    if (empty($element->points)) {
      throw new CultureFeed_ParseException('points missing for PointsPromotions element');
    }

    $pointsPromotion = new CultureFeed_PointsPromotion();

    $pointsPromotion->id = $element->xpath_str('id');
    $pointsPromotion->cashInState = $element->xpath_str('cashInState');
    $pointsPromotion->cashedIn = $element->xpath_str('cashedIn') == "true" ? TRUE : FALSE;
    $pointsPromotion->inSpotlight = $element->xpath_str('inSpotlight') == "true" ? TRUE : FALSE;
    $pointsPromotion->cashingPeriodBegin = $element->xpath_time('cashingPeriodBegin');
    $pointsPromotion->cashingPeriodEnd = $element->xpath_time('cashingPeriodEnd');
    $pointsPromotion->creationDate = $element->xpath_time('creationDate');
    $pointsPromotion->maxAvailableUnits = $element->xpath_str('maxAvailableUnits');
    $pointsPromotion->points = $element->xpath_str('points');
    $pointsPromotion->title = $element->xpath_str('title');
    $pointsPromotion->unitsTaken = $element->xpath_str('unitsTaken');

    $pointsPromotion->description1 = $element->xpath_str('description1');
    $pointsPromotion->description2 = $element->xpath_str('description2');

    // Set relations.
    if (!empty($element->pictures) && !empty($element->pictures->picture)) {

      foreach ($element->pictures->picture as $picture) {
        $pointsPromotion->pictures[] = (string) $picture;
      }

    }

    $periodType = $element->xpath_str('periodConstraint/periodType');
    $periodVolume = $element->xpath_str('periodConstraint/periodVolume');
    if (!empty($periodType) && !empty($periodVolume)) {
      $pointsPromotion->periodConstraint = new CultureFeed_PeriodConstraint($periodType, $periodVolume);
    }

    return $pointsPromotion;

  }
}