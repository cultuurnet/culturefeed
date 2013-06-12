<?php
/**
 * @file 
 * Template file for the list of promotions.
 */
?>

<h2>1. Kies één of meerdere geschenken</h2>

<div class="clearfix">
<?php foreach ($items as $item): ?>
  <div style="float: left; margin: 10px; padding: 10px; border: 2px dashed green;">
    <?php print $item ?>
  </div>
<?php endforeach;?>
<?php print drupal_render_children($form, array()); ?>
</div>