<?php

/**
 * @file
 * Theme the list of suggestions.
 */
?>

<?php if (!empty($results)): ?>
  <ul>
    <?php dsm($results, 'results'); foreach ($results as $activity): ?>
      <li><?php print $activity ?></li>
    <?php endforeach; ?>
  </ul>
<?php else: ?>
  <p><?php print t('No nearby activities found'); ?></p>
<?php endif; ?>
