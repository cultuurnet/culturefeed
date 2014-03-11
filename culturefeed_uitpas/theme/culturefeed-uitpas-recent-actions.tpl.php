<?php

/**
 * @file
 * Default theme implementation to display culturefeed uitpas recent actions.
 *
 * Available variables:
 * - $actions_form: Form to set publishing of actions.
 * - $actions_list: The list of actions.
 */
?>
<div class="activity-preferences-form">
  <?php print $activity_preferences_form; ?>
</div>
<div class="recent-actions">
  <div class="list"><?php print $actions_list; ?></div>
</div>
