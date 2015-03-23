<?php

/**
 * @vars
 *   - $items: multi-level array with children as key for the children.
 */

?>
<div class="row">
<?php foreach ($items as $tid => $item) : ?>

  <div class="col-xs-12 col-md-4">
    <h2><?php print $item['item']->name; ?></h2>
    <?php if (!empty($item['children'])): ?>
    <ul>
    <?php foreach ($item['children'] as $child_tid => $child_item) : ?>
    <li><?php print l($child_item['item']->name, 'agenda/search', array('query' => array('facet' => array('category_eventtype_id' => array($child_item['item']->tid))))); ?></li>
    <?php endforeach; ?>
    </ul>
    <?php endif; ?>
  </div>

<?php endforeach; ?>
</div>
