<?php
/**
 * @file
 * Template file for the list of promotions.
 */
?>

<h2><?php print t('Select one or more gifts'); ?></h2>

<?php
foreach ($items as $item) {
  print $item;
}
?>
