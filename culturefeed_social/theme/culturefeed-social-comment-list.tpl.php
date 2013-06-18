<?php
/**
 * @vars
 *   $activities
 *   $url
 *   $list
 */
?>
<?php if (!empty($list)): ?>

  <a href="<?php print $url ?>#schrijf">Voeg jouw reactie toe</a>

  <?php foreach ($list as $list_item): ?>
  <?php print $list_item ?>
  <?php endforeach;?>
<?php endif; ?>
