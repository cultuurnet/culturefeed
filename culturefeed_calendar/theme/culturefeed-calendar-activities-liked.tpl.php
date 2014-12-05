<?php
/**
 * @file
 * Template for the calendar 'liked' activities.
 */
?>

<div class="calendar-activities-liked-wrapper row">
<h4><?php print t('Nog niet ingepland'); ?></h4>
<div class="col-xs-12">
  <table class="table table-bordered">
  <?php foreach ($activities as $activity): ?>
    <?php print theme('culturefeed_calendar_activity_mini', array('activity' => $activity)); ?>
  <?php endforeach ?>
  </table>
</div>
</div>
