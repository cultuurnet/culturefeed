<?php
/**
 * @file
 * Template file for the culturefeed social user activity event details in the summary item.
 */

/**
 * @var string $image
 * @var string $title
 * @var string $body
 * @var string $link
 */
?>

<img width="64" src="<?php print $image ?>" alt="<?php print $title ?>" />

<a href="<?php print $url; ?>"><?php print $title ?></a>

<?php print $body ?>