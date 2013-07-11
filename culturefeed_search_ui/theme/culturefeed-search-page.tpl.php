<?php
/**
 * @file
 * Template for the main section of a search page.
 *
 * @var $content
 */
?>

<div class="result-title lead">
  <?php print format_plural($results_found, '<strong>@count</strong> resultaat gevonden', '<strong>@count</strong> resultaten gevonden'); ?>
</div>

<hr />

<?php print $content; ?>
