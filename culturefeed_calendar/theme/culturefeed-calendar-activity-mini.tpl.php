<?php
/**
 * @file
 * Template for the calendar mini summary of an activity.
 */
?>
<tr>
  <td class="col-lg-7 col-md-7">
    <strong class="hidden-xs hidden-sm"><?php print $title; ?></strong>
    <span><?php print $when; ?></span>
  </td>
  <td class="col-lg-3 col-md-3"><a href="<?php print $plan_link['url'] ?>"><?php print $plan_link['text']; ?></a></td>
  <td class="col-lg-2 col-md-2"><?php print $remove_button; ?></td>
</tr>