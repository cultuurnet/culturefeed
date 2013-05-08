<?php

/**
 * @file
 * Default theme implementation to provide a recommendation form
 */

$button_printed = render($form['recommendation_send']);
?>

<div class="recommendation-left">
  <?php print render($form['recommendation_message'])?>
  <?php print render($form['recommendation_updates'])?>
</div>

<div class="recommendation-right">
  <?php print drupal_render_children($form); ?>
  <?php 
  // Render the button as last one.
  print $button_printed ?>
</div>
