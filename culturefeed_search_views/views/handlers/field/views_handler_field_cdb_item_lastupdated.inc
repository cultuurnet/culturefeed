<?php
/**
 * @file
 */

class views_handler_field_cdb_item_lastupdated extends views_handler_field_date {

  /**
   * {@inheritdoc}
   */
  function get_value($values, $field = NULL) {
    /** @var \CultuurNet\Search\ActivityStatsExtendedEntity $values */
    /** @var CultureFeed_Cdb_Item_Event $entity */
    $entity = $values->getEntity();
    $last_updated = $entity->getLastUpdated();

    if ($last_updated) {
      // Some PHP versions / operating systems seem to have problems with
      // creating a date/time from a formatted time without a timezone
      // designator. Let's add one if it's missing.
      if (substr($last_updated, -1) !== 'Europe/Brussels' && !preg_match('/[-\+][0-9]{2}(:?[0-9]{2})?$/', $last_updated)) {
        $last_updated .= 'Europe/Brussels';
      }

      $timezone = new DateTimeZone('Europe/Brussels');
      $date_time = DateTime::createFromFormat(DateTime::ISO8601, $last_updated, $timezone);

      if ($date_time) {
        // Return a Unix timestamp.
        return $date_time->format('U');
      }
    }

    return NULL;
  }
}
