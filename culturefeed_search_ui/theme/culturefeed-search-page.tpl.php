<?php
/**
 * @file
 * Template for the main section of a search page.
 *
 * @var $content
 */
?>

<p>
  <?php print format_plural($results_found, '<strong>@count</strong> result found', '<strong>@count</strong> results found'); ?>
</p>

<?php print $content; ?>
