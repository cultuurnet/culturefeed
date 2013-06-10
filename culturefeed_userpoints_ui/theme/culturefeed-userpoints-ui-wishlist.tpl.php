<?php
/**
 * @file 
 * Template file for the list of wishlist items.
 */
?>

<?php if (!empty($items)): ?>
Je koos voor:

<div class="clearfix">
<?php foreach ($items as $item): ?>
  <div>
    <?php print $item ?>
  </div>
<?php endforeach;?>
</div>
<?php else: ?>
  Je hebt nog geen items geselecteerd.
<?php endif; ?>