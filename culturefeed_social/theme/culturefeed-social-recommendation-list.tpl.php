<?php
/**
 * @vars
 *   $activities
 *   $url 
 *   $list
 */
?>
<a href="<?php print $url ?>#schrijf">Schrijf beoordeling</a>

<?php if (!empty($list)): ?>
  <?php foreach ($list as $list_item): ?>
  <?php print $list_item ?>
  <?php endforeach;?>
<?php endif; ?>
