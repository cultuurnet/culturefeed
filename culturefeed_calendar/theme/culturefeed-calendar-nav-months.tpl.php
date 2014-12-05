<?php
/**
 * @file
 * Template for the calendar months nav.
 */
?>

<div class="calendar-months-navbar">
  <ul class="nav nav-tabs" role="tablist">
    <?php foreach ($months as $month_name): ?>
    <li><a href="#<?php print drupal_strtolower($month_name); ?>"><?php print $month_name; ?></a></li>
    <?php endforeach; ?>
  </ul>
</div>
