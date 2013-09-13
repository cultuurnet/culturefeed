<?php
/**
 * @file
 * Template for block for userpoints.
 * 
 * @vars
 *   $points
 *   $real_points
 *   $pointsPromotions
 *   $cf_user
 *   
 *   @Note that the class <span class="userpoints-points"></span> will trigger
 *   updates on the amount of the points. Example cases:
 *   1) <span class="userpoints-points"><?php print $real_points_left ?></span>
 *   2) <span><?php print $real_points ?></span>
 */

?>
<div>
  <span class="userpoints-mypoints"><?php print $real_points ?></span>
  <span class="userpoints-points"><?php print $real_points_left ?></span> | 
  <span><?php print l('Inruilen', 'userpoints/promotions/exchange'); ?></span>
</div>

<ul>
  <li><?php print t('How does everything work?'); ?></li>
  <li><?php print t('How can I earn extra points?'); ?></li>
  <li><?php print t('Wherefore can I use my points?'); ?></li>
</ul>