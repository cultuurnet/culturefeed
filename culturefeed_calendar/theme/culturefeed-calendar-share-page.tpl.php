<?php
/**
 * @file
 * Template for the calendar page.
 */
?>
<?php if ($shared) : ?>

  <?php print l(t('Back to my calendar'), $back_to_calendar_path); ?>

  <h3><?php print t('Share your calendar with your friends'); ?></h3>
  <p><?php print t('You can share this link with your friends'); ?></p>
  <p><?php print $calendar_share_url; ?></p>
  <p><a href="<?php print $facebook_url; ?>" class="facebook-share"><?php print t('Facebook') ?></a></p>
  <p><a href="<?php print $messenger_url; ?>"><?php print t('Messenger') ?></a></p>
  <p><a href="<?php print $twitter_url; ?>"><?php print t('Twitter') ?></a></p>

  <?php if (!empty($mail_url)) : ?>
  <p><a href="<?php print $mail_url; ?>"><?php print t('Mail') ?></a></p>
  <?php endif; ?>

<?php else : ?>

  <p><?php print t('You have chosen earlier to not share your calendar. You can change this in your settings.'); ?></p>
  <p>
    <?php print l(t('Edit settings'), 'culturefeed/calendar/settings'); ?>
  </p>

<?php endif; ?>





