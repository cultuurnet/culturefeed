<?php
/**
 * @file
 * Template for a list of saved searches.
 */
?>

<div>

  <?php print format_plural($total_searches, t('1 saved search'), t('@total saved searches', array('@total' => $total_searches))); ?>

  <?php foreach ($items as $item): ?>
  <div>
    <a href="<?php print $item['search_url']; ?>"><?php print $item['title']; ?></a>
  </div>
  <?php endforeach; ?>
</div>


