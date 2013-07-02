<?php
/**
 * @file
 * Template for the main section of a search page.
 *
 * @var $content
 */
?>

<div class="result-title">
  <?php print format_plural($results_found, '@count resultaat gevonden', '@count resultaten gevonden'); ?>
</div>

<?php print $content; ?>