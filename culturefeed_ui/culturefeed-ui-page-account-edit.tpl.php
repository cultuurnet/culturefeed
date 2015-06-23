<?php

/**
 * @file
 * Template to render the page account edit page.
 */
?>

<?php if ($intro): ?>
<div id="account-edit-intro">
  <?php print $intro; ?>
</div>
<?php endif; ?>

<?php print $profile_shortcuts; ?>

<div id="account-edit-form">
  <?php print $form ?>
</div><hr />

<div id="online-accounts">
  <h3><?php print t('Connected accounts'); ?></h3>
  <?php print $online_accounts ?>
</div><hr />

<div id="manage-consumers">
  <h3><?php print t('Connected applications'); ?> <span><?php print l(t('User history'), 'culturefeed/activities'); ?></span></h3>
  <?php print $connected_applications; ?>
</div>