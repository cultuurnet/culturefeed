<?php
/**
 * @file
 * Template for the calendar page.
 */
?>

<?php if (!empty($save_cookie_button)) : ?>
  <?php print $save_cookie_button ?>
<?php endif; ?>

<?php if (!empty($share_calendar_button)) : ?>
  <?php print $share_calendar_button ?>
<?php endif; ?>

<div>
<?php if (!empty($nav_months)) : ?>
  <?php print $nav_months ?>
<?php endif; ?>
</div>

<div>
  <?php print $sidebar; ?>
</div>

<div>
  <?php if (!empty($planned) || !empty($not_yet_planned)): ?>
    <?php print $not_yet_planned ?>
    <?php print $planned ?>
  <?php else: ?>
    <h3><?php print t('No activities added to your calendar yet.') ?></h3>
  <?php endif; ?>
</div>
