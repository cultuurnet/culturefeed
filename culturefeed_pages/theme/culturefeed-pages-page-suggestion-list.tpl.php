<?php

/**
 * @file
 * Theme the list of suggestions.
 */
?>

<?php if (!empty($suggestions)): ?>
  <ul>
    <?php foreach ($suggestions as $suggestion): ?>
            <li><?php print theme('culturefeed_pages_page_suggestion_list_item', array('item' => $suggestion)); ?></li>
      <?php endforeach; ?>
  </ul>
<?php else: ?>
  <p><?php print t('No suggestions found'); ?></p>
<?php endif; ?>
