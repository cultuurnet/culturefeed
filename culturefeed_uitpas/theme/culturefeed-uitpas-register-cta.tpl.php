<?php

/**
 * @file
 * Default theme implementation to display culturefeed uitpas register cta.
 *
 * Available variables:
 * - $intro: Text to explain why you should register your uitpas.
 * - $register_link: Link to register_uitpas page.
 * - $get_title: No UiTPAS yet?.
 * - $get_link: Link to points of sales page (register_where).
 */
?>
<div class="register_cta">
  <p><?php print $intro; ?></p>
  <p><?php print $register_link; ?></p>
  <hr />
  <h5><?php print $get_title; ?></h5>
  <p><?php print $get_link; ?></p>
</div>
