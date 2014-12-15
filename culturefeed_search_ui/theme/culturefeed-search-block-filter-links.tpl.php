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
<?php foreach ($items as $item): ?>
  <li>
    <?php print $item['data'] ?>
  </li>
<?php endforeach; ?>
</ul>