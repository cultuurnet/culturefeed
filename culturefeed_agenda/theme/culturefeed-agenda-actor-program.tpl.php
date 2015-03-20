<?php
/**
 * @file
 * Actor program file.
 */
?>

<?php if (!empty($items)): ?>
  <?php foreach ($items as $item): ?>
  <?php print $item ?>
  <?php endforeach;?>
<?php endif; ?>

<a href="<?php print $search_url ?>"><?php print t('Show full calendar of') . ' ' . $title; ?></a>
