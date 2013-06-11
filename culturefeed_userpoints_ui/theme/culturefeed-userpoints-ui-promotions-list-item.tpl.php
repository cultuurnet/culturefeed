<?php
/**
 * @file 
 * Template for one promotion list item.
 * 
 * @vars 
 *   Content
 *     $title
 *     $id
 *     $points / $real_points
 *     $cashingPeriodBegin
 *     $cashingPeriodEnd
 *     $active : Whether the promotion is active in the wishlist.
 *   Styling
 *     $classes : the required classes (E.g. selected or not).
 *   Operations
 *     $can_exchange
 *     $select_link
 *     $add_link
 *     $remove_link
 *     $volume_constraint 
 *       Whether the volume is exceeded during the call.
 *   
 * @note 
 *   The culturefeed-userpoints-item-<?php print $id ?> should be available as wrapper
 *   for ajax requests updating the userpoint promotions.
 */

?>

<div class="<?php print $classes ?>" id="culturefeed-userpoints-item-wrapper-<?php print $id ?>">
  
  <?php print $real_points ?><br />
  <?php print $title ?> (<?php print $link ?>)<br />
  Geldig van <?php print $cashingPeriodBegin; ?> <?php print (!empty($cashingPeriodEnd) ? ' tot ' . $cashingPeriodEnd : '') ?>

  <div id="culturefeed-userpoints-item-<?php print $id ?>">
    <?php if ($volume_constraint): ?>
    Dit voordeel is niet meer in voorraad.
    <?php print $remove_link ?>
    <?php elseif ($can_exchange): ?>
    <?php print $select_link ?>
    <?php print $add_link ?>
    <?php print $remove_link ?>
    <?php else: ?>
    Je hebt onvoldoende punten voor dit geschenk. 
    
    <a href="/">Hoe extra punten sparen?</a>
    <?php endif; ?>
  </div>
  
</div>

