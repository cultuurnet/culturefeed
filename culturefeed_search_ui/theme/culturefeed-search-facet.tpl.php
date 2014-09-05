<?php
/**
 * @file
 * Template file for one culturefeed search facet.
 */

/**
 * @var array $items
 */
?>
<ul class="facet-search facet-level-1">

<?php foreach ($items as $facet_item): ?>
  <li<?php if ($facet_item['has_active_sub_item']) print ' class="active-parent"'; ?>>
    <?php print $facet_item['output'] ?>
    <?php if (!empty($facet_item['sub_items'])): ?>
      <ul class="facet-search facet-level-2">
      <?php foreach ($facet_item['sub_items'] as $sub_item): ?>
        <li><?php print $sub_item; ?></li>
      <?php endforeach; ?>
      </ul>
    <?php endif; ?>
  </li>

<?php endforeach; ?>
</ul>
