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

  <h2>Vul jouw gegevens in</h2>
  
  <p>Informatie niet publiek. Enkel voor gebruik wedstrijden. Vulputate lectus quisque aliquip suspendisse placeat nisl nostrum eu natus numquam aspernatur.
  
  <div class="actions">
    <?php print drupal_render_children($form); ?>
    <?php
    // Render the button as last one.
    print $button_printed ?>
  </div>
  
</div>
