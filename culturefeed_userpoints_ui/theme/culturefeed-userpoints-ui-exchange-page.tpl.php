<?php
/**
 * Template for the userpoints exchange page.
 * 
 * @vars
 *   $points
 *     raw points from API
 *   $real_points
 *     points calculated to points for this site (E.g vliegpunten).
 *   $real_points_in_wishlist
 *     Site points in wishlist
 *   $real_points_left
 *     Calculated points currently left.
 *   
 *   $cf_user
 *     the culturefeed user object
 *   $promotion_list
 *     String version of the list, 
 *     see culturefeed-userpoints-ui-promotions-list.tpl.php
 *   $promotion_form
 *     The form to exchange points.
 *   
 *   @Note that the class <span class="userpoints-points"></span> will trigger
 *   updates on the amount of the points. Example cases:
 *   1) <span class="userpoints-points"><?php print $real_points_left ?></span>
 *   2) <span><?php print $real_points ?></span>
 */

?>
Resterend aantal punten: <span class="userpoints-points"><?php print $real_points_left ?></span>

<div class="alert alert-block alert-success">Je hebt onvoldoende punten. <strong>Vanaf 500 punten</strong> kan je jouw punten inruilen voor tickets, gadgets, waardebonnen, kortingen, ... .<br />
Niet voldoende punten? Niet getreurd, Ontdek hier hoe en hoeveel punten je telkens kunt sparen.</div>

<?php print $promotions_list ?>

<?php print $wishlist ?>

<?php print $promotions_form ?>