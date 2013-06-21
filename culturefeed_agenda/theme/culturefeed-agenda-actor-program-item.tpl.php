<?php
/**
 * @file 
 * Template file for an event or production item on the actor program page.
 */

?>

    <?php if (!empty($when)): ?>
    <?php print $when; ?>
    <?php endif; ?>
    
    <a href="<?php print $url ?>"><?php print $title; ?></a>
    
    <?php if ($location): ?>
    <?php print $location['title'] ?> <?php print $location['city']; ?> - 
    <?php endif; ?>
    <?php foreach ($themes as $theme): ?>
    <?php print $theme; ?>
    <?php endforeach; ?>
    