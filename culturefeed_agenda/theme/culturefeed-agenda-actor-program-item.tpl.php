<?php
/**
 * @file
 * Template file for an event or production item on the actor program page.
 */

?>
<div>
    <?php if (!empty($when_md)): ?>
    <?php print $when_md; ?>
    <?php endif; ?>

    <a href="<?php print $url ?>"><?php print $title; ?></a>

    <?php if ($location): ?>
      <?php print $location['title'] ?><?php if (!empty($location['city'])): ?><?php print $location['city']; ?><?php endif; ?> -
    <?php endif; ?>
    <?php foreach ($themes as $theme): ?>
    <?php print $theme; ?>
    <?php endforeach; ?>

    <a href="<?php print $url ?>"><?php print t('More details'); ?></a>
</div>
