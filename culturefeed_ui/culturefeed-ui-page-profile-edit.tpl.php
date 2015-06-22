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

<div id="profile-edit-form">
  <?php print $form ?>
</div>
