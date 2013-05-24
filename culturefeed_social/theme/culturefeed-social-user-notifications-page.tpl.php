<div class="new-messages">

  <?php if ($new_notifications_count): ?>
    <h2><?php print format_plural($new_notifications_count, '@count nieuwe melding', '@count nieuwe meldingen') ?></h2>
    <?php print $new_notifications; ?>
  <?php else: ?>
    <div class="alert alert-info"><p>Je hebt geen nieuwe meldingen</p></div>
  <?php endif; ?>

</div>

<?php if ($read_notifications_count): ?>
<div class="read-messages">

    <h2>Eerder gelezen meldingen</h2>
    <?php print $read_notifications; ?>

</div>
<?php endif; ?>
