<?php
/**
 * @file
 */

use \CultuurNet\Search\Parameter;

class views_handler_field_cdb_item_creation_date extends views_handler_field_date {

  /**
   * {@inheritdoc}
   */
  function get_value($values, $field = NULL) {

    /** @var \CultuurNet\Search\ActivityStatsExtendedEntity $values */
    /** @var CultureFeed_Cdb_Item_Event $entity */
    $entity = $values->getEntity();
    $creation_date = $entity->getCreationDate();

    if ($creation_date) {
      // Some PHP versions / operating systems seem to have problems with
      // creating a date/time from a formatted time without a timezone
      // designator. Let's add one if it's missing.
      if (substr($creation_date, -1) !== 'Europe/Brussels' && !preg_match('/[-\+][0-9]{2}(:?[0-9]{2})?$/', $creation_date)) {
        $creation_date .= 'Europe/Brussels';
      }

      $timezone = new DateTimeZone('Europe/Brussels');
      $date_time = DateTime::createFromFormat(DateTime::ISO8601, $creation_date, $timezone);

      if ($date_time) {
        // Return a Unix timestamp.
        return $date_time->format('U');
      }
    }

    return NULL;
  }

  /**
   * Called to determine what to tell the clicksorter.
   */
  function click_sort($order) {

    $parameter = new Parameter\Sort('creationdate', strtolower($order));
    $this->query->add_parameter($parameter);

  }

}
