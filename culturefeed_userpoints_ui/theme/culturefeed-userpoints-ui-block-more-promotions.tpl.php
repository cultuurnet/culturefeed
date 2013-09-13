<?php
/**
 * @file 
 * Template file for a next and previous item.
 * 
 * @vars 
 *  - $prev_item_url : url to content
 *  - $prev_image_url : url of image 
 *  - $next_item_url : url of content
 *  - $next_image_url : url of image
 */

?>
<?php if (!empty($prev_item_url)): ?>
<div>
  <a href="<?php print $prev_item_url ?>">
    <?php if (!empty($prev_image_url)): ?>
    <img src="<?php print $prev_image_url ?>" />
    <?php else: ?>
    <?php print $prev_item_title; ?>
    <?php endif; ?>
  </a>
</div>
<?php endif; ?>

<?php if (!empty($next_item_url)): ?>
<div>
  <a href="<?php print $next_item_url ?>">
    <?php if (!empty($next_image_url)): ?>
    <img src="<?php print $next_image_url ?>" />
    <?php else: ?>
    <?php print $next_item_title; ?>
    <?php endif; ?>
  </a>
</div>
<?php endif; ?>
