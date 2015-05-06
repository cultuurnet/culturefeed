<?php
/**
 * @file
 * Template for the mini summary of a production.
 */
?>

<h4><a href="<?php print $url ?>"><?php print $title; ?></a></h4>
<?php if (!empty($themes)): ?>
<p><?php print $themes[0] ?></p>
<?php endif; ?>

<p><?php print $agefrom; ?>

<p>
<?php if (isset($location['city'])): ?>
<?php print $location['city']; ?>
<?php endif;?>
<?php if (isset($when_sm)): ?>
, <?php print $when_sm; ?>
<?php endif;?>
</p>

<hr />
