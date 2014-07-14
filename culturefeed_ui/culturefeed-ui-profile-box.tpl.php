<?php
/**
 * @file
 * Template file for the culturefeed ui profile box.
 */

/**
 * @var string $picture
 * @var string $nick
 * @var string $link
 * @var array $dropdown_items
 * @var array $main_items
 * @var string $logout
 */
?>

<div class="culturefeed-ui culturefeed-profile">
  <ul>
    <li><?php print $picture ?></li>
    <li><?php print $link ?></li>
    <?php foreach ($main_items as $item): ?>
      <li<?php if (isset($item['class'])): print ' class="' . $item['class'] . '"' ?> <?php endif;?>><?php print $item['data']; ?></li>
    <?php endforeach; ?>
    <li><?php print $logout ?></li>
  </ul>
</div>
