
<?php if (!empty($add_new_url)): ?>
  <a href="<?php print $add_new_url; ?>"><?php print t('New message'); ?></a>
<?php endif; ?>

<ul class="messages">
<?php foreach ($items as $item): ?>
  <li class="<?php print $item['class'] ?>"><?php print $item['data'] ?></li>
<?php endforeach; ?>
</ul>