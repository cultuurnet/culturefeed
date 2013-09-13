<?php
/**
 * @file
 *   Template file for a detail of a CultureFeed_PointsPromotion
 *   
 * @var 
 *   - $id
 *   - $title
 *   - $real_points
 *   - $maxAvailableUnits
 *   - $unitsTaken
 *   - $unlimited Indicates whether the item is available unlimited
 *   - $unitsLeft 
 *   - $cashingPeriodBegin
 *   - $cashingPeriodEnd
 *   - $description1
 *   - $description2
 *   - $picture_url
 *   - $exchange_link : link with fragment anchor to the id "culturefeed-userpoints-notifications"
 *   - $exchange_url : If you use this, feel free to use deeplinks.
 *     E.g. href="<?php print $exchange_url ?>#culturefeed-userpoints-notifications"
 *   
 */

?>

<h2><?php print $title ?></h2>

<p><?php print t('Expires'); ?> <?php print $cashingPeriodEnd ?> <?php print ($unlimited ? t('unlimited') : $unitsLeft . ' ' . t('x in stock')); ?></p>

<?php if (!empty($picture_url)): ?>
<p>
  <img style="float: right;" width="100" src="<?php print $picture_url; ?>" />
  <span><?php print $real_points ?></span>
</p>
<?php endif; ?>

<p><?php print $id ?></p>
<p><?php print $cashingPeriodBegin ?></p>
<p><?php print $description1 ?></p>
<p><?php print $description2 ?></p>


<p>
  
  <span><?php print $real_points ?></span>
  <?php if ($can_exchange && !$active): ?>
  <span><?php print $exchange_link ?></span>
  <?php else: ?>
  <?php endif; ?>
  
</p>