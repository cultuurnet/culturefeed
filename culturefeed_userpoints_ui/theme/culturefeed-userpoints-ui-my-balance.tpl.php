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
  <li>Hoe gaat alles in zijn werk?</li>
  <li>Hoe kan ik extra punten verdienen?</li>
  <li>Waarvoor kan ik mijn punten gebruken?</li>
</ul>