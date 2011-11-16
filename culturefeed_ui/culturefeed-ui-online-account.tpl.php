<?php if ($type) : ?>
  <div class="type <?php print $type ?>"><span><?php print $type ?></span></div>
<?php endif; ?>
<?php if ($picture) : ?>
  <div class="depiction"><?php print $picture ?></div>
<?php endif; ?>
<?php if ($name) : ?>
  <div class="name"><?php print $name ?></div>
<?php endif; ?>
<?php if ($nick) : ?>
  <div class="nick"><?php print $nick ?></div>
<?php endif; ?>
<?php if ($publish_link) : ?>
  <div class="publish"><?php print $publish_link ?></div>
<?php endif; ?>
<?php if ($delete_link) : ?>
  <div class="delete"><?php print $delete_link ?></div>
<?php endif; ?>
<?php if ($connect_link) : ?>
  <div class="connect"><?php print $connect_link ?></div>
<?php endif; ?>