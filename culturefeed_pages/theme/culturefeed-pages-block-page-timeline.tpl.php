<?php

/**
* @file
 * Template for the timeline block.
 */
?>

<?php if ($page_admin): ?>
    <a href="<?php print url('pages/' . $page_id . '/news/add'); ?>"><?php print t('Add a news item') ?></a>
<?php endif; ?>

<?php print $activities; ?>
