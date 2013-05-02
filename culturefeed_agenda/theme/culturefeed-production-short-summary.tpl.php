<?php
/**
 * @file
 * Template for the summary of an event.
 */
?>

<h3><a href="<?print $url ?>"><?print $title; ?></a></h3>
<?php if ($themes): ?>
<?php print $themes[0] ?>
<?php endif; ?>

<?php if (isset($location['city'])): ?>
<?php print $location['city']; ?>
<?php endif;?>

<?php if (!empty($thumbnail)): ?>
<img src="<?php print $thumbnail; ?>?width=160&height=120&crop=auto" />
<?php endif; ?>

<?php print culturefeed_search_detail_l('event', $cdbid, $title, 'Meer info en boeking', array('attributes' => array('class' => 'button'))); ?>
<?php print culturefeed_social_activity_recommend_link($item); ?>


