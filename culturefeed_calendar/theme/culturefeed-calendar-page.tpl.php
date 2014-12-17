<?php
/**
 * @file
 * Template for the calendar page.
 */
?>

<?php if (!empty($add_button)) : ?>
  <?php print $add_button ?>
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
