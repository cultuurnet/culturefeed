<?php if (!empty($add_new_url)): ?>
<h2><?php print t('Inbox'); ?></h2>
<p><a href="<?php print $add_new_url; ?>"> + <?php print t('New message'); ?></a></p>
<?php endif; ?>
  
<div class="messages">

  <?php foreach ($items as $item): ?>
    <?php print $item['data'] ?>
  <?php endforeach; ?>
  
</div>
