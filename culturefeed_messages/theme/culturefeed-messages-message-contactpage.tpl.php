<p>
  <?php if ($picture): ?>
    <?php print $picture; ?>
  <?php else: ?>
    <img src="http://media.uitid.be/fis/rest/download/ce126667652776f0e9e55160f12f5478/uiv/default.png?maxwidth=70&maxheight=70&crop=auto" />
  <?php endif; ?>
</p>

<blockquote>
  <p><strong><?php print $sender; ?></strong> <small><?php print $date; ?></small></p>
  <p><?php print t('To'); ?>: <?php print $recipient_links; ?></p>
  <hr>
  <p><?php print t('Subject'); ?>: <?php print $subject; ?></p>
  <p><?php print $body; ?></p>
</blockquote>
