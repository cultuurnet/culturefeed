<?php
/**
 * @file
 * Template file for a list of users recommending the activity.
 *
 * All variables are available (as convention) in the format of:
 * - activities_total
 * - recommend_advanced
 * - list
 */
?>

<?php if ($activities_total > 0) : ?>
<p><?php print $activities_total; ?></p>

<?php print theme('item_list', array('items' => $list)); ?>
<?php endif; ?>

<p><?php print $activity_link; ?></p>
