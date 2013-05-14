
<?php if (!empty($add_new_url)): ?>
  <a href="<?php print $add_new_url; ?>">Nieuw bericht</a>
<?php endif; ?>

<ul class="messages">
<?php foreach ($items as $item): ?>
  <li><?php print $item ?></li>
<?php endforeach; ?>
</ul>