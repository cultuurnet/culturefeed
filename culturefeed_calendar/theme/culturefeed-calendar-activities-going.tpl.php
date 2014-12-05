<?php
/**
 * @file
 * Template for the calendar 'going' activities.
 */
?>

<div class="calendar-activities-going-wrapper">
  <?php foreach($months as $month_name => $activities): ?>
    <div class="calendar-activities-month-wrapper row">
      <h4 id="<?php print drupal_strtolower($month_name); ?>"><?php print $month_name ?></h4>
      <div class="calendar-activity-wrapper col-xs-12">
        <?php foreach($activities as $activity): ?>
          <?php print theme('culturefeed_calendar_activity_summary', array('activity' => $activity)) ?>
        <?php endforeach; ?>
      </div>
    </div>
  <?php endforeach; ?>
</div>