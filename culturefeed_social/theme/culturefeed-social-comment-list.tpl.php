<?php
/**
 * @vars
 *   $activities
 *   $url 
 *   $list
 */
?>
<a href="<?php print $url ?>#schrijf">Voeg jouw reactie toe</a>

<?php if (!empty($list)): ?>
  <?php foreach ($list as $list_item): ?>
  <?php print $list_item ?>
  <?php endforeach;?>
<?php endif; ?>
