<?php
/**
 * @vars
 *   $activities
 *   $url
 *   $list
 */
?>
<?php if (!empty($list)): ?>

  <a href="<?php print $url ?>#schrijf"><?php print t('Add your reaction'); ?></a>

  <?php foreach ($list as $list_item): ?>
  <?php print $list_item ?>
  <?php endforeach;?>
<?php endif; ?>
