<?php
/**
 * @file
 * Template file for the list of wishlist items.
 */
?>

<?php if (!empty($items)): ?>
  <h2>Je koos voor</h2>

  <table class="table table-striped">
    <thead>
      <tr>
        <th><?php print t('Title'); ?></th>
        <th><?php print t('Amount'); ?></th>
        <th><?php print t('Points'); ?></th>
        <th><?php print t('Delete'); ?></th>
      </tr>
    </thead>
    <tbody>
      <?php foreach ($items as $item): ?>
        <?php print $item ?>
      <?php endforeach;?>
    </tbody>
  </table>
<?php else: ?>
  <div class="alert">
    <span><?php print t('You have not selected any items.'); ?></span>
  </div>
<?php endif; ?>