<?php

/**
 * @file
 * Template for the block that contains page suggestions.
 */
?>

<a href="<?php print url('agenda/pages') ?>"><?php print t('All pages') ?></a>

<?php print drupal_render($filter_form); ?>

<div id="page-suggestions">
  Loading
</div>