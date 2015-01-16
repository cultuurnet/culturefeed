<?php
/**
 * @file
 * Template for the calendar page.
 */
?>
<?php if ($shared) : ?>

  <?php print l(t('< Back to my OuTcalendar'), $back_to_calendar_url); ?>

  <h3><?php print t('Share your OuTcalendar with your friends'); ?></h3>
  <p><?php print t('You can share this link with your friends'); ?></p>
  <p><?php print $calendar_full_share_url; ?></p>
  <p><?php print l(t('Facebook'), $facebook['url'], $facebook['attr']); ?></p>
  <p><?php print l(t('Google+'), $googleplus['url'], $googleplus['attr']); ?></p>
  <p><?php print l(t('Twitter'), $twitter['url'], $twitter['attr']); ?></p>

  <?php if ($mail['enabled']) : ?>

    <p><?php print l(t('Mail'), $mail['url'], $mail['attr']); ?></p>

  <?php endif; ?>

<?php else : ?>

  <p><?php print t('You have chosen earlier to not share your Outcalendar. You can change this in your settings.'); ?></p>
  <p>
    <?php print l(t('Adjust settings'), 'culturefeed/calendar/settings'); ?>
  </p>

<?php endif; ?>





