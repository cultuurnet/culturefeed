<?php
/**
 * @file
 * Default theme implementation to provide a form to exchange user points
 * 
 * 
 * @Note that the wrapper of the form is defined here. This wrapper is used
 *     to toggle visibility.
 */

$button_printed = render($form['submit']);
?>

<div id="culturefeed-userpoints-exchange-form-wrapper">

  <h2><?php print t('Enter your details'); ?></h2>
  
  <p><?php print t('Information not public. Use only for contests.'); ?></p>
  
  <div class="actions">
    <?php print drupal_render_children($form); ?>
    <?php
    // Render the button as last one.
    print $button_printed ?>
  </div>
  
</div>
