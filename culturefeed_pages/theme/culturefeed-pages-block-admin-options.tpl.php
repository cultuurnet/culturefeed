<?php
/**
 * @file
 * Template file for the administrative options for a page.
 *
 * @vars
 * - $account The DrupalCultureFeed loggedin user
 * - $page The Page object
 * - $is_page_admin is Page Admin,
 * - $logged_in_as_page_admin Logged In As Page Admin
 *
 */
?>

<?php if ($logged_in_as_page_admin): ?>

<?php if (!$has_activities): ?>
  <p><?php print t('Your page currently has no published activities.'); ?> <a href="http://www.uitdatabank.be"><?php print t('Add an activity via UiTdatabank.be'); ?></a></p>
  <?php endif; ?>

  <?php if (!empty($notifications)): ?>
  <p><strong><?php print t('Latest notifications'); ?></strong>
  <div>
    <?php print $notifications; ?>
  </div>
  <?php endif; ?>
  
  <?php print $admin_menu; ?>

<?php else: ?>
  <p><?php print t('You have <strong>administrator privileges</strong> for this page. In order to make any changes, please <strong>change your active page</strong>:'); ?> <?php print $switch_link; ?></p>
  
<?php endif; ?>
