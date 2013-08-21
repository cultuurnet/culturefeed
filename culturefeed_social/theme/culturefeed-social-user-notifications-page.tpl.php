<div class="new-messages">

  <?php if ($new_notifications_count): ?>
    <h2><?php print format_plural($new_notifications_count, '@count new notification', '@count new notifications') ?></h2>
    <?php print $new_notifications; ?>
  <?php else: ?>
    <div class="alert alert-info"><p><?php print t('You have no new notifications'); ?></p></div>
  <?php endif; ?>

</div>

<?php if ($read_notifications_count): ?>
<div class="read-messages">

    <h2><?php print t('Previously read messages'); ?></h2>
    <?php print $read_notifications; ?>

</div>
<?php endif; ?>
