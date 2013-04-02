<?php
/**
 * @file
 * Template file for one culturefeed search facet.
 */

/**
 * @var string $items
 */
?>
<ul>

<?php foreach ($items as $facet_item): ?>
  <li>
    <?php print $facet_item['output'] ?>
    <?php if (!empty($facet_item['sub_items'])): ?>
      <ul>
      <?php foreach ($facet_item['sub_items'] as $sub_item): ?>
        <li><?php print $sub_item; ?></li>
      <?php endforeach; ?>
      </ul>
    <?php endif; ?>
  </li>

<?php endforeach; ?>
</ul>