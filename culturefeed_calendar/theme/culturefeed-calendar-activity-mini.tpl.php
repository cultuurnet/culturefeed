<?php
/**
 * @file
 * Template for the calendar mini summary of an activity.
 */
?>

<h3><?php print $title; ?></h3>
<span><?php print $when_md; ?></span>
<a href="<?php print $edit_link['url'] ?>"><?php print $edit_link['text']; ?></a>
<a href="<?php print $delete_link['url'] ?>"><?php print $delete_link['text']; ?></a>