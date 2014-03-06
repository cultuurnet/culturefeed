
<?php print render($form); ?>

<?php if ($search): ?>
  <?php print $total_results_message; ?>
  <?php if (!empty($items)): ?>
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





