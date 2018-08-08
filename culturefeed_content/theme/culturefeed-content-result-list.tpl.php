<?php
/**
 * @file
 * Provides the template to render the results of a culturefeed content field.
 */
?>

<h2><?php print $title; ?></h2>

<?php if (empty($items)): ?>
<div><?php print t('No results found.'); ?></div>
<?php else: ?>
<div id="<?php print $wrapper_id ?>">
  <?php foreach ($items as $item): ?>
    <?php print $item; ?>
  <?php endforeach; ?>
</div>

<a href="<?php print $show_more_url; ?>"><?php print t('See more tips'); ?></a>
<?php endif; ?>