<div class="new-messages">

  <?php if ($new_notifications_count): ?>
    <h2><?php print format_plural($new_notifications_count, 'U hebt @count nieuwe melding', 'U hebt @count nieuwe meldingen') ?></h2>
    <?php print $new_notifications; ?>
  <?php else: ?>
    <h2>U hebt geen nieuwe meldingen</h2>
  <?php endif; ?>

</div>

<?php if ($read_notifications_count): ?>
<div class="read-messages">

    <h2>Eerder gelezen meldingen</h2>
    <?php print $read_notifications; ?>

</div>
<?php endif; ?>
