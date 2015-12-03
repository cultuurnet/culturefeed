<?php

/**
 * @file
 * Default theme implementation to display culturefeed uitpas profile actions.
 *
 * Available variables:
 * - $actions_form: Form to set publishing of actions.
 * - $intro: Intro text.
 * - $actions_table: The list of actions.
 */
?>
<div class="profile_actions">
  <?php if ($intro): ?>
  <div class="intro">
    <p><?php print $intro; ?></p>
  </div>
  <?php endif; ?>
  <?php print $actions_table; ?>
</div>
<hr />
<?php if ($activity_preferences_form): ?>
<div class="activity-preferences-form">
  <?php print $activity_preferences_form; ?>
</div>
<?php endif; ?>
