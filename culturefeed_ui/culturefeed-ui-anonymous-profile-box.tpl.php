<?php
/**
 * @file
 * Template file for the culturefeed ui login box.
 */

/**
 * @var string $link_login
 * @var string $link_register
 * @var string $link_login_facebook
 */
?>
<div class="culturefeed-ui culturefeed-login">
  <?php print $link_login ?>
  <?php foreach ($main_items as $item): ?>
    <li<?php if (isset($item['class'])): print ' class="' . $item['class'] . '"' ?> <?php endif;?>><?php print $item['data']; ?></li>
  <?php endforeach; ?>
</div>
