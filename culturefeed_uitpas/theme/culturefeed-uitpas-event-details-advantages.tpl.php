<?php
/**
* @file
* Default theme implementation to display a the promotions on an event detail.
*
* Available variables:
* - $title: the title.
* - $show_all_link_default_render: the default rendering of the show all link.
* - $show_all_link_title: the title of the show all link.
* - $show_all_link_path: the path of the show all link.
* - $promotions: the promotions.
* - $promotions_array: the promotions as an easy accessible array.
* - $promotions_default_render: the promotions default rendering.
**/
?>
<div class="event-details-advantages">
  <?php if ($title): ?>
  <h3><?php print $title; ?></h3>
  <?php endif; ?>
  <?php if ($show_all_link_default_render): ?>
  <div class="all-promotions-link"><?php print $show_all_link_default_render; ?></div>
  <?php endif; ?>
  <?php if ($promotions_default_render): ?>
  <div class="promotions"><?php print $promotions_default_render; ?></div>
  <?php endif; ?>
</div>
