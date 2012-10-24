<?php

/**
 * Class to represent a mailing.
 */
class CultureFeed_Mailing {

  /**
   * Frequency daily.
   */
  const MAILING_FREQUENCY_DAILY = 'DAILY';

  /**
   * Frequency weekly.
   */
  const MAILING_FREQUENCY_WEEKLY = 'WEEKLY';

  /**
   * Frequency biweekly.
   */
  const MAILING_FREQUENCY_BIWEEKLY = 'BIWEEKLY';

  /**
   * Frequency monthly.
   */
  const MAILING_FREQUENCY_MONTHLY = 'MONTHLY';

  /**
   * ID of the mailing object.
   *
   * @var string
   */
  public $id;

  /**
   * Name of the mailing object.
   *
   * @var string
   */
  public $name;

  /**
   * Template used for the mailing object.
   *
   * @var string
   */
  public $template;

  /**
   * Id of the service consumer.
   * @var string
   */
  public $consumerKey;

  /**
   * Subject from the mailing object.
   * @var string
   */
  public $subject;

  /**
   * Frequency of the mailing object beïng sent.
   * @var string
   */
  public $frequency;

  /**
   * From addres used for the mailing object.
   * @var string
   */
  public $fromAddress;

  /**
   * Day of month that the mailing will be sent.
   * @var int
   */
  public $startDay;

  /**
   * Day of the week that the mailing will be sent. (1 = monday)
   * @var int
   */
  public $startDayOfWeek;

  /**
   * Hour of the day that the mailing will be sent.
   * @var int
   */
  public $startHour;

  /**
   * Minute that the mailing will be sent.
   * @var int
   */
  public $startMinute;

  /**
   * Html entered for first textblock.
   * @var string
   */
  public $block1;

  /**
   * Html entered for second textblock.
   * @var string
   */
  public $block2;

  /**
   * Html entered for thirth textblock.
   * @var string
   */
  public $block3;

  /**
   * Html entered for fourth textblock.
   * @var string
   */
  public $block4;

  /**
   * Html entered for fifth textblock.
   * @var string
   */
  public $block5;

  /**
   * Html entered for sixth textblock.
   * @var string
   */
  public $block6;

  /**
   * Query used to load the events.
   * @var string
   */
  public $searchQuery;

  /**
   * Query used to load events based on recommendations.
   * @var string
   */
  public $recommendationQuery;

  /**
   * Boolean indicating wether the searchQuery should be used.
   * @var bool
   */
  public $searchEnabled;

  /**
   * Boolean indicating wether the recommendationQuery should be used.
   * @var bool
   */
  public $recommendationEnabled;

  /**
   * Boolean indicating wether the mail should be sent, when the returned events are empty.
   * @var bool
   */
  public $sendEmptySearchResult;

  /**
   * Boolean indicating wether the mail should be sent, when the returned recommendations are empty.
   * @var bool
   */
  public $sendEmptyRecommendationResult;

  /**
   * Boolean indicating wether the mail is sent periodically.
   * @var bool
   */
  public $enabled;

  /**
   * Convert a CultureFeed_Mailing object to an array that can be used as data in POST requests that expect mailing data.
   *
   * @return array
   *   Associative array representing the object. For documentation of the structure, check the Culture Feed API documentation.
   */
  public function toPostData() {

    // For most properties we can rely on get_object_vars.
    $data = get_object_vars($this);

    // Represent private as a string (true/false);
    $boolean_properties = array(
      'searchEnabled',
      'recommendationEnabled',
      'sendEmptySeachResult',
      'sendEmptyRecommendationResult',
      'enabled',
    );

    foreach ($boolean_properties as $property) {
      if (isset($data[$property])) {
        $data[$property] = $data[$property] ? 'true' : 'false';
      }
    }

    $data = array_filter($data);

    return $data;
  }

}

