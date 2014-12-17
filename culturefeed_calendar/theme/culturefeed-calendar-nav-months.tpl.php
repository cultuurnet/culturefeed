<?php
/**
 * @file
 * Template for the calendar months nav.
 */
?>

<div class="calendar-months-navbar">
  <ul>
    <?php foreach ($months as $month): ?>
    <li>
      <a href="#<?php print drupal_strtolower($month['full_month']); ?>">
        <?php print $month['month']; ?> <?php print $month['year']; ?>
      </a>
    </li>
    <?php endforeach; ?>
  </ul>
</div>
