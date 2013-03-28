<?php
/**
 * @file
 * Template file for one culturefeed search facet.
 */

/**
 * @var string title
 * @var string $items
 */
?>

<?php if (!empty($title)) : ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>

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