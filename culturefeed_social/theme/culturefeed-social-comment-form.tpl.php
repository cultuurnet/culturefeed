<?php

/**
 * @file
 * Default theme implementation to provide a comment form
 */

$button_printed = render($form['submit']);
?>

<div class="recommendation-left">
  <?php print render($form['message'])?>
  <?php print render($form['update_optin'])?>
</div>

<div class="recommendation-right">
  <?php print drupal_render_children($form); ?>
  <?php
  // Render the button as last one.
  print $button_printed ?>
</div>
