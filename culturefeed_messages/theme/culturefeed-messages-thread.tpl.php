<p><?php print format_plural($message_count, '@count message', '@count messages') ?> | </span><?php print $delete_link; ?></p>
<p><strong><?php print t('From'); ?>:</strong> <?php print $sender; ?></p>
<p><strong><?php print t('To'); ?>:</strong> <?php print $recipient ?></p>
<p><strong><?php print t('Subject'); ?>:</strong> <?php $subject ? print $subject  : print t('No subject') ; ?></p>
<hr />
<?php foreach ($messages as $message): ?>
  <?php print $message; ?>
  <hr />
<?php endforeach; ?>

<div id="thread-delete-confirm" class="modal hide fade" tabindex="-1" role="dialog" aria-hidden="true">
  <div class="modal-body"></div>
</div>
