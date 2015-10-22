<?php if ($type) : ?>
  <div class="type <?php print $type ?>"><span><?php print $type ?></span></div>
<?php endif; ?>
<?php if ($picture) : ?>
  <div class="depiction"><?php print $picture ?></div>
<?php endif; ?>
<?php if ($name || $nick) : ?>
  <span class="small quiet"><?php print t('Connected to:') ?> </span>
<?php endif; ?>
<?php if ($name) : ?>
  <div class="name"><?php print $name ?></div>
<?php endif; ?>
<?php if ($nick) : ?>
  <div class="nick"><?php print $nick ?></div>
<?php endif; ?>
<?php if (!empty($publish_form)) : ?>
  <div class="publish"><?php print $publish_form; ?></div>
<?php endif; ?>
<?php if ($connect_link && !$delete_link) : ?>
  <div class="connect"><?php print $connect_link ?></div>
<?php endif; ?>
<?php if ($delete_link) : ?>
  <div class="delete"><?php print $delete_link ?></div>
<?php endif; ?>