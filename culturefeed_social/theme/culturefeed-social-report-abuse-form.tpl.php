<?php

/**
 * @file
 * Default theme implementation to provide a report abuse form
 */

$button_printed = render($form['submit']);
?>

<h2><?php print render($form['title']); ?></h2>

<?php print render($form['recipient']) ?>

<div class="new-message-primary">
  <?php print render($form['subject'])?>
  <?php print render($form['message'])?>
</div>

<div class="new-message-secondary">
  <?php print drupal_render_children($form); ?>
  <?php
  // Render the button as last one.
  print $button_printed ?>
</div>
