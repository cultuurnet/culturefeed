<?php

/**
 * @file
 * Theme the list of suggestions.
 */
?>

<?php if (!empty($suggestions)): ?>
  <ul>
    <?php foreach ($suggestions as $suggestion): ?>
    <li><?php print theme('culturefeed_page_summary', array('item' => $suggestion)); ?></li>
    <?php endforeach; ?>
  </ul>
<?php endif; ?>
