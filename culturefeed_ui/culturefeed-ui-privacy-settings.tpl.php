<?php

/**
 * @file
 * Template to render the profile privacy settings.
 */

?>

<?php if ($intro): ?>
<div id="privacy-settings-intro">
  <?php print $intro; ?>
</div>
<?php endif; ?>

<?php print $profile_shortcuts; ?>

<div id="privacy-settings-form">
  <?php print $form; ?>
</div>
