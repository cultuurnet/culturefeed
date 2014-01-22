<?php

/**
 * @file
 * Template to render the page account edit page.
 */
?>

<div id="account-edit-form">
  <?php print $account ?>
</div><hr />

<div id="online-accounts">
  <h3><?php print t('Connected accounts'); ?></h3>
  <?php print $online_accounts ?>
</div><hr />

<div id="manage-consumers">
  <h3><?php print t('Connected applications'); ?></h3>
  <?php print t('Manage all'); ?> <?php print l(t('websites and applications'), 'culturefeed/serviceconsumers'); ?> <?php print t('who uses your UiTiD profile.'); ?>
</div>