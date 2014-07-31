<?php
/**
 * @file
 * Template file for news teasers when shown in streams.
 */
?>

<?php if (!empty($teaser_image)): ?>
    <img src="<?php print $teaser_image ?>" alt="<?php print $teaser_title ?>" />
<?php endif; ?>

<?php print $link ?>

<?php print $body ?>