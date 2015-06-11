<?php

/**
 * @file
 * Template to render the page account edit page.
 */
?>

<?php if ($intro): ?>
<div id="profile-edit-intro">
  <?php print $intro; ?>
</div>
<?php endif; ?>

<?php print $profile_menu; ?>

<?php if ($synchronization): ?>
<div id="profile-edit-synchronization">
  <?php print $synchronization; ?>
</div>
<?php endif; ?>

<div id="profile-edit-form">
  <?php print $form ?>
</div>
