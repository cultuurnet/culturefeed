<?php
/**
 * @file
 * Template file for the culturefeed social user activity wall.
 */

/**
 * @var array $items
 */
?>

<div class="culturefeed-social culturefeed-activity-list">
  <div class="activity-list-wrapper">
    <ul class="activity-list">
    <?php foreach ($items as $item): ?>
      <li><?php print $item;?></li>
    <?php endforeach; ?>
    </ul>

    <?php if (!empty($read_more_url)): ?>
    <a href="<?php print $read_more_url ?>" rel="no-follow"><?php print $read_more_text; ?></a>
    <?php endif; ?>
  </div>
</div>
