<?php if ($message): ?>
<div class="alert"><?php print $message; ?></div>
<?php endif; ?>

<?php print render($form); ?>

<?php if ($search): ?>
<?php if (empty($items)): ?>
Er bestaat nog geen pagina van <?php print $search; ?> op <?php print $site?>
<?php else :?>
<ul>
  <?php foreach ($items as $item): ?>
  <li>
    <?php print $item; ?>
  </li>
  <?php endforeach; ?>
<?php endif;?>
</ul>

<?php print $create_message; ?>

<?php endif; ?>





