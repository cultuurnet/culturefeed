<?php
/**
 * @file
 * Template for the main section of a search page.
 *
 * @var $content
 */
?>

<div class="result-title lead">
  <?php print format_plural($results_found, '<strong>@count</strong> result found', '<strong>@count</strong> results found'); ?>
</div>

<hr />

<?php print $content; ?>
