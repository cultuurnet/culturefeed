<?php if ($items): ?>
<ul>
<?php foreach ($items as $item) :?>
  <li><?php print $item; ?>
<?php endforeach; ?>
</ul>

<?php elseif ($is_admin) :?>

<div>
  <h3><?php print t('You page has published no activities yet.'); ?></h3>
  <p><?php print t('Add new activities on <a href="http://www.uitdatabank.be">www.uitdatabank.be</a>.'); ?>
</div>

<?php endif; ?>
