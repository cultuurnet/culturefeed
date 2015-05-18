<?php
/**
 * @file
 * Actor program file.
 */
?>

<?php if (!empty($items)): ?>
	<ul>
  <?php foreach ($items as $item): ?>
  	<li><?php print $item ?></li>
  <?php endforeach;?>
	</ul>
	<p><?php print t('More of ') . $organiser_link; ?></p>
<?php endif; ?>
