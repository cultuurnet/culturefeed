<?php

/**
 * @file
 * Default theme implementation to display culturefeed uitpas profile actions.
 *
 * Available variables:
 * - $intro: Intro text.
 * - $actions_table: The list of actions.
 */
?>
<div class="profile_actions">
  <?php if ($intro): ?>
  <div class="intro">
  <?php print $intro; ?>
  </div>
  <?php endif; ?>
  <?php print $actions_table; ?>
</div>
