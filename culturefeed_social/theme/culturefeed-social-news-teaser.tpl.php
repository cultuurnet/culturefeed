<?php
/**
 * @file
 * Template file for news teasers when shown in streams.
 */
?>

<?php if (!empty($teaser_image)): ?>
    <img src="<?php print $teaser_image ?>" alt="<?php print $teaser_title ?>" />
<?php endif; ?>

<a href="<?php print $url; ?>"><?php print $title; ?></a>

<?php print $summary; ?>
