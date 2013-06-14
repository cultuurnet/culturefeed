<?php
/**
 * @file 
 * Template page for a promotion item.
 */

?>

<h2>
  <a href="<?php print $item_url ?>"><?php print $title ?></a>
</h2>

<p>Geldig tot <?php print $cashingPeriodEnd ?> <?php print ($unlimited ? 'onbeperkt' : $unitsLeft . ' x op voorraad'); ?></p>

<?php if (!empty($picture_url)): ?>
<p>
  <img style="float: right;" width="100" src="<?php print $picture_url; ?>" />
  <span><?php print $real_points ?></span>
</p>
<?php endif; ?>
