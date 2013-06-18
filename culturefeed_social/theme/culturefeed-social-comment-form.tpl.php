<?php

/**
 * @file
 * Default theme implementation to provide a comment form
 */

$button_printed = render($form['submit']);
?>
<a id="schrijf"></a>
<div class="comment-left">
  <?php print render($form['message'])?>
  <?php print render($form['update_optin'])?>
</div>

<div class="comment-right">
  <?php print drupal_render_children($form); ?>
  <?php
  // Render the button as last one.
  print $button_printed ?>
</div>
