<?php
/**
 * @file
 * Template file for one culturefeed search facet.
 */

/**
 * @var string $items
 */
?>

<ul class="facet-search facet-level-1">
<?php foreach ($items as $facet_item): ?>
    <?php if (!empty($facet_item['sub_items'])): ?>
      <?php foreach ($facet_item['sub_items'] as $sub_item): ?>
        <li><?php print $sub_item; ?></li>
      <?php endforeach; ?>
    <?php endif; ?>

<?php endforeach; ?>
</ul>