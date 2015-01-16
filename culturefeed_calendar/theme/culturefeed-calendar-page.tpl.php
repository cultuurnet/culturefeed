<?php
/**
 * @file
 * Template for the calendar page.
 */
?>

<div>
  <h1><?php print t('OuTcalendar') ?></h1>
  <?php if (!empty($user_name)) : ?>
    <?php print $user_name ?>
  <?php endif; ?>
</div>

<?php if ($deny_access) : ?>
  <h3><?php print t("This calendar has not been shared yet.") ?></h3>
<?php else : ?>
  <?php if (!empty($save_cookie_button)) : ?>
    <?php print $save_cookie_button ?>
  <?php endif; ?>

  <?php if (!empty($calendar_settings_url)) : ?>
    <?php print l(t('Settings'), $calendar_settings_url) ?>
  <?php endif; ?>

  <?php if (!empty($share_calendar_url)) : ?>
    <?php print l(t('Share'), $share_calendar_url) ?>
  <?php endif; ?>

  <div>
  <?php if (!empty($nav_months)) : ?>
    <?php print $nav_months ?>
  <?php endif; ?>
  </div>

  <div>
    <?php print $sidebar; ?>
  </div>

  <div>
    <?php if (!empty($planned) || !empty($not_yet_planned)): ?>
      <?php print $not_yet_planned ?>
      <?php print $planned ?>
    <?php else: ?>
      <h3><?php print t('No activities added to your calendar yet.') ?></h3>
    <?php endif; ?>
  </div>
<?php endif; ?>
