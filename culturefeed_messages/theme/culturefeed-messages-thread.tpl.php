<h2><?php print $sender; ?></h2>
<br/><?php print format_plural($message_count, '@count message', '@count messages') ?><br/>
<?php print $delete_link; ?><br/>

<?php foreach ($messages as $message): ?>
  <?php print $message ?>
<?php endforeach; ?>

<div id="thread-delete-confirm" class="modal hide fade" tabindex="-1" role="dialog" aria-hidden="true">
  <div class="modal-body"></div>
</div>