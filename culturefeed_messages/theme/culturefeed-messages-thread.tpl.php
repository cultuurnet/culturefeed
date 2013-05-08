<h2><?php print $sender; ?></h2>
<br/><?php print $message_count; ?>berichten<br/>
<?php print $delete_link; ?>

<?php foreach ($messages as $message): ?>
  <?php print $message ?>
<?php endforeach; ?>

<div id="thread-delete-confirm"></div>