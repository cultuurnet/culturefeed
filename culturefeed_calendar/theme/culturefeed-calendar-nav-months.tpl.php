<?php
/**
 * @file
 * Template for the calendar months nav.
 */
?>

<div class="calendar-months-navbar">
  <ul class="nav nav-tabs" role="tablist">
    <?php foreach ($months as $month): ?>
    <li><a href="#<?php print drupal_strtolower($month['full_month']); ?>">
        <span class="nav-month"><?php print $month['month']; ?></span><br />
        <span class="nav-year"><?php print $month['year']; ?></span>
      </a>
    </li>
    <?php endforeach; ?>
  </ul>
</div>
