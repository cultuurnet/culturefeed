<a href="<?php print $url; ?>" class="message message-<?php print $status; ?> <?php print $class ?>">
  <?php $status == 'NEW' ? print '<span class="new">' . t('New') . '</span>' : '' ?>
  <strong><?php print $sender; ?></strong><br />
  <?php isset($recipient_page) ? print t('To') . ':' . ' ' . $recipient_page . '<br />' : '' ; ?>
  <small><?php print $date; ?></small>
</a>
