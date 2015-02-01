<?php
/**
 * @file
 * Template for the calendar settings page.
 */
?>

<?php print l(t('Back to my calendar'), $back_to_calendar_url); ?>
<h3><?php print t('Settings for your calendar'); ?></h3>
<?php print render($settings_form); ?>







