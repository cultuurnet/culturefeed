<?php
/**
 * @file
 * Template for the summary of an event.
 */
?>

<h3><a href="<?php print $url ?>"><?php print $title; ?></a></h3>
<?php if (!empty($themes)): ?>
<?php print $themes[0] ?>
<?php endif; ?>

<?php if (isset($location['city'])): ?>
<?php print $location['city']; ?>
<?php endif;?>

<?php if (!empty($thumbnail)): ?>
<img src="<?php print $thumbnail; ?>?width=160&height=120&crop=auto" />
<?php endif; ?>

<?php print culturefeed_search_detail_l('event', $cdbid, $title, t('More info'), array('attributes' => array('class' => 'button'))); ?>

<?php if (!empty($recommend_link)) : ?>
  <?php print $recommend_link; ?>
<?php endif; ?>
