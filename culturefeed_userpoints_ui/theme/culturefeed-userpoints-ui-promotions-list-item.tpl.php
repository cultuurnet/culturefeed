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
 *     $unitsLeft: Number of units left (max option in the select box).
 *   Styling
 *     $classes : the required classes (E.g. selected or not).
 *   Operations
 *     $can_exchange
 *     $remove_link
 *     $select_list (the list / dropdown for the count)
 *     $select_btn
 *
 * @note
 *   The culturefeed-userpoints-item-<?php print $id ?> should be available as wrapper
 *   for ajax requests updating the userpoint promotions.
 */
$label = $can_exchange ? 'label-success' : 'label-important';
?>
<div class="thumbnail <?php if(!$can_exchange) print 'well '; ?><?php print $classes ?>" id="culturefeed-userpoints-item-wrapper-<?php print $id ?>">
  <?php if(!empty($picture_url)): ?>
    <img src="<?php print $picture_url; ?>?width=480&height=320&crop=auto" />
  <?php endif; ?>

  <h3><?php print $link ?></h3>

  <span class="cost label <?php print $label; ?>"><?php print $real_points ?></span>
  <p class='date text-medium-grey'>
    <?php print t('Valid from'); ?> <?php print $cashingPeriodBegin; ?> <?php print (!empty($cashingPeriodEnd) ? ' ' . t('till') . ' ' . $cashingPeriodEnd : '') ?>
  </p>

  <div id="culturefeed-userpoints-item-<?php print $id ?>">

    <?php if ($unitsLeft == 0): ?>
      <?php print t('This promotion is no longer in stock.'); ?>
    <?php elseif ($can_exchange): ?>

    <div class="row-fluid">
      <div class="span12"><?php print $select_list; ?></div>
    </div>
    <div class="row-fluid">
      <div class="span12"><?php print $select_btn; ?></div>
    </div>
    <?php if($remove_link): ?>
      <div class="row-fluid">
        <div class="span12"><?php print $remove_link ?></div>
      </div>
    <?php endif; ?>

    <?php else: ?>
      <p><?php print t('You do not have enough points for this promotion.'); ?><?php print t('points'); ?> <a href="/"><?php print t('How to save extra points?'); ?></a></p>
    <?php endif; ?>

  </div>
</div>

