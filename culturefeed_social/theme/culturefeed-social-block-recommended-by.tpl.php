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
  <div class="recommended-times count-number pull-right"><span class="badge"><?php print $activities_total; ?></span></div>

  <?php print theme('item_list', array('items' => $lists, 'attributes' => array('class' => 'user-list list-unstyled list-inline'))); ?>
<?php endif; ?>

<?php print $recommend_advanced; ?>